#include <GUI/Table.h>
#include <Rendering/Renderer.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Table::Table(uint32_t rows, uint32_t columns)
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

void Table::Render(int x, int y)
{
}

void Table::SetNumRows(uint32_t n)
{
    rows.resize(n);
}

void Table::SetNumCols(uint32_t n)
{
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
