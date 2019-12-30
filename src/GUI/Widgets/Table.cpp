#include "GUI/Widgets/Table.h"

#include "Rendering/Renderer.h"
#include "Rendering/Font.h"
#include "Rendering/Display.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Table::Table(uint32_t rows, uint32_t columns) :
    EventConsumer<Mouse>()
{
    Create(rows, columns);
}

void Table::Create(uint32_t rows, uint32_t columns)
{
    SetNumRows(rows);
    SetNumCols(columns);
}

void Table::Destroy()
{
    cols.clear();
}

void Table::Render(int x, int y, int w)
{
    if(cols.empty()) return;

    Column &col = cols[0];

    int mx = GetMouseX();
    int my = GetMouseY();

    int rx = ToCoordX(GetTargetX()) + x;
    int ry = ToCoordY(GetTargetY()) + y + (renderNames ? 20 : 0);

    if(mx > rx && mx < rx + w && my > ry && my < ry+20*numRows) {
        highlight = (my - ry) / 20;
    }else highlight = -1;

    if(renderNames) {
        RenderRect(Rect<int>(x, y, w, 20), 104, 120, 88, 152, true);
        RenderRect(Rect<int>(x, y, w, 20), 239, 239, 189, 255, false);
        RenderText(col.name, x + 10, y + 2, FONT_SMALL);
        y += 20;
    }

    for(int i = 0; i < col.rows.size(); i++) {
        std::wstring &row = col.rows[i];

        if(i == highlight || i == selected) {
            RenderRect(Rect<int>(x, y+20*i, w, 20), 144, 160, 136, 152, true);
            RenderRect(Rect<int>(x, y+20*i, w, 20), 239, 239, 189, 255, false);
        }else {
            Uint8 r, g, b;
            if(i % 2 == 0) {
                r = 104;
                g = 120;
                b = 88;
            }else {
                r = 24;
                g = 80;
                b = 24;
            }
            RenderRect(Rect<int>(x, y+20*i, w, 20), r, g, b, 152, true);
        }

        RenderText(row, Rect<int>(x + 10, 2+y+20*i, w, 20), Align::LEFT, FONT_LARGE, false);
    }

    RenderRect(Rect<int>(x, y, w, 20*numRows), 239, 239, 189, 255, false);
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
    if(n > cols.size()) return;

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
    if(!cols.size() || highlight > numRows) return;
    if(!cols[0].rows[highlight].empty() && mouse.type == BUTTONDOWN) {
        selected = highlight;
    }
}
