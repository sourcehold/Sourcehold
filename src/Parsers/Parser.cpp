#include <Parsers/Parser.h>

using namespace Sourcehold::Parsers;

Parser::Parser() : std::ifstream() {

}

bool Parser::Open(const std::string &path, std::ios_base::openmode mode) {
    open(path, mode);
    if(!is_open()) return false;

    std::streampos fsize = tellg();
    seekg(0, std::ios::end);
    fsize = tellg() - fsize;
    seekg(0, std::ios::beg);
    length = (uint32_t)fsize;

    return true;
}

void Parser::Close() {
    close();
}

bool Parser::Ok() {
    return !(rdstate() & std::ifstream::failbit);
}

bool Parser::GetData(void *buf, size_t bufsize) {
    if(rdstate() & std::ifstream::failbit) return false;
    this->read((char*)buf, bufsize);
    return true;
}

bool Parser::GetWhole(void *buf) {
    if(rdstate() & std::ifstream::failbit) return false;
    this->read((char*)buf, length);
    return true;
}

void Parser::Seek(uint32_t pos) {
    seekg(pos, std::ios_base::beg);
}

uint32_t Parser::Tell() {
    return tellg();
}

std::string Parser::GetUTF16() {
    std::wstring ws;

    while(Ok()) {
        uint16_t word = GetWord();

        uint8_t lo = word & 0xFF;
        uint8_t hi = word >> 8;

        int byte = hi << 8 | lo;
        if(byte == 0) break;

        ws.push_back(byte);
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.to_bytes(ws);
}

std::string Parser::GetLine() {
    std::string l;
    std::getline(*this, l);
    return l;
}

uint8_t Parser::GetByte() {
    return get();
}

uint16_t Parser::GetWord() {
    uint16_t w;
    read(reinterpret_cast<char *>(&w), sizeof(w));
    return w;
}

uint32_t Parser::GetDWord() {
    uint32_t w;
    read(reinterpret_cast<char *>(&w), sizeof(w));
    return w;
}

uint32_t Parser::GetOffset() {
    return tellg();
}
