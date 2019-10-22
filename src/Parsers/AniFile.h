#pragma once

#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

#include "System/Logger.h"
#include "Rendering/Renderer.h"
#include "Parsers/Parser.h"

#include <filesystem>

namespace Sourcehold {
    namespace Parsers {
        /*
         * Animated cursor parser
         */
        class AniFile : public Parser {
        public:
            AniFile();
            AniFile(const AniFile&);
            AniFile(std::filesystem::path path);
            ~AniFile();

            bool LoadFromDisk(std::filesystem::path path);
        protected:
            bool ParseChunks();
            struct RiffHeader;
            struct RiffChunk;
        };
    }
}
