#include <GUI/Table.h>

#include <Rendering/Renderer.h>
#include <Rendering/Font.h>
#include <Rendering/Display.h>

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
    rows.clear();
}

void Table::Render(int x, int y, int w)
{
    if(rows.empty()) return;

    // TODO: multiple rows
    Row &row = rows[0];

    int mx = GetMouseX();
    int my = GetMouseY();

    int rx = ToCoordX(GetTargetX()) + x;
    int ry = ToCoordY(GetTargetY()) + y + (renderNames ? 20 : 0);

    if(mx > rx && mx < rx + w && my > ry && my < ry+20*numCols) {
        highlight = (my - ry) / 20;
    }else highlight = -1;

    if(renderNames) {
        RenderRect(Rect<int>(x, y, w, 20), 104, 120, 88, 152, true);
        RenderRect(Rect<int>(x, y, w, 20), 239, 239, 189, 255, false);
        RenderText(row.name, x + 10, y + 2, FONT_SMALL);
        y += 20;
    }

    for(int i = 0; i < row.cols.size(); i++) {
        std::wstring &col = row.cols[i];

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

        RenderText(col, x + 10, 2+y+20*i, FONT_SMALL);
    }

    RenderRect(Rect<int>(x, y, w, 20*numCols), 239, 239, 189, 255, false);
}

void Table::SetNumRows(uint32_t n)
{
    rows.resize(n);
}

void Table::SetNumCols(uint32_t n)
{
    for(auto & r : rows) {
        r.cols.resize(n);
    }
    numCols = n;
}

void Table::SetRowName(uint32_t n, const std::wstring& name)
{
    if(n > rows.size()) return;

    rows[n].name = name;
}

void Table::SetText(uint32_t row, uint32_t col, const std::wstring& text)
{
    try {
        Row &r = rows.at(row);
        r.cols.at(col) = text;
    } catch(std::out_of_range &ex) {
        return;
    }
}

void Table::onEventReceive(Mouse &mouse)
{
    if(!rows.size() || highlight > numCols) return;
    if(!rows[0].cols[highlight].empty() && mouse.type == MOUSE_BUTTONDOWN) {
        selected = highlight;
    }
}
