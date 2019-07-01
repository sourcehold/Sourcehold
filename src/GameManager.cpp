#include <SDL.h>
#include <cstring>
#include <cwchar>

#include <boost/algorithm/string.hpp>

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
#include <Rendering/Camera.h>

using namespace Sourcehold;
using namespace Game;
using namespace Audio;
using namespace System;
using namespace Rendering;

static int _usernameIndex = -1;
static GameOptions _opt;
static MlbFile _mlb;
static CfgFile _cfg;
static boost::filesystem::path _dataFolder;
static boost::filesystem::path _saveFolder;
static std::unordered_map<std::string, std::shared_ptr<TgxFile>> _tgxFiles;
static std::unordered_map<std::string, std::shared_ptr<Gm1File>> _gm1Files;
static std::unordered_map<std::string, std::shared_ptr<AniFile>> _aniFiles;
static std::unordered_map<std::string, std::shared_ptr<BinkVideo>> _bikFiles;
static std::map<int, std::function<void(double)>> _tickFuncs;
static StrongholdEdition _edition;
static bool _running = false;
static double _time = 0.0;

static void DetectEdition()
{
    /**
    * TODO:
    * - Maybe give the user the option to manually switch editions
    * - How do you detect Collection vs Classic?
    */

    if (DoesFileExist(_dataFolder / "gfx/SH1_Back.tgx")) {
        _edition = STRONGHOLD_HD;
    }
    else {
        _edition = STRONGHOLD_CLASSIC;
    }
}

static void DetectUsername()
{
    if(_cfg.username.empty()) {
        return;
    }

    std::vector<std::wstring> words;
    boost::algorithm::split(words, _cfg.username, boost::is_any_of("\t "), boost::token_compress_on);
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
    for(size_t i = 0; i < lut_names.size(); i++) {
        const wchar_t *name = lut_names[i];
        if(!wcscmp(name, boost::to_upper_copy<std::wstring>(username).c_str())) {
            /* Prevent names like 'Lord Cindy' or 'Lady Aaron', no offense */
            if((gender == 1 && i < NAME_INDEX_MALE) || (gender == 2 && i >= NAME_INDEX_MALE)) {
                return;
            }
            _usernameIndex = i;
            return;
        }
    }

    return;
}

