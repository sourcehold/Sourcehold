#pragma once

#include <map>
#include <cinttypes>
#include <regex>

#include <Config.h>
#include <System/Logger.h>
#include <Parsers/Parser.h>

namespace Sourcehold
{
    namespace Parsers
    {
        /*
         * Text file containing audio volume data
         */
        class VolumeTxt : private Parser
        {
                std::map<std::string, uint8_t> volumes;
            public:
                VolumeTxt();
                ~VolumeTxt();

                bool LoadFromDisk(const std::string &path);
                void Clear();

                uint8_t GetVolumeOf(std::string key);
            protected:
                void ParseLine(std::string &line);
        };
    }
}
