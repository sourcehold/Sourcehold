#include <codecvt>

#include "GameManager.h"

#include "GUI/MenuUtils.h"
#include "GUI/StaticElement.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Rendering/Font.h"

#include "Parsers/Gm1File.h"
#include "Parsers/TgxFile.h"
#include "Parsers/CfgFile.h"

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;
using namespace GUI;

std::shared_ptr<TextureAtlas> interface_icons;
std::shared_ptr<TgxFile> tgx_border;

void GUI::InitMenuUtils()
{
    interface_icons = GetGm1("gm/interface_icons3.gm1")->GetTextureAtlas();
    tgx_border = GetTgx("gfx/SH1_Back.tgx");
}

void GUI::RenderMenuText(const std::wstring &text)
{
    if(text.empty()) return;

    auto rect = interface_icons->Get(18);
    Render(*interface_icons, 312, 400, &rect);
    RenderText(text.substr(0,1), 317, 406, FONT_SMALL, true);
    RenderText(text.substr(1,text.size()), 338, 406, FONT_SMALL);
}

void GUI::RenderMenuBorder()
{
    if(GetEdition() == STRONGHOLD_HD) {
        SDL_Rect border_rect;

        /**
         * Render the border 'zoomed in' so that theP
         * menu can be placed in the middle without scaling.
         */
        border_rect.x = (1920 - GetWidth()) / 2;
        border_rect.y = (1200 - GetHeight()) / 2;
        border_rect.w = GetWidth();
        border_rect.h = GetHeight();

        Render(*tgx_border, &border_rect);
    }
}

bool GUI::CheckButtonCollision(uint32_t rx, uint32_t ry)
{
    // todo
    return false;
}


/* Dialog functions */
bool DialogWindow::RenderButton(DialogButton style, const std::wstring& text, uint32_t x, uint32_t y)
{
    const static int button_indices[] = {
        20, 23, 29, 32, 35, 38, 41, 44, 96,
    };

    auto dim = GetStringPixelDim(text, FONT_SMALL);

    auto atlas = interface_icons;
    SDL_Rect rect = atlas->Get(button_indices[style]);;

    int mx = GetMouseX();
    int my = GetMouseY();

    // highlight
    int rw = GetTargetWidth() * rect.w;
    int rh = GetTargetHeight() * rect.h;
    int rx = ToCoordX(GetTargetX()) + x;
    int ry = ToCoordY(GetTargetY()) + y;

    bool selected = false;
    if(mx > rx && mx < rx + rw && my > ry && my < ry + rh) {
        atlas->SetBlendMode(SDL_BLENDMODE_ADD);

        rect = atlas->Get(button_indices[style] + 1);
        Rendering::Render(*atlas.get(), int(x)-5, int(y)-5, &rect);

        atlas->SetBlendMode(SDL_BLENDMODE_MOD);

        rect = atlas->Get(button_indices[style] + 2);
        Rendering::Render(*atlas.get(), int(x)-5, int(y)-5, &rect);

        atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
        selected = true;
    }

    rect = atlas->Get(button_indices[style]);

    // button
    Rendering::Render(*atlas.get(), int(x), int(y), &rect);

    // text
    Rendering::RenderText(text, Rect<int>(x+4, y+4, rect.w-4, rect.h-4), Align::CENTER, FONT_LARGE, false);

    // TODO
    if(clicked && selected) {
        clicked = false;
        return true;
    } else return false;
}

