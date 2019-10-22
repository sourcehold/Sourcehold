#include <SDL.h>
#include <cstring>
#include <cwchar>
#include <chrono>

#include "GameManager.h"

#include "Events/EventHandler.h"

#include "System/System.h"
#include "System/FileUtil.h"
#include "System/Logger.h"
#include "System/ModCampaign.h"

#include "Parsers/TgxFile.h"
#include "Parsers/Gm1File.h"
#include "Parsers/AniFile.h"
#include "Parsers/HlpFile.h"
#include "Parsers/TexFile.h"

#include "Audio/Audio.h"

#include "Rendering/BinkVideo.h"
#include "Rendering/Camera.h"

#include "GUI/MenuUtils.h"

using namespace Sourcehold;
using namespace Game;
using namespace Audio;
using namespace System;
using namespace Rendering;
using namespace GUI;

int _usernameIndex = -1;
GameOptions _opt;
MlbFile _mlb;
CfgFile _cfg;
TexFile _tex;
ghc::filesystem::path _cfgPath;
ghc::filesystem::path _dataFolder;
ghc::filesystem::path _saveFolder;
std::unordered_map<std::string, std::weak_ptr<TgxFile>> _tgxFiles;
std::unordered_map<std::string, std::weak_ptr<Gm1File>> _gm1Files;
std::unordered_map<std::string, std::weak_ptr<AniFile>> _aniFiles;
std::unordered_map<std::string, std::weak_ptr<BinkVideo>> _bikFiles;
StrongholdEdition _edition;
Resolution _resolution;
bool _running = false;

bool IsAssetCached(ghc::filesystem::path path)
{
    // todo
    return false;
}

void DetectEdition()
{
    /**
    * TODO:
    * - Maybe give the user the option to manually switch editions
    * - How do you detect Collection vs Classic?
    */

    if (DoesFileExist(_dataFolder / "gfx/SH1_Back.tgx")) {
        _edition = STRONGHOLD_HD;
        Logger::message(GAME) << "Detected edition: Stronghold HD" << std::endl;
    }
    else {
        _edition = STRONGHOLD_CLASSIC;
        Logger::message(GAME) << "Detected edition: Stronghold (2001)" << std::endl;
    }
}

void DetectUsername()
{
    if(_cfg.username.empty()) {
        return;
    }

    std::vector<std::wstring> words;
    //boost::algorithm::split(words, _cfg.username, boost::is_any_of("\t "), boost::token_compress_on);
    if(words.empty()) return;

    int gender = 0;

    /* Check if 'Lord', 'Lady' or nothing */
    std::wstring username = _cfg.username;;
    if(words.size() >= 2) {
        if(words[0] == L"Lord") gender = 1;
        else if(words[0] == L"Lady") gender = 2;

        if(gender) {
            username.clear();
            for(auto it = words.begin()+1; it != words.end(); ++it) {
                username += *it + (it == words.end()-1 ? L"" : L" ");
            }
        }
    }
    /* Look-up the name and store the index */
/*
    for(size_t i = 0; i < lut_names.size(); i++) {
        const wchar_t *name = lut_names[i];
        if(!wcscmp(name, boost::to_upper_copy<std::wstring>(username).c_str())) {
            if((gender == 1 && i < NAME_INDEX_MALE) || (gender == 2 && i >= NAME_INDEX_MALE)) {
                return;
            }
            _usernameIndex = i;
            return;
        }
    }
*/
    return;
}

void UpdateGame()
{
    if(!IsDisplayOpen() || !FetchEvents()) _running = false;

    UpdateRenderer();
}

AssetType ExtToType(const std::string &ext)
{
    AssetType type = UNKNOWN;

    if(ext == ".bik") {
        type = BIK;
    }
    else if(ext == ".wav") {
        type = WAV;
    }
    else if(ext == ".raw") {
        type = RAW;
    }
    else if(ext == ".tgx") {
        type = TGX;
    }
    else if(ext == ".gm1") {
        type = GM1;
    }
    else if(ext == ".ani") {
        type = ANI;
    }
    else if(ext == ".map") {
        type = MAP;
    }
    else if(ext == ".mlb") {
        type = MLB;
    }
    else if(ext == ".act") {
        type = ACT;
    }
    else if(ext == ".bmp") {
        type = BMP;
    }
    else if(ext == ".txt") {
        type = TXT;
    }

    return type;
}

