#pragma once

#include <string>
#include <cinttypes>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>

namespace Sourcehold
{
    namespace Parsers
    {

        /*
        * Parser super class, provide an interface for file IO
        * TODO: DO __NOT__ assume everyone is using little-endian
        */
        class Parser : protected std::fstream
        {
            uint32_t length = 0;
        public:
            Parser();
            virtual ~Parser() = default;

            bool Open(const std::string &path, std::ios_base::openmode mode);
            void Close();
            bool Ok();
            void SeekG(uint32_t pos);
            void SeekP(uint32_t pos);
            uint32_t Tell();

            bool GetData(void *buf, size_t bufsize);
            bool GetWhole(void *buf);
            std::wstring GetUTF16();
            std::string GetLine();
            uint8_t GetByte();
            uint16_t GetWord();
            uint32_t GetDWord();

            void WriteData(void *buf, size_t bufsize);
            void WriteBytes(uint8_t byte, size_t num);
            void WriteUTF16(std::wstring str);
            void WriteByte(uint8_t byte);
            void WriteWord(uint16_t word);
            void WriteDWord(uint32_t dword);

            inline uint32_t GetLength() { return length; }
        };
    }
}
