#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include <Parsers/Gm1File.h>

namespace Sourcehold {
    namespace GUI {
        struct Row {
            std::wstring name;
            std::vector<std::wstring> cols;
        };

        class Table {
        public:
            Table() = default;
            Table(uint32_t rows, uint32_t columns);
            ~Table() = default;

            void Create(uint32_t rows, uint32_t columns);
            void Destroy();
            void Render(int x, int y);

            void SetNumRows(uint32_t n);
            void SetNumCols(uint32_t n);
            void SetRowName(uint32_t n, const std::wstring& name);
            void SetText(uint32_t row, uint32_t col, const std::wstring& text);

            inline void Scrollable(bool b) { scrollbar = true; }
            inline void RenderNames(bool b) { renderNames = b; }
        protected:
            std::vector<Row> rows;
            bool scrollbar = true;
            bool renderNames = true;
        };
    }
}
