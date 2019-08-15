#pragma once

#include <memory>
#include <utility>
#include <map>
#include <unordered_map>
#include <functional>
#include <boost/filesystem.hpp>

#include <Assets.h>

#include <Parsers/MlbFile.h>
#include <Parsers/CfgFile.h>

#include <System/System.h>

#include <Rendering/Display.h>
#include <Rendering/Renderer.h>

namespace Sourcehold {
    namespace Events {
        class EventHandler;
    }

    namespace Parsers {
        class TgxFile;
        class Gm1File;
        class AniFile;
        class CfgFile;
    }

    namespace Rendering {
        class BinkVideo;
    }

    namespace Game {
        using namespace Events;
        using namespace Parsers;
        using namespace System;
        using namespace Assets;
        using namespace Rendering;

        bool InitManager(GameOptions &opt);
        void DestroyManager();

        /**
         * Called periodically by the front end,
         * updates all the components and returns
         * false if the game was stopped
         */
        bool Running();

        /**
         * Override the game system paths
         */
        void SetDataDirectory(boost::filesystem::path dir);
        void SetSaveDirectory(boost::filesystem::path dir);

        /**
         * Load all of the files which will remain in memory
         * for the entirety of the game from the current
         * data path.
         */
        bool LoadGameData();

        /**
         * Clear all the file caches, every call to the
         * getter functions will lead to a reload
         */
        void ClearFileCache();

        /**
         * Load and cache an asset, 'filename' is an
         * absolute path
         */
        void Cache(boost::filesystem::path filename);

        /**
         * Delete a given entry from the file cache.
         * This will ensure that an asset will be freed once
         * every shared_ptr is destructed.
         */
        void DeleteCacheEntry(boost::filesystem::path filename);

        void SaveConfig();

        /**
         * Localization functions. These will get
         * a text string from the 'stronghold.mlb'
         * and 'sh.tex' files.
         */
        std::wstring GetDescription(MissionDescription index);
        std::wstring GetString(TextSection sec, uint16_t index);

        /**
         * Register a function which will tick every frame
         * Passes the time in seconds
         * Returns function ID
         * TODO: replace ID with smart ptr to avoid crashes
         */
        int RegisterFrameTick(std::function<void(double)> tick_fn);
        void DeregisterFrameTick(int ID);

        double GetTime();
        boost::filesystem::path GetDirectory();
        StrongholdEdition GetEdition();
        Resolution GetResolution();
        int GetUsernameIndex();
        CfgFile &GetCfg();

        /**
         * Asset getter functions.
         * All filenames are relative to the data directory
         */
        std::shared_ptr<TgxFile> GetTgx(boost::filesystem::path filename);
        std::shared_ptr<Gm1File> GetGm1(boost::filesystem::path filename);
        std::shared_ptr<AniFile> GetAni(boost::filesystem::path filename);
        std::shared_ptr<BinkVideo> GetBik(boost::filesystem::path filename);
    }
}
