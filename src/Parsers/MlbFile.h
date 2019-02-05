#pragma once

#include <fstream>
#include <vector>

#include <Config.h>
#include <System/Logger.h>
#include <Parsers/Parser.h>

/* File containing scenario description */
namespace OpenSH
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

                void LoadFromDisk(boost::filesystem::path path);
                void Clear();
            protected:
        };
    }
}
