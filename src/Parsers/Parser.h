#pragma once

#include <boost/filesystem/fstream.hpp>
#include <string>
#include <cinttypes>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>

#include <Config.h>

/* Parser super class */
namespace Sourcehold
{
    namespace Parsers
    {
        class Parser : public std::ifstream
        {
                uint32_t length = 0;
            public:
                Parser() : std::ifstream() { }; 
                ~Parser() = default;
            
                bool Open(boost::filesystem::path &path, std::ios_base::openmode mode);
                void Close();
                bool Ok();
                bool GetData(void *buf, size_t bufsize);

                std::string GetUTF16();
                std::string GetLine();
                uint8_t GetByte();
                uint16_t GetWord();
                uint32_t GetLength();
                uint32_t GetOffset();
            protected:
        };
    }
}