void DialogWindow::RenderDialogBorder(int x, int y, int nx, int ny)
{
    auto atlas = interface_icons;
    SDL_Rect rect;

    // background
    RenderRect(Rect<int>{ x+8, y+8, 8+nx*24, 8+ny*24 }, 0, 0, 0, 128, true);

    /* Render alpha masks */
    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    // corners
    rect = atlas->Get(3);
    Rendering::Render(*atlas, x, y, &rect);
    rect = atlas->Get(15);
    Rendering::Render(*atlas, x, y+ny*24, &rect);
    rect = atlas->Get(5);
    Rendering::Render(*atlas, x+nx*24, y, &rect);
    rect = atlas->Get(17);
    Rendering::Render(*atlas, x+nx*24, y+ny*24, &rect);

    // edges
    for(int ix = x+24; ix < x+nx*24; ix+=24) {
        rect = atlas->Get(4);
        Rendering::Render(*atlas, ix, y, &rect);
        rect = atlas->Get(16);
        Rendering::Render(*atlas, ix, y+ny*24, &rect);
    }
    for(int iy = y+24; iy < y+ny*24; iy+=24) {
        rect = atlas->Get(9);
        Rendering::Render(*atlas, x, iy, &rect);
        rect = atlas->Get(11);
        Rendering::Render(*atlas, x+nx*24, iy, &rect);
    }

    /* Render color */
    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    // corners
    rect = atlas->Get(0);
    Rendering::Render(*atlas, x, y, &rect);
    rect = atlas->Get(12);
    Rendering::Render(*atlas, x, y+ny*24, &rect);
    rect = atlas->Get(2);
    Rendering::Render(*atlas, x+nx*24, y, &rect);
    rect = atlas->Get(14);
    Rendering::Render(*atlas, x+nx*24, y+ny*24, &rect);

    // edges
    for(int ix = x+24; ix < x+nx*24; ix+=24) {
        rect = atlas->Get(1);
        Rendering::Render(*atlas, ix, y, &rect);
        rect = atlas->Get(13);
        Rendering::Render(*atlas, ix, y+ny*24, &rect);
    }
    for(int iy = y+24; iy < y+ny*24; iy+=24) {
        rect = atlas->Get(6);
        Rendering::Render(*atlas, x, iy, &rect);
        rect = atlas->Get(8);
        Rendering::Render(*atlas, x+nx*24, iy, &rect);
    }

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void DialogWindow::RenderDeco(Deco type, int x, int y)
{
    auto atlas = interface_icons;

    atlas->SetBlendMode(SDL_BLENDMODE_ADD);

    SDL_Rect rect = atlas->Get(91);
    Rendering::Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_MOD);

    rect = atlas->Get(90);
    Rendering::Render(*atlas, x, y, &rect);

    atlas->SetBlendMode(SDL_BLENDMODE_BLEND);
}

void DialogWindow::RenderDialogTextBox(int x, int y, int w, int h, const std::wstring &text, bool deco)
{
    RenderRect(Rect<int>{ x+8, y+8, 8+w, 8+h }, 24, 80, 24, 200, true);
    RenderRect(Rect<int>{ x+8, y+8, 8+w, 8+h }, 247, 235, 198, 255, false);

    auto dim = GetStringPixelDim(text, FONT_LARGE);
    RenderText(text, x + 20 + (w / 2) - (dim.first / 2), y+25, FONT_LARGE);

    if(deco) {
        RenderDeco(Deco::LARGE,      x + 11 , y + (h/2)-11);
        RenderDeco(Deco::LARGE, x + (w - 16), y + (h/2)-11);
    }
}

DialogResult DialogWindow::QuitDialog()
{
    int w = ToCoordX(GetTargetWidth());
    int h = ToCoordY(GetTargetHeight());

    int x = (w / 2) - (18*24 / 2);
    int y = (h / 2) - (7*24  / 2);

    RenderDialogBorder(x, y, 18, 6);
    RenderDialogTextBox(x, y, 433, 64, GetString(T_MAIN_MENU, 5), true);

    if(RenderButton(BUTTON_4, GetString(T_GAME_OPTIONS, 22), x+105, y+105)) {
        return QUIT;
    }

    if(RenderButton(BUTTON_4, GetString(T_GAME_OPTIONS, 23), x+245, y+105)) {
        return BACK;
    }

    return IDLE;
}

DialogResult DialogWindow::LoadDialog()
{
    int w = ToCoordX(GetTargetWidth());
    int h = ToCoordY(GetTargetHeight());

    int x = (w / 2) - (30*24 / 2);
    int y = (h / 2) - (17*24 / 2);

    const std::wstring &load = GetString(T_GAME_OPTIONS, 2);

    RenderDialogBorder(x, y, 30, 17);
    RenderDialogTextBox(x, y, 288, 64, load, true);

    if(RenderButton(BUTTON_3, load, x+52, y+300)) {
        // TODO
    }
    if(RenderButton(BUTTON_3, GetString(T_GAME_OPTIONS, 17), x+52, y+340)) {
        return BACK;
    }

    table.Render(x + 330, y + 28, 363);

    return IDLE;
}

DialogResult DialogWindow::CombatMenuDialog()
{
    int x = (1024 / 2) - (416 / 2);
    int y = 90;

    RenderDialogTextBox(x, y, 416, 64, GetString(T_MAIN_MENU, 15), true);
    table.Render(x + 37, y + 90, 363);

    return IDLE;
}

DialogResult DialogWindow::SiegeMenuDialog()
{
    return IDLE;
}

DialogResult DialogWindow::EconomicsMenuDialog()
{
    int w = ToCoordX(GetTargetWidth());
    int h = ToCoordY(GetTargetHeight());

    int x = (1024 / 2) - (416 / 2);
    int y = 250;

    RenderDialogTextBox(x, y, 416, 64, GetString(T_MAIN_MENU, 15), true);
    table.Render(x + 37, y + 90, 363);

    return IDLE;
}

