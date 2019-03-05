#pragma once

#include <string>
#include <cinttypes>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>

#include <Config.h>

namespace Sourcehold
{
    namespace Parsers
    {

        /*
        * Parser super class, provide an interface for file IO
        * TODO: DO __NOT__ assume everyone is using little-endian
        */
        class Parser : protected std::ifstream
        {
                uint32_t length = 0;
            public:
                Parser(); 
                virtual ~Parser() = default;
            
                bool Open(const std::string &path, std::ios_base::openmode mode);
                void Close();
                bool Ok();
                bool GetData(void *buf, size_t bufsize);
                bool GetWhole(void *buf);
                void Seek(uint32_t pos);
                uint32_t Tell();

                std::string GetUTF16();
                std::string GetLine();
                uint8_t GetByte();
                uint16_t GetWord();
                uint32_t GetOffset();

                inline uint32_t GetLength() { return length; }
        };
    }
}
