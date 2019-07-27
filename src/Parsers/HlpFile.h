#pragma once

#include <string>
#include <vector>
#include <utility>

namespace Sourcehold {
    namespace Parsers {
        enum class SectionType : uint8_t {
            PIC,
            FONT,
            COLOUR,
            SECTION,
            LOADPIC,
            HEADER,
            ENDHEADER,
            BODY,
            ENDBODY,
            INCLUDE,
            NEWPARAGRAPH,
            LINK,
            ENDLINK,
            CENTRE,
            ENDCENTRE
        };

        // A single section or command
        struct HlpSection {
        public:
            // the name of the section or command
            std::wstring name;
            SectionType type;
            std::vector<HlpSection> children;
            std::vector<std::wstring> args;

            int GetIntArg(size_t index);
            std::wstring GetStrArg(size_t index);
        };

        bool LoadStrongholdHlp();
        void UnloadStrongholdHlp();
        HlpSection *GetHlpSection(std::wstring name);
    }
}
