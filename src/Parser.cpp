#include <Parser.h>

using namespace OpenSH::Parsers;
using namespace boost::filesystem;

bool Parser::Open(boost::filesystem::path &path, std::ios_base::openmode mode) {
    open(path.native(), mode);
    if(!is_open()) return false;

    std::streampos fsize = 0;
    fsize = tellg();
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
    this->get((char*)buf, bufsize);
    return true;
}

std::string Parser::GetUTF16() {
    std::wstring ws;

    std::stringstream ss;
    ss << rdbuf();
    std::string bytes = ss.str();

    size_t len = bytes.size();
    if(len % 2 != 0) len--;

    for(size_t i = 0; i < len;) {
        int lo = bytes[i++] & 0xFF;
        int hi = bytes[i++] & 0xFF;
        ws.push_back(hi << 8| lo);
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.to_bytes(ws);
}

uint8_t Parser::GetByte() {
    return get();
}

uint16_t Parser::GetWord() {
    uint16_t w;
    get((char*)&w, sizeof(uint16_t));
    return w;
}

uint32_t Parser::GetLength() {
    return length;
}
