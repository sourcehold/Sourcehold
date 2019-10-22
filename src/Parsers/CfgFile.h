#pragma once

#include <string>
#include <cinttypes>

#include <filesystem>

#include "System/Logger.h"
#include "Parsers/Parser.h"

namespace Sourcehold {
    namespace Parsers {
        class CfgFile : private Parser {
        public:
            CfgFile();
            ~CfgFile() {};

            void SetDefaultValues();

            bool LoadFromDisk(std::filesystem::path path);
            bool WriteToDisk(std::filesystem::path path);

            std::wstring username;
            uint8_t speed;
            uint8_t bubbleHelp;
            uint8_t resolution;
            uint8_t soundEnabled;
            uint8_t musicVolume;
            uint8_t sfxVolume;
            uint8_t unlockedMilitary;
            uint8_t unlockedEconomic;
            uint8_t speechVolume;
            uint8_t mousePointer;
        };
    }
}
