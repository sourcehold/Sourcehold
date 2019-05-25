#include <SDL.h>

#include <GameManager.h>

#include <Events/EventHandler.h>

#include <System/System.h>
#include <System/FileUtil.h>
#include <System/Logger.h>

#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>
#include <Parsers/AniFile.h>

#include <Audio/Audio.h>

#include <Rendering/BinkVideo.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

GameManager::GameManager(GameOptions &opt) : AnimationHandler(), Display(), opt(opt)
{
#if SOURCEHOLD_UNIX == 1
    Logger::SetColorOutput(true);
#else
    Logger::SetColorOutput(false);
#endif

    if(opt.color >= 0) Logger::SetColorOutput(opt.color == 1);

	auto dim = ResolutionToDim(opt.resolution);

    Open(
        "Sourcehold version " SOURCEHOLD_VERSION_STRING,
        dim.first,
		dim.second,
        opt.ndisp,
        opt.fullscreen,
        opt.noborder,
        opt.nograb
    );

    InitOpenAL();
    if(opt.nosound) {
        MuteOpenAL();
    }

    eventHandler = std::make_shared<EventHandler>();
    running = true;
}

GameManager::~GameManager() {
    DestroyOpenAL();
}

bool GameManager::Running() {
    GameManager::Update();
    return running;
}

void GameManager::SetDataDirectory(boost::filesystem::path dir) {
    _dataFolder = dir;
}

void GameManager::SetSaveDirectory(boost::filesystem::path dir) {
    _saveFolder = dir;
}

bool GameManager::LoadGameData() {
	DetectEdition();

    /* Detect game directories */
    _dataFolder = opt.dataDir;
    _saveFolder = GetDocumentsPath();

    if(_saveFolder.empty()) {
        /* Attempt to fall back to local dir */
        boost::filesystem::path np = _dataFolder / "../saves/";
        if(DoesFileExist(np)) {
            _saveFolder = np;
        }else {
            Logger::error("GAME") << "Location for save files could not be determined!" << std::endl;
            return false;
        }
    }else _saveFolder /= "Stronghold/Saves/";

    /* Does nothing if it already exists */
    CreateFolder(_saveFolder);

    /* Create default config */
    boost::filesystem::path cfgPath = _saveFolder / "../stronghold.cfg";
    if(!DoesFileExist(cfgPath)) {
        Logger::warning("PARSERS") << "Cfg file not found, creating one!" << std::endl;
        _cfg.SetDefaultValues();
        _cfg.WriteToDisk(_saveFolder / "../stronghold.cfg");
    }

    /* Load special files */
    if(!_mlb.LoadFromDisk(_dataFolder / "stronghold.mlb")) return false;
    if(!_cfg.LoadFromDisk(cfgPath)) return false;

    return true;
}

void GameManager::ClearFileCache() {
    _tgxFiles.clear();
    _gm1Files.clear();
    _aniFiles.clear();
    _bikFiles.clear();
}

void GameManager::Cache(boost::filesystem::path filename) {
    std::string ext = GetFileExtension(filename);
    if(filename.empty() || ext.empty()) {
        Logger::warning("ASSETS") << "Unable to cache asset: '" << filename << "'" << std::endl;
        return;
    }

    switch(ExtToType(ext)) {
    case TGX: {
        _tgxFiles.emplace(filename.string(), std::make_unique<TgxFile>(shared_from_this(), filename));
    }break;
    case GM1: {
        _gm1Files.emplace(filename.string(), std::make_unique<Gm1File>(shared_from_this(), filename));
    }break;
	case ANI: {
        _aniFiles.emplace(filename.string(), std::make_unique<AniFile>(shared_from_this(), filename));
    }break;
	case BIK: {
        _bikFiles.emplace(filename.string(), std::make_unique<BinkVideo>(shared_from_this(), filename));
    }break;
    case UNKNOWN: {
        Logger::warning("ASSETS") << "Unknown asset type cached: '" << filename << "'" << std::endl;
    }break;
    default: break;
    }
}

void GameManager::DeleteCacheEntry(boost::filesystem::path filename) {
    AssetType t = ExtToType(GetFileExtension(filename));
    switch(t) {
    case TGX: {
        auto iter = _tgxFiles.find(filename.string());
        if(iter != _tgxFiles.end()) _tgxFiles.erase(iter);
    }break;
    case GM1: {
        auto iter = _gm1Files.find(filename.string());
        if(iter != _gm1Files.end()) _gm1Files.erase(iter);
    }break;
    case ANI: {
        auto iter = _aniFiles.find(filename.string());
        if(iter != _aniFiles.end()) _aniFiles.erase(iter);
    }break;
    case BIK: {
        auto iter = _bikFiles.find(filename.string());
        if(iter != _bikFiles.end()) _bikFiles.erase(iter);
    }break;
    default: break;
    }
}

