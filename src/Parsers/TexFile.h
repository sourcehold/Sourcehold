#pragma once

#include <boost/filesystem.hpp>
#include <cinttypes>

#include <Parsers/Parser.h>

namespace Sourcehold {
    namespace Parsers {
        class TexFile : private Parser {
        public:
            TexFile();
            ~TexFile() =default;

            bool LoadFromDisk(boost::filesystem::path path);
        protected:
        };
    }
}
