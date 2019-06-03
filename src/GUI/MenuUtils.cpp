#include <GUI/MenuUtils.h>
#include <Rendering/Font.h>
#include <Parsers/Gm1File.h>

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;

static std::shared_ptr<GameManager> _mgr;
static std::shared_ptr<Gm1File> _gm_interface_icons3;

bool GUI::InitializeUtils(std::shared_ptr<GameManager> mgr) {
    _mgr = mgr;
    _gm_interface_icons3 = _mgr->GetGm1(_mgr->GetDirectory() / "gm/interface_icons3.gm1").lock();
    return true;
}

void GUI::RenderMenuText(const std::wstring &text)
{
    std::shared_ptr<TextureAtlas> interface_icons = _gm_interface_icons3->GetTextureAtlas().lock();
    interface_icons->SetRect(interface_icons->Get(18));
    auto rect = interface_icons->Get(18);
    _mgr->Render(*interface_icons, 0.305, 0.52, &rect);
    RenderText(text.substr(0,1), 0.3095703125, 0.528, 0.5, FONT_SMALL, true);
    RenderText(text.substr(1,text.size()), 0.330078125, 0.528, 0.5, FONT_SMALL);
}
