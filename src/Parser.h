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
namespace OpenSH
{
    namespace Parsers
    {
        class Parser : public std::ifstream
        {
            public:
                Parser() : std::ifstream() { }; 
                ~Parser() = default;
            
                bool Open(boost::filesystem::path &path, std::ios_base::openmode mode);
                void Close();
                bool GetData(void *buf, size_t bufsize);
                std::string GetUTF16();
            protected:
        };
    }
}
