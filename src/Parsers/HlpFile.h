#pragma once

#include <string>
#include <vector>
#include <utility>

namespace Sourcehold {
	namespace Parsers {
      // A single section or command
      struct HlpSection {
      public:
          // the name of the section or command
          std::wstring name;
          enum SectionType : uint8_t {
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
          } type;
          std::vector<HlpSection> children;
          std::vector<std::wstring> args;

          int GetIntArg(size_t index);
          std::wstring GetStrArg(size_t index);
      };

      bool LoadStrongholdHlp();
      void UnloadStrongholdHlp();
	}
}
