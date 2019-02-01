#include <Parser.h>

using namespace OpenSH::Parsers;
using namespace boost::filesystem;

bool Parser::Open(boost::filesystem::path &path, std::ios_base::openmode mode) {
    ifstream::open(path, mode);
    if(!is_open()) return false;

    return true;
}

void Parser::Close() {
    ifstream::close();
}

bool Parser::GetData(void *buf, size_t bufsize) {
    if(!is_open()) return false;
    ifstream::get((char*)buf, bufsize);
    return true;
}

std::u16string Parser::GetUTF16() {
    std::u16string str;

    return str;
}
