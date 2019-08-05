#include <utility>

#include <Rendering/Font.h>
#include <GameManager.h>
#include <Parsers/Gm1File.h>

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;

static std::shared_ptr<Gm1File> _fonts[3];
static std::pair<uint8_t, uint8_t> _table_width_height[3] = {
    { 15, 18 }, /* Slanted */
    {  7, 17 }, /* Small */
    { 19, 32 }, /* Large */
};

bool Rendering::LoadFonts()
{
    _fonts[0] = GetGm1(GetDirectory() / "gm/font_slanted.gm1");
    _fonts[1] = GetGm1(GetDirectory() / "gm/font_stronghold.gm1");
    _fonts[2] = GetGm1(GetDirectory() / "gm/font_stronghold_aa.gm1");

    return true;
}

void Rendering::UnloadFonts()
{
    _fonts[0].reset();
    _fonts[1].reset();
    _fonts[2].reset();
}

void Rendering::RenderText(const std::wstring& text, int32_t x, int32_t y, Font type, bool illumination, double scaleFactor)
{
    auto font = _fonts[type];
    if (illumination && text.size() != 1) {
        return;
    }

    font->GetTextureAtlas()->SetColorMod(248, 240, 184);

    SDL_Rect glyph = {};
    for(wchar_t c : text) {
        if(c < 0x20) continue;
        /* Space */
        if(c == 0x20) {
            x += _table_width_height[type].first*scaleFactor;
        }
        else {
            /* Calculate lowercase offset */
            int8_t lowercaseOffset = 0;
            switch(c) {
            case 0x67: /* g */
            case 0x70: /* p */
            case 0x71: /* q */
            case 0x79: /* y */
            case 0x51: /* Q */
            case 0x4A: { /* J */
                lowercaseOffset = _table_width_height[type].second / 2 - (type == FONT_SMALL ? 1 : 5);
            }
            break;
            case 0x6A: { /* j */
                lowercaseOffset = _table_width_height[type].second / 2;
            }
            break;
            case 0x2D: { /* - */
                lowercaseOffset = FONT_SMALL ? -8 : -12;
            }
            break;
            case 0x27: { /* ' */
                lowercaseOffset = FONT_SMALL ? -12 : -17;
            }
            break;
            default:
                break;
            }
            glyph = font->GetTextureAtlas()->Get(c - 0x21);
            Render(*font->GetTextureAtlas(),
                   illumination ? (x + (13 - glyph.w)/2) : x,
                   y + int(_table_width_height[type].second*scaleFactor)- int(glyph.h*scaleFactor) + int(lowercaseOffset*scaleFactor),
                   int(glyph.w*scaleFactor),
                   int(glyph.h*scaleFactor),
                   &glyph);
            x += glyph.w*scaleFactor+1;
        }
    }
}

void Rendering::RenderText(const std::wstring& text, Rect<int> bounds, Align al, Font type, bool illumination)
{
    auto dim = GetStringPixelDim(text, type);

    double factor = (double(bounds.h) / dim.second) * 0.75;

    dim.first *= factor;
    dim.second *= factor;

    int x=0, y=0;
    if(al == Align::CENTER) {
        x = bounds.x + bounds.w/2 - dim.first/2;
    } else if(al == Align::LEFT) {
        x = bounds.x + 4;
    } else if(al == Align::RIGHT) {
        x = bounds.x + bounds.w - dim.first - 4;
    }

    y = bounds.y + bounds.h/2 - dim.second/2;

    RenderText(text, x, y, type, illumination, factor);
}

std::pair<uint32_t, uint32_t> Rendering::GetStringPixelDim(const std::wstring& text, Font type)
{
    std::pair<uint32_t, uint32_t> dim = {
        _table_width_height[type].first,
        _table_width_height[type].second + 6 // todo
    };

    auto font = _fonts[type];

    SDL_Rect glyph = {};
    for(wchar_t c : text) {
        if(c < 0x20) continue;
        /* Space */
        if(c == 0x20) {
            dim.first += _table_width_height[type].first;
        }
        else {
            /* Calculate lowercase offset */
            int8_t lowercaseOffset = 0;
            switch(c) {
            case 0x67: /* g */
            case 0x70: /* p */
            case 0x71: /* q */
            case 0x79: /* y */
            case 0x51: /* Q */
            case 0x4A: { /* J */
                lowercaseOffset = _table_width_height[type].second / 2 - (type == FONT_SMALL ? 1 : 5);
            }
            break;
            case 0x6A: { /* j */
                lowercaseOffset = _table_width_height[type].second / 2;
            }
            break;
            case 0x2D: { /* - */
                lowercaseOffset = FONT_SMALL ? -8 : -12;
            }
            break;
            case 0x27: { /* ' */
                lowercaseOffset = FONT_SMALL ? -12 : -17;
            }
            break;
            default:
                break;
            }
            glyph = font->GetTextureAtlas()->Get(c - 0x21);

            dim.first += glyph.w +1;
        }
    }

    return dim;
}
