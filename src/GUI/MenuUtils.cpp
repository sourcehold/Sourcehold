#include <GameManager.h>

#include <GUI/MenuUtils.h>
#include <GUI/StaticElement.h>

#include <Rendering/Renderer.h>
#include <Rendering/Font.h>

#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;

static std::shared_ptr<Gm1File> _gm_interface_icons3;
static std::shared_ptr<TgxFile> _tgx_border;
static SDL_Rect _border_rect;
static StrongholdEdition _ed;

bool GUI::InitializeUtils()
{
    _gm_interface_icons3 = GetGm1(GetDirectory() / "gm/interface_icons3.gm1").lock();
    _ed = GetEdition();
    if(_ed == STRONGHOLD_HD) {
    	_tgx_border = GetTgx(GetDirectory() / "gfx/SH1_Back.tgx").lock();
		/**
	     * Render the border 'zoomed in' so that the
	     * menu can be placed in the middle without scaling.
	     */
	    _border_rect.x = (1920 - GetWidth()) / 2;
	    _border_rect.y = (1200 - GetHeight()) / 2;
	    _border_rect.w = GetWidth();
	    _border_rect.h = GetHeight();
    }

    return true;
}

void GUI::RenderMenuText(const std::wstring &text)
{
    std::shared_ptr<TextureAtlas> interface_icons = _gm_interface_icons3->GetTextureAtlas().lock();
	auto rect = interface_icons->Get(18);
    //Render(*interface_icons, 0.305, 0.52, &rect);
    RenderText(text.substr(0,1), 0.3095703125, 0.528, 0.5, FONT_SMALL, true);
    RenderText(text.substr(1,text.size()), 0.330078125, 0.528, 0.5, FONT_SMALL);
}

void GUI::RenderMenuBorder() {
	if(_ed == STRONGHOLD_HD) {
	    Render(*_tgx_border, &_border_rect);
	}
}