std::wstring GameManager::GetLocalizedDescription(LocalizedMissionDescription index) {
    std::wstring& str = _mlb.GetString(index);
    return str;
}

std::wstring GameManager::GetLocalizedString(LocalizedTextString index) {
    std::wstring str;
    return str;
}

std::weak_ptr<TgxFile> GameManager::GetTgx(boost::filesystem::path filename) {
    if(_tgxFiles.count(filename.string())) {
        return _tgxFiles.at(filename.string());
    }

    auto iter = _tgxFiles.emplace(filename.string(), std::make_unique<TgxFile>(shared_from_this(), filename));
    return iter.first->second;
}

std::weak_ptr<Gm1File> GameManager::GetGm1(boost::filesystem::path filename) {
    if(_gm1Files.count(filename.string())) {
        return _gm1Files.at(filename.string());
    }

    auto iter = _gm1Files.emplace(filename.string(), std::make_unique<Gm1File>(shared_from_this(), filename));
    return iter.first->second;
}

std::weak_ptr<AniFile> GameManager::GetAni(boost::filesystem::path filename) {
    if(_aniFiles.count(filename.string())) {
        return _aniFiles.at(filename.string());
    }

    auto iter = _aniFiles.emplace(filename.string(), std::make_unique<AniFile>(shared_from_this(), filename));
    return iter.first->second;
}

std::weak_ptr<BinkVideo> GameManager::GetBik(boost::filesystem::path filename) {
    if(_bikFiles.count(filename.string())) {
        return _bikFiles.at(filename.string());
    }

    auto iter = _bikFiles.emplace(filename.string(), std::make_unique<BinkVideo>(shared_from_this(), filename));
    return iter.first->second;
}

void GameManager::DetectEdition() {
	/**
	* TODO:
	* -Maybe give the user the option to manually switch editions
	* - How do you detect Collection vs Classic?
	*/

	if (DoesFileExist(_dataFolder / "gfx/SH1_Back.tgx")) {
		edition = STRONGHOLD_HD;
	}
	else {
		edition = STRONGHOLD_CLASSIC;
	}
}

void GameManager::Update() {
    if(!IsOpen() || !eventHandler->FetchEvents()) running = false;

    time = SDL_GetTicks() / 1000.0;
    Camera::Update();
    AnimationHandler::Update();
    Renderer::Update();
}

AssetType GameManager::ExtToType(const std::string &ext) {
    AssetType type = UNKNOWN;

    if(ext == ".bik") {
        type = BIK;
    }else if(ext == ".wav") {
        type = WAV;
    }else if(ext == ".raw") {
        type = RAW;
    }else if(ext == ".tgx") {
        type = TGX;
    }else if(ext == ".gm1") {
        type = GM1;
    }else if(ext == ".ani") {
        type = ANI;
    }else if(ext == ".map") {
        type = MAP;
    }else if(ext == ".mlb") {
        type = MLB;
    }else if(ext == ".act") {
        type = ACT;
    }else if(ext == ".bmp") {
        type = BMP;
    }else if(ext == ".txt") {
        type = TXT;
    }

    return type;
}

std::pair<int, int> GameManager::ResolutionToDim(Resolution res) {
	std::pair<int, int> dim;

	switch (res) {
	case RESOLUTION_800x600: dim.first = 800; dim.second = 600; break;
	case RESOLUTION_1024x768: dim.first = 1024; dim.second = 768; break;
	case RESOLUTION_1280x720: dim.first = 1280; dim.second = 720; break;
	case RESOLUTION_1280x1024: dim.first = 1280; dim.second = 1024; break;
	case RESOLUTION_1366x768: dim.first = 1366; dim.second = 768; break;
	case RESOLUTION_1440x900: dim.first = 1440; dim.second = 900; break;
	case RESOLUTION_1600x900: dim.first = 1600; dim.second = 900; break;
	case RESOLUTION_1600x1200: dim.first = 1600; dim.second = 1200; break;
	case RESOLUTION_1680x1050: dim.first = 1680; dim.second = 1050; break;
	case RESOLUTION_1920x1080: dim.first = 1920; dim.second = 1080; break;
	default: break;
	}

	return dim;
}

