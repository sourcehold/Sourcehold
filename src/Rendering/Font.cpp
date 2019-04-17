#include <utility>

#include <Rendering/Font.h>
#include <GameManager.h>
#include <Parsers/Gm1File.h>

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;

static std::shared_ptr<GameManager> _mgr;
static std::shared_ptr<Gm1File> _fonts[3];
static std::pair<uint8_t, uint8_t> _table_width_height[3] = {
    { 15, 18 }, /* Slanted */
    {  7, 17 }, /* Small */
    { 19, 32 }, /* Large */
};

bool Rendering::LoadFonts(std::shared_ptr<GameManager> mgr) {
    _mgr = mgr;

    _fonts[0] = mgr->GetGm1(mgr->GetDirectory() / "gm/font_slanted.gm1").lock();
    _fonts[1] = mgr->GetGm1(mgr->GetDirectory() / "gm/font_stronghold.gm1").lock();
    _fonts[2] = mgr->GetGm1(mgr->GetDirectory() / "gm/font_stronghold_aa.gm1").lock();

    return true;
}

void Rendering::UnloadFonts() {
    _mgr.reset();
    _fonts[0].reset();
    _fonts[1].reset();
    _fonts[2].reset();
}

void Rendering::RenderText(const std::wstring& text, int32_t x, int32_t y, double scaleFactor, Font type) {
    auto font = _fonts[type];

    SDL_Rect glyph;
    for(wchar_t c : text) {
        if(c < 0x20 || (c-0x20) > font->GetTextureAtlas().lock()->GetNumTextures()) continue;
        /* Space */
        if(c == 0x20) {
            x += _table_width_height[type].first;
        }else {
            glyph = font->GetTextureAtlas().lock()->Get(c - 0x21);
            _mgr->Render(*font->GetTextureAtlas().lock(),
                         x,
                         y + _table_width_height[type].second - glyph.h,
                         glyph.w,
                         glyph.h,
                         &glyph);

            x += glyph.w;
        }
    }
}

void Rendering::RenderText(const std::wstring& text, double x, double y, double scaleFactor, Font type) {
    auto font = _fonts[type];

    SDL_Rect glyph;
    for(wchar_t c : text) {
        if(c < 0x20 || (c-0x20) > font->GetTextureAtlas().lock()->GetNumTextures()) continue;
        /* Space */
        if(c == 0x20) {
            x += _table_width_height[type].first;
        }else {
            glyph = font->GetTextureAtlas().lock()->Get(c - 0x21);
            _mgr->Render(*font->GetTextureAtlas().lock(),
                         x,
                         double(_mgr->NormalizeY(_mgr->ToCoordY(y) + _table_width_height[type].second - glyph.h)),
                         double(_mgr->NormalizeX(glyph.w)),
                         double(_mgr->NormalizeX(glyph.h)),
                         &glyph);

            x += glyph.w;
        }
    }
}
