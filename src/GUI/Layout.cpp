#include <algorithm>

#include "GameManager.h"

#include "GUI/Layout.h"

#include "Rendering/Renderer.h"
#include "Rendering/Display.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::Game;

#undef min // ...
#undef max

Layout::Layout()
{
}

Layout::Layout(HlpSection *hlp, Rect<double> bounds)
{
    SetBounds(bounds);
    CreateFromHlp(hlp);
}

Layout::~Layout()
{
    Destroy();
}

void Layout::CreateFromHlp([[maybe_unused]] HlpSection *hlp)
{
    /*
    for(int i = 0; i < hlp->children.size(); i++) {
        HlpSection &sect = hlp->children[i];

        switch(sect.type) {
        case SectionType::PIC : {
            int arg0 = sect.GetIntArg(0);
            std::wstring arg1 = sect.GetStrArg(1);

            PicPosition pos = PicPosition::LEFT;
            if(arg1 == L"LEFT") {
                pos = PicPosition::LEFT;
            }else if(arg1 == L"RIGHT") {
                pos = PicPosition::RIGHT;
            }else if(arg1 == L"CENTRE") {
                pos = PicPosition::CENTRE;
            }

            if(arg0 < pics.size()) {
                AddPic(pics[arg0], pos);
            }
        } break;
        case SectionType::FONT : {
            int arg0 = sect.GetIntArg(0);
            SetFont((Font)arg0);
        } break;
        case SectionType::COLOUR : {
        } break;
        case SectionType::LOADPIC : {
            if(sect.name.empty()) continue;

            auto pic = GetTgx(ghc::filesystem::path("gfx/") / ghc::filesystem::path(sect.name));
            pics.push_back(pic);
        } break;
        case SectionType::SECTION : {
            pics.clear();
        } break;
        case SectionType::HEADER : {
        } break;
        case SectionType::BODY : {
        } break;
        case SectionType::INCLUDE : {
        } break;
        case SectionType::NEWPARAGRAPH : {
        } break;
        case SectionType::LINK : {
        } break;
        case SectionType::CENTRE : {
        } break;
        default: break;
        }
    }*/
}

void Layout::Destroy()
{
    elements_.clear();
}

void Layout::Render([[maybe_unused]] double x, [[maybe_unused]] double y)
{
}

void Layout::Render(int x, int y)
{
    int winH = GetHeight();

    for (auto &elem : elements_)
    {
        if(y + elem.y > winH) return;

        if(elem.type == LayoutElement::LINE) {
            RenderText(elem.text,  static_cast<uint32_t>(elem.x + x), static_cast<uint32_t>(elem.y + y), elem.font);
        }else if(elem.type == LayoutElement::PIC) {
            ::Render(*elem.pic, elem.x + x, elem.y + y);
        }else if(elem.type == LayoutElement::PARAGRAPH) {
        }else if(elem.type == LayoutElement::LINEBREAK) {
        }
    }
}

void Layout::SetFont(Font index)
{
    current_font_ = index;
}

void Layout::AddText(const std::wstring &str)
{
    auto dim = GetStringPixelDim(str, current_font_);

    LayoutElement elem;
    elem.type = LayoutElement::LINE;
    elem.text = str;
    elem.font = current_font_;
    elem.x = 0;
    elem.y = static_cast<int>(height_);
    elements_.push_back(elem);

    width_ = std::max(width_, dim.first);
    height_ += dim.second;
}

void Layout::AddPic(std::shared_ptr<TgxFile> pic, PicPosition pos)
{
    LayoutElement elem;
    elem.type = LayoutElement::PIC;
    elem.pic = pic;
    elem.pos = pos;
    elem.y = static_cast<int>(height_);
    if(pos == PicPosition::LEFT) {
        elem.x = 0;
    }else if(pos == PicPosition::RIGHT) {
        elem.x = static_cast<int>(width_) - pic->GetWidth();
    }else if(pos == PicPosition::CENTRE) {
        elem.x = (static_cast<int>(width_) - pic->GetWidth()) / 2;
    }
    elements_.push_back(elem);

    width_ = std::max(width_, static_cast<uint32_t>(pic->GetWidth()));
    height_ += static_cast<uint32_t>(pic->GetHeight());
}

void Layout::NewParagraph()
{
    LayoutElement elem;
    elem.type = LayoutElement::PARAGRAPH;
    elements_.push_back(elem);
}
