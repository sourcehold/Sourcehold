#include <Parser.h>

using namespace OpenSH::Parsers;
using namespace boost::filesystem;

bool Parser::Open(boost::filesystem::path &path, std::ios_base::openmode mode) {
    open(path.native(), mode);
    if(!is_open()) return false;

    return true;
}

void Parser::Close() {
    close();
}

bool Parser::GetData(void *buf, size_t bufsize) {
    if(!is_open()) return false;
    get((char*)buf, bufsize);
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