DialogResult DialogWindow::SettingsDialog()
{
    int w = ToCoordX(GetTargetWidth());
    int h = ToCoordY(GetTargetHeight());

    int x = (w / 2) - (21*24 / 2);
    int y = (h / 2) - (15*24 / 2);

    switch(state) {
    case State::GAME_OPTIONS: {
        RenderDialogBorder(x, y, 21, 15);
        RenderDialogTextBox(x, y, 505, 64, GetString(T_GAME_OPTIONS, 1), true);

        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 25), x + 110, y + 110)) {
            state = State::GAMEPLAY_OPTIONS;
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 4), x + 110, y + 145)) {
            state = State::VIDEO_OPTIONS;
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 5), x + 110, y + 180)) {
            state = State::SOUND_OPTIONS;
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 45), x + 110, y + 215)) {
            lineEdit.BeginInput();
            state = State::CHANGE_NAME;
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 17), x + 110, y + 260)) {
            return BACK;
        }
    } break;
    case State::GAMEPLAY_OPTIONS: {
        RenderDialogBorder(x, y, 21, 15);
        RenderDialogTextBox(x, y, 505, 64, GetString(T_GAME_OPTIONS, 25), true);

        if(RenderButton(BUTTON_3, GetString(T_GAME_OPTIONS, 18), x + 45, y + 235)) {
            state = State::GAME_OPTIONS;
        }
        if(RenderButton(BUTTON_3, GetString(T_GAME_OPTIONS, 17), x + 265, y + 235)) {
            state = State::GAME_OPTIONS;
        }
    } break;
    case State::VIDEO_OPTIONS: {
        RenderDialogBorder(x, y, 21, 15);
        RenderDialogTextBox(x, y, 505, 64, GetString(T_GAME_OPTIONS, 4), true);

        if(RenderButton(BUTTON_3, GetString(T_GAME_OPTIONS, 18), x + 45, y + 265)) {
            state = State::GAME_OPTIONS;
        }
        if(RenderButton(BUTTON_3, GetString(T_GAME_OPTIONS, 17), x + 265, y + 265)) {
            state = State::GAME_OPTIONS;
        }
    } break;
    case State::SOUND_OPTIONS: {
        RenderDialogBorder(x, y, 21, 15);
        RenderDialogTextBox(x, y, 505, 64, GetString(T_GAME_OPTIONS, 5), true);

        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 17), x + 110, y + 265)) {
            state = State::GAME_OPTIONS;
        }
    } break;
    case State::CHANGE_NAME: {
        y = (768  / 2) - (8*24 / 2);

        RenderDialogBorder(x, y, 21, 9);
        RenderDialogTextBox(x, y, 505, 64, GetString(T_GAME_OPTIONS, 45), true);

        if(RenderButton(BUTTON_3, GetString(T_GAME_OPTIONS, 17), x + 260, y + 170)) {
            lineEdit.EndInput();

            CfgFile &cfg = GetCfg();
            cfg.username = lineEdit.GetLine();

            state = State::GAME_OPTIONS;
        }

        lineEdit.Render(x+85, y+110, 34);
    } break;
    default: break;
    }

    return IDLE;
}

DialogResult DialogWindow::EscMenu()
{
    int w = ToCoordX(GetTargetWidth());
    int h = ToCoordY(GetTargetHeight());

    int x = (w / 2) - (21*24 / 2);
    int y = (h / 2) - (15*24 / 2);

    switch(state) {
    case State::GAME_OPTIONS: {
        RenderDialogBorder(x, y, 21, 15);
        RenderDialogTextBox(x, y, 505, 64, GetString(T_GAME_OPTIONS, 1), true);

        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 3), x + 110, y + 90)) {
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 2), x + 110, y + 122)) {
            state = State::LOAD;
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 24), x + 110, y + 152)) {
            state = State::OPTIONS;
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 26), x + 110, y + 182)) {
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 34), x + 110, y + 212)) {
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 7), x + 110, y + 242)) {
            state = State::QUIT_MISSION;
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 9), x + 110, y + 272)) {
            state = State::EXIT_STRONGHOLD;
        }
        if(RenderButton(BUTTON_1, GetString(T_GAME_OPTIONS, 10), x + 110, y + 302)) {
            return BACK;
        }
    } break;
    case State::EXIT_STRONGHOLD: {
        DialogResult res = QuitDialog();
        if(res == QUIT) {
            return QUIT;
        }else if(res == BACK) {
            state = State::GAME_OPTIONS;
        }
    } break;
    case State::OPTIONS: {
        DialogResult res = SettingsDialog();
        if(res == BACK) state = State::GAME_OPTIONS;
    } break;
    case State::LOAD: {
        DialogResult res = LoadDialog();
        if(res == BACK) state = State::GAME_OPTIONS;
        else if(res == LOAD) {
            // TODO
        }
    } break;
    case State::QUIT_MISSION: {
        DialogResult res = QuitMissionDialog();
        if(res == QUIT_MISSION) {
            return QUIT_MISSION;
        }else if(res == BACK) {
            state = State::GAME_OPTIONS;
        }
    } break;
    default: break;
    }

    return IDLE;
}

