#include <Parsers/Parser.h>

using namespace Sourcehold::Parsers;

Parser::Parser() : std::fstream()
{

}

bool Parser::Open(const std::string &path, std::ios_base::openmode mode)
{
    open(path, mode);
    if(!is_open()) return false;

    if(mode & std::ifstream::in) {
        std::streampos fsize = tellg();
        seekg(0, std::ios::end);
        fsize = tellg() - fsize;
        seekg(0, std::ios::beg);
        length = (uint32_t)fsize;
    }

    return true;
}

void Parser::Close()
{
    close();
}

bool Parser::Ok()
{
    return !(rdstate() & std::fstream::failbit);
}

void Parser::SeekG(uint32_t pos)
{
    seekg(pos, std::ios_base::beg);
}

void Parser::SeekP(uint32_t pos)
{
    seekp(pos, std::ios_base::beg);
}

uint32_t Parser::Tell()
{
    return tellg();
}

bool Parser::GetData(void *buf, size_t bufsize)
{
    if(rdstate() & std::fstream::failbit) return false;
    this->read((char*)buf, bufsize);
    return true;
}

bool Parser::GetWhole(void *buf)
{
    if(rdstate() & std::fstream::failbit) return false;
    this->read((char*)buf, length);
    return true;
}

std::wstring Parser::GetUTF16()
{
    std::wstring ws;

    while(Ok()) {
        uint16_t word = GetWord();

        uint8_t lo = word & 0xFF;
        uint8_t hi = word >> 8;

        int byte = hi << 8 | lo;
        if(byte == 0) break;

        ws.push_back(byte);
    }

    return ws;
}

std::string Parser::GetLine()
{
    std::string l;
    std::getline(*this, l);
    return l;
}

uint8_t Parser::GetByte()
{
    return get();
}

uint16_t Parser::GetWord()
{
    uint16_t w;
    read(reinterpret_cast<char *>(&w), sizeof(w));
    return w;
}

uint32_t Parser::GetDWord()
{
    uint32_t w;
    read(reinterpret_cast<char *>(&w), sizeof(w));
    return w;
}

void Parser::WriteData(void *buf, size_t bufsize)
{
    write((const char*)buf, bufsize);
}

void Parser::WriteBytes(uint8_t byte, size_t num)
{
    for(; num; num--) {
        WriteByte(byte);
    }
}

void Parser::WriteUTF16(std::wstring str)
{
    write((const char*)str.c_str(), str.length() * sizeof(wchar_t));
}

void Parser::WriteByte(uint8_t byte)
{
    write((const char*)&byte, 1);
}

void Parser::WriteWord(uint16_t word)
{
    write((const char*)&word, 2);
}

void Parser::WriteDWord(uint32_t dword)
{
    write((const char*)&dword, 4);
}

