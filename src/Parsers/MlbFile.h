#pragma once

#include <fstream>
#include <vector>

#include <Config.h>
#include <System/Logger.h>
#include <Parsers/Parser.h>

/* File containing scenario description */
namespace Sourcehold
{
    namespace Parsers
    {
        class MlbFile : public Parser
        {
                /* Encoded in UTF-16LE */
                std::vector<std::u16string> field;
            public:
                MlbFile();
                ~MlbFile();

                bool LoadFromDisk(const std::string &path);
                void Clear();
            protected:
        };
    }
}
