#pragma once

#include <cinttypes>
#include <boost/filesystem.hpp>

#include "Parsers/Parser.h"

#include "Rendering/Texture.h"
#include "Rendering/Surface.h"

namespace Sourcehold {
    namespace Parsers {
        using namespace Rendering;

        class MapFile : private Parser {
            Texture preview;
        public:
            MapFile();
            MapFile(boost::filesystem::path path);
            MapFile(const MapFile&) = delete;
            ~MapFile();

            bool LoadFromDisk(boost::filesystem::path path);

            inline Texture& GetPreview() { return preview; }
        protected:
            struct MapSec;
            MapSec BlastSection();
            void ParsePreview();
            uint32_t ComputeCRC32(const void *data, size_t size);
        };
    }
}
