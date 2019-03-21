#pragma once

#include <memory>

#include <System/System.h>

#include <AnimationHandler.h>
#include <Assets/Assets.h>

#include <Rendering/Display.h>
#include <Rendering/Renderer.h>

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

        class GameManager : public AnimationHandler, public Display, public std::enable_shared_from_this<Renderer>
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
                void SetDirectory(const std::string &dir);

                /**
                 * Clear all the file caches, every call to the
                 * getter functions will lead to a reload 
                 */
                void ClearFileCache();

                /**
                 * Load and cache an asset, 'filename' is the
                 * path relative to the data directory
                 */
                void Cache(const std::string &filename);

                /**
                 * Asset getter functions
                 */
                std::shared_ptr<TgxFile> GetTgx(const std::string &filename);
                std::shared_ptr<Gm1File> GetGm1(const std::string &filename);
                std::shared_ptr<AniFile> GetAni(const std::string &filename);
                std::shared_ptr<BinkVideo> GetBik(const std::string &filename);


                inline double GetTime() { return time; }
                inline std::shared_ptr<EventHandler> GetHandler() { return eventHandler; }
                inline std::string GetDirectory() { return _dataFolder; }
            protected:
                void Update();
                AssetType ExtToType(const std::string &ext);

                std::string _dataFolder;
                std::unordered_map<std::string, std::shared_ptr<TgxFile>> _tgxFiles;
                std::unordered_map<std::string, std::shared_ptr<Gm1File>> _gm1Files;
                std::unordered_map<std::string, std::shared_ptr<AniFile>> _aniFiles;
                std::unordered_map<std::string, std::shared_ptr<BinkVideo>> _bikFiles;
        };
    }
}