bool Game::InitManager(GameOptions &opt, Resolution res)
{
    _opt = opt;
    _resolution = res;

    InitDisplay(
        "Sourcehold version " SOURCEHOLD_VERSION_STRING " - " SOURCEHOLD_BUILD,
        opt.width,
        opt.height,
        _opt.ndisp,
        _opt.fullscreen,
        _opt.noborder,
        _opt.nograb
    );

    InitRenderer();
    InitOpenAL();

    if(_opt.nosound) {
        MuteOpenAL();
    }

    _running = true;
    return true;
}

void Game::DestroyManager()
{
    DestroyOpenAL();
    DestroyDisplay();
    DestroyRenderer();
}

bool Game::Running()
{
    UpdateGame();
    return _running;
}

void Game::SetDataDirectory(ghc::filesystem::path dir)
{
    _dataFolder = dir;
}

void Game::SetSaveDirectory(ghc::filesystem::path dir)
{
    _saveFolder = dir;
}

bool Game::LoadGameData()
{
    /* Detect game directories */
    _dataFolder = _opt.dataDir;
    _saveFolder = GetDocumentsPath();

    if(_saveFolder.empty()) {
        /* Attempt to fall back to local dir */
        ghc::filesystem::path np = "../saves/";
        if(DoesFileExist(np)) {
            _saveFolder = np;
        }
        else {
            Logger::error(GAME) << "Location for save files could not be determined!" << std::endl;
            return false;
        }
    }
    else _saveFolder /= "Stronghold/Saves/";

    /* Create asset folders. Does nothing if it already exists. */
    CreateFolder(_saveFolder);
    CreateFolder(_dataFolder / "campaigns/");

    /* Create default config */
    _cfgPath = _saveFolder / "../stronghold.cfg";
    if(!DoesFileExist(_cfgPath)) {
        Logger::message(GAME) << "Cfg file not found, creating one!" << std::endl;
        _cfg.SetDefaultValues();
        _cfg.WriteToDisk(_saveFolder / "../stronghold.cfg");
    }

    LoadModCampaigns(_dataFolder / "campaigns/");

    /* Load special files */
    if( !_mlb.LoadFromDisk(_dataFolder / "stronghold.mlb") ||
        !_tex.LoadFromDisk(_dataFolder / "sh.tex") ||
        !_cfg.LoadFromDisk(_cfgPath) ||
        !LoadStrongholdHlp()
        ) {
        return false;
    }

    if (!_cfg.soundEnabled) {
        MuteOpenAL();
    }

    DetectEdition();
    DetectUsername();

    InitMenuUtils();
    return true;
}

void Game::ClearFileCache()
{
    _tgxFiles.clear();
    _gm1Files.clear();
    _aniFiles.clear();
    _bikFiles.clear();
}

void Game::Cache(ghc::filesystem::path filename)
{
    std::string ext = GetFileExtension(filename);
    if(filename.empty() || ext.empty()) {
        Logger::warning(GAME) << "Unable to cache asset: '" << filename << "'" << std::endl;
        return;
    }

    std::string name = GetFilename(filename);

    switch(ExtToType(ext)) {
    case TGX: {
        _tgxFiles.emplace(name, std::make_shared<TgxFile>(filename));
    }
    break;
    case GM1: {
        _gm1Files.emplace(name, std::make_shared<Gm1File>(filename));
    }
    break;
    case ANI: {
        _aniFiles.emplace(name, std::make_shared<AniFile>(filename));
    }
    break;
    case BIK: {
        _bikFiles.emplace(name, std::make_shared<BinkVideo>(filename));
    }
    break;
    case UNKNOWN: {
        Logger::warning(GAME) << "Unknown asset type cached: '" << name << "'" << std::endl;
    }
    break;
    default:
        break;
    }
}

