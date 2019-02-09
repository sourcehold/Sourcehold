#pragma once

#include <map>
#include <cinttypes>
#include <regex>

#include <Config.h>
#include <System/Logger.h>
#include <Parsers/Parser.h>

/* Text file containing audio volume data */
namespace Sourcehold
{
    namespace Parsers
    {
        class VolumeTxt : private Parser
        {
                std::map<std::string, uint8_t> volumes;
            public:
                VolumeTxt();
                ~VolumeTxt();

                bool LoadFromDisk(std::string path);
                void Clear();

                uint8_t GetVolumeOf(std::string key);
            protected:
                void ParseLine(std::string &line);
        };
    }
}
