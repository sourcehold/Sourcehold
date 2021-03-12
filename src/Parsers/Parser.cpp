#include "Parsers/Parser.h"

using namespace Sourcehold::Parsers;

Parser::Parser()
{
}

bool Parser::Open(const std::string &path, std::ios_base::openmode mode)
{
    stream.open(path, mode);
    if(!stream.is_open()) return false;

    if(mode & std::ifstream::in) {
        std::streampos fsize = stream.tellg();
        stream.seekg(0, std::ios::end);
        fsize = stream.tellg() - fsize;
        stream.seekg(0, std::ios::beg);
        length = (uint32_t)fsize;
    }

    return true;
}

void Parser::Close()
{
    stream.close();
}

bool Parser::Ok()
{
    return !(stream.rdstate() & std::fstream::failbit);
}

void Parser::Skip(uint32_t n)
{
    stream.seekg(n, std::ios_base::cur);
}

void Parser::SeekG(uint32_t pos)
{
    stream.seekg(pos, std::ios_base::beg);
}

void Parser::SeekP(uint32_t pos)
{
    stream.seekp(pos, std::ios_base::beg);
}

uint32_t Parser::Tell()
{
    return (uint32_t)stream.tellg();
}

bool Parser::GetData(void *buf, size_t bufsize)
{
    if(stream.rdstate() & std::fstream::failbit) return false;
    stream.read((char*)buf, bufsize);
    return true;
}

bool Parser::GetWhole(void *buf)
{
    if(stream.rdstate() & std::fstream::failbit) return false;
    stream.read((char*)buf, length);
    return true;
}

std::wstring Parser::GetUTF16(uint32_t len)
{
    std::wstring ws;

    while(Ok() && ws.length() < len) {
        uint16_t word = GetWord();

        uint8_t lo = word & 0xFF;
        uint8_t hi = word >> 8;

        int byte = hi << 8 | lo;

        ws.push_back(byte);
    }

    return ws;
}

std::wstring Parser::GetUTF16()
{
    std::wstring ws;

    while(Ok()) {
        uint16_t word = GetWord();

        uint8_t lo = word & 0xFF;
        uint8_t hi = word >> 8;

        int byte = hi << 8 | lo;
        if(!byte) break;

        ws.push_back(byte);
    }

    return ws;
}

std::string Parser::GetLine()
{
    std::string l;
    std::getline(stream, l);
    return l;
}

uint8_t Parser::GetByte()
{
    return stream.get();
}

uint16_t Parser::GetWord()
{
    uint16_t w;
    stream.read(reinterpret_cast<char *>(&w), sizeof(w));
    return w;
}

uint32_t Parser::GetDWord()
{
    uint32_t w;
    stream.read(reinterpret_cast<char *>(&w), sizeof(w));
    return w;
}

void Parser::WriteData(void *buf, size_t bufsize)
{
    stream.write((const char*)buf, bufsize);
}

void Parser::WriteBytes(uint8_t byte, size_t num)
{
    for(; num; num--) {
        WriteByte(byte);
    }
}

void Parser::WriteUTF16(std::wstring str)
{
    for(wchar_t c : str) {
        stream.write((const char*)&c, 2);
    }
}

void Parser::WriteByte(uint8_t byte)
{
    stream.write((const char*)&byte, 1);
}

void Parser::WriteWord(uint16_t word)
{
    stream.write((const char*)&word, 2);
}

void Parser::WriteDWord(uint32_t dword)
{
    stream.write((const char*)&dword, 4);
}

