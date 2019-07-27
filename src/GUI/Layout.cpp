#include <algorithm>

#include <GameManager.h>

#include <GUI/Layout.h>

#include <Rendering/Renderer.h>
#include <Rendering/Display.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::Game;

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

void Layout::CreateFromHlp(HlpSection *hlp)
{
    for(int i = 0; i < hlp->children.size(); i++) {
        HlpSection &sect = hlp->children[i];

        switch(sect.type) {
        case SectionType::PIC : {
            int arg0 = sect.GetIntArg(0);
            std::wstring arg1 = sect.GetStrArg(1);

            PicPosition pos;
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

            auto pic = GetTgx(GetDirectory() / boost::filesystem::path("gfx/") / boost::filesystem::path(sect.name));
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
    }
}

void Layout::Destroy()
{
    elems.clear();
}

void Layout::Render(double x, double y)
{
    Render(ToCoordX(x), ToCoordY(y));
}

void Layout::Render(int x, int y)
{
    int winH = GetHeight();

    for(int i = 0; i < elems.size(); i++) {
        LayoutElement &elem = elems[i];

        if(y + elem.y > winH) return;

        if(elem.type == LayoutElement::LINE) {
            RenderText(elem.text, elem.x + x, elem.y + y, elem.font);
        }else if(elem.type == LayoutElement::PIC) {
            ::Render(*elem.pic, elem.x + x, elem.y + y);
        }else if(elem.type == LayoutElement::PARAGRAPH) {
        }else if(elem.type == LayoutElement::LINEBREAK) {
        }
    }
}

void Layout::SetFont(Font index)
{
    currentFont = index;
}

void Layout::AddText(const std::wstring &str)
{
    auto dim = GetStringPixelDim(str, currentFont);
    double fw = NormalizeTargetX(dim.first);

    if(fw > bounds.w) {
        // TODO: insert a line break at nearest word
    }

    LayoutElement elem;
    elem.type = LayoutElement::LINE;
    elem.text = str;
    elem.font = currentFont;
    elem.x = 0;
    elem.y = height;
    elems.push_back(elem);

    width = std::max(width, dim.first);
    height += dim.second;
}

void Layout::AddPic(std::shared_ptr<TgxFile> pic, PicPosition pos)
{
    LayoutElement elem;
    elem.type = LayoutElement::PIC;
    elem.pic = pic;
    elem.pos = pos;
    elem.y = height;
    if(pos == PicPosition::LEFT) {
        elem.x = 0;
    }else if(pos == PicPosition::RIGHT) {
        elem.x = width - pic->GetWidth();
    }else if(pos == PicPosition::CENTRE) {
        elem.x = (width - pic->GetWidth()) / 2;
    }
    elems.push_back(elem);

    width = std::max((int)width, pic->GetWidth());
    height += pic->GetHeight();
}

void Layout::NewParagraph()
{
    LayoutElement elem;
    elem.type = LayoutElement::PARAGRAPH;
    elems.push_back(elem);
}
