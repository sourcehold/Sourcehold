#pragma once

#include <string>
#include <boost/filesystem/fstream.hpp>
#include <cinttypes>

#include <Config.h>

/* Parser super class */
namespace OpenSH
{
    namespace Parsers
    {
        class Parser : public boost::filesystem::ifstream
        {
            public:
                Parser() : boost::filesystem::ifstream() { };
                ~Parser() = default;
            
                bool Open(boost::filesystem::path &path, std::ios_base::openmode mode);
                void Close();
                bool GetData(void *buf, size_t bufsize);
                std::u16string GetUTF16();
            protected:
        };
    }
}