void Game::DeleteCacheEntry(ghc::filesystem::path filename)
{
    AssetType t = ExtToType(GetFileExtension(filename));
    switch(t) {
    case TGX: {
        auto iter = _tgxFiles.find(filename.string());
        if(iter != _tgxFiles.end()) _tgxFiles.erase(iter);
    }
    break;
    case GM1: {
        auto iter = _gm1Files.find(filename.string());
        if(iter != _gm1Files.end()) _gm1Files.erase(iter);
    }
    break;
    case ANI: {
        auto iter = _aniFiles.find(filename.string());
        if(iter != _aniFiles.end()) _aniFiles.erase(iter);
    }
    break;
    case BIK: {
        auto iter = _bikFiles.find(filename.string());
        if(iter != _bikFiles.end()) _bikFiles.erase(iter);
    }
    break;
    default:
        break;
    }
}

void Game::SaveConfig()
{
    _cfg.WriteToDisk(_cfgPath);
}

std::wstring Game::GetDescription(MissionDescription index)
{
    std::wstring& str = _mlb.GetString(index);
    return str;
}

std::wstring Game::GetString(TextSection sec, uint16_t index)
{
    std::wstring str = _tex.GetString(sec, index);
    return str;
}

double Game::GetTime()
{
    return SDL_GetTicks() / 1000.0f;
}

ghc::filesystem::path Game::GetDirectory()
{
    return _dataFolder;
}

StrongholdEdition Game::GetEdition()
{
    return _edition;
}

Resolution Game::GetResolution()
{
    return _resolution;
}

int Game::GetUsernameIndex()
{
    return _usernameIndex;
}

CfgFile &Game::GetCfg()
{
    return _cfg;
}

std::shared_ptr<TgxFile> Game::GetTgx(ghc::filesystem::path filename)
{
    auto sp = _tgxFiles[filename.string()].lock();
    if (!sp) _tgxFiles[filename.string()] = sp = std::make_shared<TgxFile>(_dataFolder / filename);
    return sp;
}

std::shared_ptr<Gm1File> Game::GetGm1(ghc::filesystem::path filename)
{
#if 0
    /** TODO
     * Certain (large) gm1 files will be cached. The next time this function is called
     * and has to load the gm1, the cache will be used instead of parsing the file.
     * To save disk space, this behaviour can be disabled via '--nocache'.
     */
    const static std::vector<std::string> large_files = { // todo
        "tile_workshops.gm1",
        "tile_castle.gm1",
        "body_catapult.gm1",
        "mapedit_buttons.gm1",
        "icons_front_end.gm1",
        "icons_front_end_builder.gm1",
        "icons_front_end_combat.gm1",
        "body_trebutchet.gm1",
        "tile_buildings2.gm1",
        "interface_icons2.gm1",
        "tree_oak.gm1",
        "face800 - blank.gm1",
        "tile_goods.gm1",
        "body_farmer.gm1"
    };
    bool cache = !_opt.nocache && std::find(large_files.begin(), large_files.end(), filename.string()) == large_files.end();
    if (cache && IsAssetCached(filename)) {

    }
#endif

    auto sp = _gm1Files[filename.string()].lock();
    if (!sp) _gm1Files[filename.string()] = sp = std::make_shared<Gm1File>(_dataFolder / filename);
    return sp;
}

std::shared_ptr<AniFile> Game::GetAni(ghc::filesystem::path filename)
{
    auto sp = _aniFiles[filename.string()].lock();
    if (!sp) _aniFiles[filename.string()] = sp = std::make_shared<AniFile>(_dataFolder / filename);
    return sp;
}

std::shared_ptr<BinkVideo> Game::GetBik(ghc::filesystem::path filename)
{
    auto sp = _bikFiles[filename.string()].lock();
    if (!sp) _bikFiles[filename.string()] = sp = std::make_shared<BinkVideo>(_dataFolder / filename);
    return sp;
}
