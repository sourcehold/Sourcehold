#pragma once

#include <map>
#include <cinttypes>
#include <regex>

#include <System/Logger.h>
#include <Parsers/Parser.h>

#include <boost/filesystem.hpp>

namespace Sourcehold {
    namespace Parsers {
        /*
         * Text file containing audio volume data
         */
        class VolumeTxt : private Parser {
            std::map<std::string, uint8_t> volumes;
        public:
            VolumeTxt();
            ~VolumeTxt();

            bool LoadFromDisk(boost::filesystem::path path);
            void Clear();

            uint8_t GetVolumeOf(std::string key);
        protected:
            void ParseLine(std::string &line);
        };
    }
}
