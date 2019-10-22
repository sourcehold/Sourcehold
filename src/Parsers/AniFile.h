#pragma once

#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

#include "System/Logger.h"
#include "Rendering/Renderer.h"
#include "Parsers/Parser.h"

#include "System/filesystem.h"

namespace Sourcehold {
    namespace Parsers {
        /*
         * Animated cursor parser
         */
        class AniFile : public Parser {
        public:
            AniFile();
            AniFile(const AniFile&);
            AniFile(ghc::filesystem::path path);
            ~AniFile();

            bool LoadFromDisk(ghc::filesystem::path path);
        protected:
            bool ParseChunks();
            struct RiffHeader;
            struct RiffChunk;
        };
    }
}
