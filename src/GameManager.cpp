#include <SDL2/SDL.h>

#include <GameManager.h>
#include <Assets.h>

#include <Events/EventHandler.h>

#include <System/System.h>
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
    Open(
        "Sourcehold version " SOURCEHOLD_VERSION_STRING,
        opt.width,
        opt.height,
        opt.fullscreen,
        opt.noborder
    );

    InitOpenAL();

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

void GameManager::SetDirectory(const std::string &dir) {
    _dataFolder = dir;
}

void GameManager::ClearFileCache() {
    _tgxFiles.clear();
    _gm1Files.clear();
    _aniFiles.clear();
    _bikFiles.clear();
}

void GameManager::Cache(const std::string &filename) {
    std::string ext = GetFileExtension(filename);
    if(filename.empty() || ext.empty()) {
        Logger::warning("ASSETS") << "Unable to cache asset: '" << filename << "'" << std::endl;
        return;
    }

    switch(ExtToType(ext)) {
        case TGXFILE: {
            _tgxFiles.emplace(GetFilename(filename), std::make_shared<TgxFile>(shared_from_this(), filename));
        }break;
        case GM1FILE: {
            _gm1Files.emplace(GetFilename(filename), std::make_shared<Gm1File>(shared_from_this(), filename));
        }break;
        case ANIFILE: {
            _aniFiles.emplace(GetFilename(filename), std::make_shared<AniFile>(shared_from_this(), filename));
        }break;
        case BINK_VIDEO: {
            _bikFiles.emplace(GetFilename(filename), std::make_shared<BinkVideo>(shared_from_this(), filename));
        }break;
        case UNKNOWN: {
            Logger::warning("ASSETS") << "Unknown asset type cached: '" << filename << "'" << std::endl;
        }break;
        default: break;
    }
}

std::shared_ptr<TgxFile> GameManager::GetTgx(const std::string &filename) {
    if(_tgxFiles.count(filename)) {
        return _tgxFiles.at(filename);
    }

    auto iter = _tgxFiles.emplace(filename, std::make_shared<TgxFile>(shared_from_this(), filename));
    return iter.first->second;
}

std::shared_ptr<Gm1File> GameManager::GetGm1(const std::string &filename) {
    if(_gm1Files.count(filename)) {
        return _gm1Files.at(filename);
    }

    auto iter = _gm1Files.emplace(filename, std::make_shared<Gm1File>(shared_from_this(), filename));
    return iter.first->second;
}

std::shared_ptr<AniFile> GameManager::GetAni(const std::string &filename) {
    if(_aniFiles.count(filename)) {
        return _aniFiles.at(filename);
    }

    auto iter = _aniFiles.emplace(filename, std::make_shared<AniFile>(shared_from_this(), filename));
    return iter.first->second;
}

std::shared_ptr<BinkVideo> GameManager::GetBik(const std::string &filename) {
    if(_bikFiles.count(filename)) {
        return _bikFiles.at(filename);
    }

    auto iter = _bikFiles.emplace(filename, std::make_shared<BinkVideo>(shared_from_this(), filename));
    return iter.first->second;
}

void GameManager::Update() {
    if(!IsOpen() || !eventHandler->FetchEvents()) running = false;

    AnimationHandler::Update();
    Renderer::Update();

    time = SDL_GetTicks() / 1000.0;
}

AssetType GameManager::ExtToType(const std::string &ext) {
    AssetType type = UNKNOWN;

    if(ext == ".bik") {
        type = BINK_VIDEO;
    }else if(ext == ".wav") {
        type = WAVEFILE;
    }else if(ext == ".raw") {
        type = RAWFILE;
    }else if(ext == ".tgx") {
        type = TGXFILE;
    }else if(ext == ".gm1") {
        type = GM1FILE;
    }else if(ext == ".ani") {
        type = ANIFILE;
    }else if(ext == ".map") {
        type = MAPFILE;
    }else if(ext == ".mlb") {
        type = MLBFILE;
    }else if(ext == ".act") {
        type = ACTFILE;
    }else if(ext == ".bmp") {
        type = BMPFILE;
    }else if(ext == ".txt") {
        type = TXTFILE;
    }

    return type;
}

