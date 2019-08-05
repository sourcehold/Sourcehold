#pragma once

#include <fstream>
#include <vector>

#include <Assets.h>
#include <System/Logger.h>
#include <Parsers/Parser.h>

#include <boost/filesystem.hpp>

namespace Sourcehold {
    namespace Parsers {
        /*
         * The file containing all of the localized
         * mission descriptions.
         */
        class MlbFile : private Parser {
            /* Encoded in UTF-16LE */
            std::vector<std::wstring> field;
            uint16_t num = 0;
        public:
            MlbFile();
            ~MlbFile();

            bool LoadFromDisk(boost::filesystem::path path);
            void Clear();

            inline std::wstring& GetString(Assets::MissionDescription index) {
                return field.at(index);
            }
        protected:
            struct SectionHeader;
        };
    }
}