DialogResult DialogWindow::QuitMissionDialog()
{
    int w = ToCoordX(GetTargetWidth());
    int h = ToCoordY(GetTargetHeight());

    int x = (w / 2) - (18*24 / 2);
    int y = (h / 2) - (7*24  / 2);

    RenderDialogBorder(x, y, 18, 6);
    RenderDialogTextBox(x, y, 433, 64, GetString(T_GAME_OPTIONS, 7), true);

    if(RenderButton(BUTTON_4, GetString(T_GAME_OPTIONS, 22), x+105, y+105)) {
        return QUIT_MISSION;
    }

    if(RenderButton(BUTTON_4, GetString(T_GAME_OPTIONS, 23), x+245, y+105)) {
        return BACK;
    }

    return IDLE;
}

void DialogWindow::InitModCampaignTable(std::vector<ModCampaign> *campaigns)
{
    table.Create(1, campaigns->size());
    for(int i = 0; i < campaigns->size(); i++) {
        ModCampaign &c = campaigns->at(i);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        table.SetText(0, i, conv.from_bytes(c.title));
    }
}

void DialogWindow::onEventReceive(Mouse &event) {
    if(event.GetType() == BUTTONDOWN) {
        clicked = true;
    }else if(event.GetType() == BUTTONUP) {
        clicked = false;
    }
}

DialogWindow::DialogWindow(DialogType type)
{
    Init(type);
}

void DialogWindow::Init(DialogType type)
{
    this->type = type;
    this->open = true;

    std::vector<ModCampaign>
        *campaigns_military = GetMilitaryCampaigns(),
        *campaigns_eco = GetEcoCampaigns();

    // Initialize the window based on the type of dialog to be displayed //
    switch(type) {
    case DialogType::LOAD: {
        table.Create(2, 16);
        table.SetRowName(0, GetString(T_GAME_OPTIONS, 27));
        table.SetRowName(1, GetString(T_GAME_OPTIONS, 28));
    } break;
    case DialogType::COMBAT_MENU: {
        table.Create(1, 21);
        table.Scrollable(false);
        table.RenderNames(false);

        for(int i = 0; i < 21; i++) {
            table.SetText(0, i, std::to_wstring(i+1) + L" " + GetString(T_MISSION_NAMES, 1+i));
        }
    } break;
    case DialogType::ECO_MENU: {
        table.Create(1, 5);
        table.Scrollable(false);
        table.RenderNames(false);
    } break;
    case DialogType::SETTINGS:
    case DialogType::ESC_MENU: {
        CfgFile &cfg = GetCfg();

        lineEdit.Init();
        lineEdit.SetLine(cfg.username);
    } break;
    case DialogType::CAMPAIGN_SELECT_MILITARY: {
        InitModCampaignTable(campaigns_military);
    } break;
    case DialogType::CAMPAIGN_SELECT_ECO: {
        InitModCampaignTable(campaigns_eco);
    } break;
    default: break;
    }
}

void DialogWindow::Reset()
{
    table.Destroy();
    lineEdit.SetLine(L"");
    lineEdit.SetPos(0);
}

DialogResult DialogWindow::Render()
{
    if(!open) return IDLE;

    switch(type) {
    case DialogType::QUIT: {
        return QuitDialog();
    } break;
    case DialogType::LOAD: {
        return LoadDialog();
    } break;
    case DialogType::COMBAT_MENU: {
        return CombatMenuDialog();
    } break;
    case DialogType::SIEGE_MENU: {
    } break;
    case DialogType::ECO_MENU: {
        return EconomicsMenuDialog();
    } break;
    case DialogType::SETTINGS: {
        return SettingsDialog();
    } break;
    case DialogType::ESC_MENU: {
        return EscMenu();
    } break;
    default: break;
    }

    return IDLE;
}

int DialogWindow::GetSelectedIndex()
{
    return table.GetSelected();
}