static void UpdateGame()
{
    if(!IsDisplayOpen() || !FetchEvents()) _running = false;

    /* TODO: accuracy */
    _time = SDL_GetTicks() / 1000.0;

    for(const auto & e : _tickFuncs) {
        e.second(_time);
    }

    UpdateCamera();
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

std::pair<int, int> ResolutionToDim(Resolution res)
{
    static const int resolutions[][2] = {
        { 800, 600 },
        { 1024, 768 },
        { 1280, 720 },
        { 1280, 1024 },
        { 1366, 768 },
        { 1440, 900 },
        { 1600, 900 },
        { 1600, 1200 },
        { 1680, 1050 },
        { 1920, 1080 }
    };

    int rx = resolutions[res][0];
    int ry = resolutions[res][1];

    return std::pair<int, int>(rx, ry);
}

bool Game::InitManager(GameOptions &opt)
{
    _opt = opt;

#if SOURCEHOLD_UNIX == 1
    Logger::SetColorOutput(true);
#else
    Logger::SetColorOutput(false);
#endif

    if(_opt.color >= 0) Logger::SetColorOutput(_opt.color == 1);

    auto dim = ResolutionToDim(_opt.resolution);

    InitDisplay(
        "Sourcehold version " SOURCEHOLD_VERSION_STRING " - " SOURCEHOLD_BUILD,
        dim.first,
        dim.second,
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

void Game::SetDataDirectory(boost::filesystem::path dir)
{
    _dataFolder = dir;
}

void Game::SetSaveDirectory(boost::filesystem::path dir)
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
        boost::filesystem::path np = _dataFolder / "../saves/";
        if(DoesFileExist(np)) {
            _saveFolder = np;
        }
        else {
            Logger::warning("GAME") << "Location for save files could not be determined!" << std::endl;
            return false;
        }
    }
    else _saveFolder /= "Stronghold/Saves/";

    /* Does nothing if it already exists */
    CreateFolder(_saveFolder);

    /* Create default config */
    boost::filesystem::path cfgPath = _saveFolder / "../stronghold.cfg";
    if(!DoesFileExist(cfgPath)) {
        Logger::message("PARSERS") << "Cfg file not found, creating one!" << std::endl;
        _cfg.SetDefaultValues();
        _cfg.WriteToDisk(_saveFolder / "../stronghold.cfg");
    }

    /* Load special files */
    if(!_mlb.LoadFromDisk(_dataFolder / "stronghold.mlb")) return false;
    if(!_cfg.LoadFromDisk(cfgPath)) return false;

    DetectEdition();
    DetectUsername();

    return true;
}

void Game::ClearFileCache()
{
    _tgxFiles.clear();
    _gm1Files.clear();
    _aniFiles.clear();
    _bikFiles.clear();
}

void Game::Cache(boost::filesystem::path filename)
{
    std::string ext = GetFileExtension(filename);
    if(filename.empty() || ext.empty()) {
        Logger::warning("ASSETS") << "Unable to cache asset: '" << filename << "'" << std::endl;
        return;
    }

    switch(ExtToType(ext)) {
    case TGX: {
        _tgxFiles.emplace(filename.string(), std::make_unique<TgxFile>(filename));
    }
    break;
    case GM1: {
        _gm1Files.emplace(filename.string(), std::make_unique<Gm1File>(filename));
    }
    break;
    case ANI: {
        _aniFiles.emplace(filename.string(), std::make_unique<AniFile>(filename));
    }
    break;
    case BIK: {
        _bikFiles.emplace(filename.string(), std::make_unique<BinkVideo>(filename));
    }
    break;
    case UNKNOWN: {
        Logger::warning("ASSETS") << "Unknown asset type cached: '" << filename << "'" << std::endl;
    }
    break;
    default:
        break;
    }
}

void Game::DeleteCacheEntry(boost::filesystem::path filename)
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

std::wstring Game::GetLocalizedDescription(LocalizedMissionDescription index)
{
    std::wstring& str = _mlb.GetString(index);
    return str;
}

std::wstring Game::GetLocalizedString(LocalizedTextString index)
{
    std::wstring str;
    return str;
}

int Game::RegisterFrameTick(std::function<void(double)> tick_fn)
{
    int ID = _tickFuncs.size();
    _tickFuncs.insert(std::make_pair(ID, tick_fn));
    return ID;
}

void Game::DeregisterFrameTick(int ID)
{
    _tickFuncs.erase(ID);
}

double Game::GetTime()
{
    return _time;
}

boost::filesystem::path Game::GetDirectory()
{
    return _dataFolder;
}

StrongholdEdition Game::GetEdition()
{
    return _edition;
}

Resolution Game::GetResolution()
{
    return _opt.resolution;
}

int Game::GetUsernameIndex()
{
    return _usernameIndex;
}

std::weak_ptr<TgxFile> Game::GetTgx(boost::filesystem::path filename)
{
    if(_tgxFiles.count(filename.string())) {
        return _tgxFiles.at(filename.string());
    }

    auto iter = _tgxFiles.emplace(filename.string(), std::make_unique<TgxFile>(filename));
    return iter.first->second;
}

std::weak_ptr<Gm1File> Game::GetGm1(boost::filesystem::path filename)
{
    if(_gm1Files.count(filename.string())) {
        return _gm1Files.at(filename.string());
    }

    auto iter = _gm1Files.emplace(filename.string(), std::make_unique<Gm1File>(filename));
    return iter.first->second;
}

std::weak_ptr<AniFile> Game::GetAni(boost::filesystem::path filename)
{
    if(_aniFiles.count(filename.string())) {
        return _aniFiles.at(filename.string());
    }

    auto iter = _aniFiles.emplace(filename.string(), std::make_unique<AniFile>(filename));
    return iter.first->second;
}

std::weak_ptr<BinkVideo> Game::GetBik(boost::filesystem::path filename)
{
    if(_bikFiles.count(filename.string())) {
        return _bikFiles.at(filename.string());
    }

    auto iter = _bikFiles.emplace(filename.string(), std::make_unique<BinkVideo>(filename));
    return iter.first->second;
}
