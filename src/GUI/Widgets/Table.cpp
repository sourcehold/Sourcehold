#include "GUI/Widgets/Table.h"

#include "Rendering/Renderer.h"
#include "Rendering/Font.h"
#include "Rendering/Display.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Table::Table(uint32_t rows, uint32_t columns) :
    EventConsumer<Mouse>(), Widget()
{
    Create(rows, columns);
}

void Table::Create(uint32_t rows, uint32_t columns)
{
    SetNumCols(columns);
    SetNumRows(rows);
}

void Table::Destroy()
{
    cols.clear();
}

void Table::Update(Rect<int> constraints)
{
    if(cols.empty()) return;

    auto &col = cols[0];

    auto mx = GetMouseX();
    auto my = GetMouseY();

    auto width   = constraints.w;
    auto height  = 20 * static_cast<int>(numRows);

    auto x = constraints.x + ((constraints.w -  width) / 2);
    auto y = constraints.y + ((constraints.h - height) / 2);

    auto target = GetTarget();
    auto rx = target.x + x;
    auto ry = target.y + y + (renderNames ? 20 : 0);

    if(mx > rx && mx < rx + width && my > ry && my < ry+20*static_cast<int>(numRows)) {
        highlight = (my - ry) / 20;
    }else highlight = -1;

    if(renderNames) {
        RenderRect(Rect<int>(x, y, width, 20), 104, 120, 88, 152, true);
        RenderRect(Rect<int>(x, y, width, 20), 239, 239, 189, 255, false);
        RenderText(col.name, static_cast<uint32_t>(x) + 10, static_cast<uint32_t>(y) + 2, FONT_SMALL);
        y += 20;
    }

    for(size_t i = 0; i < col.rows.size(); i++) {
        auto &row = col.rows[i];

        if(static_cast<long>(i) == highlight || static_cast<long>(i) == selected) {
            RenderRect(Rect<int>(x, y+20*static_cast<int>(i), width, 20), 144, 160, 136, 152, true);
            RenderRect(Rect<int>(x, y+20*static_cast<int>(i), width, 20), 239, 239, 189, 255, false);
        }else {
            Uint8 r, g, b;
            // Slightly lighter color for even rows //
            if(i % 2 == 0) {
                r = 104;
                g = 120;
                b = 88;
            }else {
                r = 24;
                g = 80;
                b = 24;
            }
            RenderRect(Rect<int>(x, y+20*static_cast<int>(i), width, 20), r, g, b, 152, true);
        }

        // Render row contents //
        RenderText(row,
            Rect<uint32_t>(static_cast<uint32_t>(x) + 10, 2+static_cast<uint32_t>(y)+20*static_cast<uint32_t>(i), static_cast<uint32_t>(width), 20),
            Align::LEFT, FONT_LARGE, false);
    }

    // Render border around table //
    RenderRect(Rect<int>(x, y, width, 20*static_cast<int>(numRows)), 239, 239, 189, 255, false);
}

void Table::SetNumRows(uint32_t n)
{
    for (auto& r : cols) {
        r.rows.resize(n);
    }
    numRows = n;
}

void Table::SetNumCols(uint32_t n)
{
    cols.resize(n);
}

void Table::SetColName(uint32_t n, const std::wstring& name)
{
    if(n >= cols.size()) return;
    cols[n].name = name;
}

void Table::SetText(uint32_t row, uint32_t col, const std::wstring& text)
{
    try {
        Column &c = cols.at(col);
        c.rows.at(row) = text;
    } catch(std::out_of_range &ex) {
        return;
    }
}

void Table::onEventReceive(Mouse &mouse)
{
    if(cols.empty() || highlight > static_cast<int>(numRows) || cols[0].rows.empty()) return;
    if(mouse.type == BUTTONDOWN) {
        selected = highlight;
    }
}
