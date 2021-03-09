#include "Parsers/VolumeTxt.h"

using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

VolumeTxt::VolumeTxt() : Parser()
{

}

VolumeTxt::~VolumeTxt()
{

}

bool VolumeTxt::LoadFromDisk(ghc::filesystem::path path)
{
    if(!Parser::Open(path.string(), std::ifstream::in | std::ios::binary)) {
        Logger::error(PARSERS)  << "Unable to open volume file '" << path.string() << "'!" << std::endl;
        return false;
    }

    while(Parser::Ok()) {
        std::string line = Parser::GetLine();
        ParseLine(line);
    }

    Parser::Close();
    return true;
}

void VolumeTxt::Clear()
{
    volumes.clear();
}

uint8_t VolumeTxt::GetVolumeOf(std::string key)
{
    auto iter = volumes.find(key);
    if(iter == volumes.end()) {
        return 100;
    }
    return (*iter).second;
}

void VolumeTxt::ParseLine(std::string &line)
{
    std::regex regex("\"([^\"]*)\"\\s*([0-9]+)\\s");
    std::smatch match;

    const std::string str = line;
    if(std::regex_search(str.begin(), str.end(), match, regex)) {
        std::string f = match[1];
        uint8_t v = static_cast<uint8_t>(std::stoi(match[2]));

        volumes.insert(std::pair<std::string, uint8_t>(f, v));
    }
}
