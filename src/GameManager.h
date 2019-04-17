#pragma once

#include <memory>

#include <System/System.h>

#include <AnimationHandler.h>
#include <Assets/Assets.h>

#include <Rendering/Display.h>
#include <Rendering/Renderer.h>

#include <boost/filesystem.hpp>

namespace Sourcehold
{
    namespace Events {
        class EventHandler;
    }

    namespace Parsers {
        class TgxFile;
        class Gm1File;
        class AniFile;
    }

    namespace Rendering {
        class BinkVideo;
    }

    namespace Game
    {
        using namespace Events;
        using namespace Parsers;
        using namespace System;
        using namespace Assets;
        using namespace Rendering;

        class GameManager : public AnimationHandler, public Display, public std::enable_shared_from_this<GameManager>
        {
            std::shared_ptr<EventHandler> eventHandler;
            bool running = false;
            double time = 0.0;
            GameOptions &opt;
        public:
            GameManager(GameOptions &opt);
            GameManager(const GameManager&) = delete;
            ~GameManager();

            /**
             * Called periodically by the front end,
             * updates all the components and returns
             * false if the game was stopped
             */
            bool Running();

            /**
             * Override the data directory path
             */
            void SetDirectory(boost::filesystem::path dir);

            /**
             * Clear all the file caches, every call to the
             * getter functions will lead to a reload
             */
            void ClearFileCache();

            /**
             * Load and cache an asset, 'filename' is the
             * path relative to the data directory
             */
            void Cache(boost::filesystem::path filename);

            /**
             * Asset getter functions
             */
            std::weak_ptr<TgxFile> GetTgx(boost::filesystem::path filename);
            std::weak_ptr<Gm1File> GetGm1(boost::filesystem::path filename);
            std::weak_ptr<AniFile> GetAni(boost::filesystem::path filename);
            std::weak_ptr<BinkVideo> GetBik(boost::filesystem::path filename);

            inline double GetTime() { return time; }
            inline std::shared_ptr<EventHandler> GetHandler() { return eventHandler; }
            inline boost::filesystem::path GetDirectory() { return _dataFolder; }
        protected:
            void Update();
            AssetType ExtToType(const std::string &ext);

            boost::filesystem::path _dataFolder;
            std::unordered_map<std::string, std::shared_ptr<TgxFile>> _tgxFiles;
            std::unordered_map<std::string, std::shared_ptr<Gm1File>> _gm1Files;
            std::unordered_map<std::string, std::shared_ptr<AniFile>> _aniFiles;
            std::unordered_map<std::string, std::shared_ptr<BinkVideo>> _bikFiles;
        };
    }
}
