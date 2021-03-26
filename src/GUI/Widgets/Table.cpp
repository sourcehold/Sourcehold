#include "GUI/Widgets/Table.h"

#include "Rendering/Renderer.h"
#include "Rendering/Font.h"
#include "Rendering/Display.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Table::Table(uint32_t rows, uint32_t columns)
    : EventConsumer<Mouse>(), Widget() {
  Create(rows, columns);
}

void Table::Create(uint32_t rows, uint32_t columns) {
  SetNumCols(columns);
  SetNumRows(rows);
}

void Table::Destroy() {
  cols.clear();
}

void Table::Update(Rect<int> constraints) {
  if (cols.empty())
    return;

  Column &col = cols[0];

  int mx = GetMouseX();
  int my = GetMouseY();

  int width = constraints.w;
  int height = 20 * numRows;

  int x = constraints.x + ((constraints.w - width) / 2);
  int y = constraints.y + ((constraints.h - height) / 2);

  auto target = Renderer::Instance().bounds_;
  int rx = target.x + x;
  int ry = target.y + y + (renderNames ? 20 : 0);

  if (mx > rx && mx < rx + width && my > ry && my < ry + 20 * numRows) {
    highlight = (my - ry) / 20;
  }
  else
    highlight = -1;

  if (renderNames) {
    Renderer::Instance().Render({x, y, width, 20}, {104, 120, 88, 152}, true);
    Renderer::Instance().Render({x, y, width, 20}, {239, 239, 189, 255}, false);
    RenderText(col.name, x + 10, y + 2, FONT_SMALL);
    y += 20;
  }

  for (int i = 0; i < col.rows.size(); i++) {
    std::wstring &row = col.rows[i];

    if (i == highlight || i == selected) {
      Renderer::Instance().Render({x, y + 20 * i, width, 20},
                                  {144, 160, 136, 152}, true);
      Renderer::Instance().Render({x, y + 20 * i, width, 20},
                                  {239, 239, 189, 255}, false);
    }
    else {
      Color color;
      // Slightly lighter color for even rows //
      if (i % 2 == 0) {
        color = {104, 120, 88, 152};
      }
      else {
        color = {24, 80, 24, 152};
      }
      Renderer::Instance().Render({x, y + 20 * i, width, 20}, color, true);
    }

    // Render row contents //
    RenderText(row, {x + 10, 2 + y + 20 * i, width, 20}, Align::LEFT,
               FONT_LARGE, false);
  }

  // Render border around table //
  Renderer::Instance().Render({x, y, width, 20 * static_cast<int>(numRows)},
                              {239, 239, 189, 255}, false);
}

void Table::SetNumRows(uint32_t n) {
  for (auto &r : cols) {
    r.rows.resize(n);
  }
  numRows = n;
}

void Table::SetNumCols(uint32_t n) {
  cols.resize(n);
}

void Table::SetColName(uint32_t n, const std::wstring &name) {
  if (n >= cols.size())
    return;
  cols[n].name = name;
}

void Table::SetText(uint32_t row, uint32_t col, const std::wstring &text) {
  try {
    Column &c = cols.at(col);
    c.rows.at(row) = text;
  }
  catch (std::out_of_range &ex) {
    return;
  }
}

void Table::onEventReceive(Mouse &mouse) {
  if (cols.empty() || highlight > numRows || cols[0].rows.empty())
    return;
  if (mouse.type == BUTTONDOWN) {
    selected = highlight;
  }
}
