#include <Menus/CombatMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

CombatMenu::CombatMenu(std::shared_ptr<GameManager> man) :
    manager(man),
    /* UI stuff */
    ui_back_to_main(man),
    ui_combat_campaign(man),
    ui_combat_invasion(man),
    ui_combat_siege(man),
    ui_combat_multiplayer(man)
{
    gm1_icons_combat = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_combat.gm1").lock();
    tgx_bg = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_combat.tgx").lock();
}

CombatMenu::~CombatMenu() {

}


UIState CombatMenu::EnterMenu() {
    ui_combat_campaign.Scale(0.17578125, 0.234375);
    ui_combat_invasion.Scale(0.17578125, 0.234375);
    ui_combat_multiplayer.Scale(0.17578125, 0.234375);
    ui_combat_siege.Scale(0.17578125, 0.234375);

    ui_back_to_main.Scale(0.17578125, 0.234375);
    ui_back_to_main.Translate(0.12, 0.67);

    ui_back_to_main.Show();
    ui_combat_campaign.Show();
    ui_combat_invasion.Show();
    ui_combat_multiplayer.Show();
    ui_combat_siege.Show();

    UIState currentState = COMBAT_MENU;
    int32_t glareCounter = 265;

    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(*tgx_bg);

        Draw(glareCounter);

        if(ui_back_to_main.IsClicked()) {
            return MAIN_MENU;
        } else if(ui_combat_campaign.IsClicked()) {
            return MILITARY_CAMPAIGN_MENU ;
        } else if(ui_combat_siege.IsClicked()) {
            return SIEGE_MENU;
        } else if(ui_combat_invasion.IsClicked()) {
            return INVASION_MENU;
        } else if(ui_combat_multiplayer.IsClicked()) {
            return MULTIPLAYER_MENU;
        }
        if(currentState != COMBAT_MENU){
            ui_back_to_main.Hide();
            ui_combat_campaign.Hide();
            ui_combat_invasion.Hide();
            ui_combat_multiplayer.Hide();
            ui_combat_siege.Hide();
            return currentState;
        }

        glareCounter = (glareCounter+1)%360;

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_GAME;
}

void CombatMenu::Draw(const int32_t glareCounter)
{
    manager->Render(*tgx_bg);
    auto icons_combat = gm1_icons_combat->GetTextureAtlas().lock();
    ui_back_to_main.Render(
        [&]() -> Texture& {
            if(ui_back_to_main.IsMouseOver()) icons_combat->SetRect(icons_combat->Get(69));
            else icons_combat->SetRect(icons_combat->Get(68));
            return *icons_combat;
        });

    ui_combat_campaign.Render(
        [&]() -> Texture& {
            if(ui_combat_campaign.IsMouseOver()) {
                icons_combat->SetRect(icons_combat->Get(16));//
                ui_combat_campaign.Translate(0.183, 0.276);
            } else {
                if(glareCounter >= 90) icons_combat->SetRect(icons_combat->Get(0));
                else if(glareCounter < 6) icons_combat->SetRect(icons_combat->Get(1));
                else if(glareCounter < 12) icons_combat->SetRect(icons_combat->Get(2));
                else if(glareCounter < 18) icons_combat->SetRect(icons_combat->Get(3));
                else if(glareCounter < 24) icons_combat->SetRect(icons_combat->Get(4));
                else if(glareCounter < 30) icons_combat->SetRect(icons_combat->Get(5));
                else if(glareCounter < 36) icons_combat->SetRect(icons_combat->Get(6));
                else if(glareCounter < 42) icons_combat->SetRect(icons_combat->Get(7));
                else if(glareCounter < 48) icons_combat->SetRect(icons_combat->Get(8));
                else if(glareCounter < 54) icons_combat->SetRect(icons_combat->Get(9));
                else if(glareCounter < 60) icons_combat->SetRect(icons_combat->Get(10));
                else if(glareCounter < 66) icons_combat->SetRect(icons_combat->Get(11));
                else if(glareCounter < 72) icons_combat->SetRect(icons_combat->Get(12));
                else if(glareCounter < 78) icons_combat->SetRect(icons_combat->Get(13));
                else if(glareCounter < 84) icons_combat->SetRect(icons_combat->Get(14));
                else if(glareCounter < 90) icons_combat->SetRect(icons_combat->Get(15));
                ui_combat_campaign.Translate(0.182, 0.277);
            }
            return *icons_combat;
        });

    ui_combat_siege.Render(
        [&]() -> Texture& {
            if(ui_combat_siege.IsMouseOver()) {
                icons_combat->SetRect(icons_combat->Get(33));
                ui_combat_siege.Translate(0.336, 0.276);
            } else {
                if(glareCounter < 91 || glareCounter >= 180) icons_combat->SetRect(icons_combat->Get(17));
                else if(glareCounter < 96) icons_combat->SetRect(icons_combat->Get(18));
                else if(glareCounter < 102) icons_combat->SetRect(icons_combat->Get(19));
                else if(glareCounter < 114) icons_combat->SetRect(icons_combat->Get(21));
                else if(glareCounter < 108) icons_combat->SetRect(icons_combat->Get(20));
                else if(glareCounter < 120) icons_combat->SetRect(icons_combat->Get(22));
                else if(glareCounter < 126) icons_combat->SetRect(icons_combat->Get(23));
                else if(glareCounter < 132) icons_combat->SetRect(icons_combat->Get(24));
                else if(glareCounter < 138) icons_combat->SetRect(icons_combat->Get(25));
                else if(glareCounter < 144) icons_combat->SetRect(icons_combat->Get(26));
                else if(glareCounter < 150) icons_combat->SetRect(icons_combat->Get(27));
                else if(glareCounter < 156) icons_combat->SetRect(icons_combat->Get(28));
                else if(glareCounter < 162) icons_combat->SetRect(icons_combat->Get(29));
                else if(glareCounter < 168) icons_combat->SetRect(icons_combat->Get(30));
                else if(glareCounter < 174) icons_combat->SetRect(icons_combat->Get(31));
                else if(glareCounter < 180) icons_combat->SetRect(icons_combat->Get(32));
                ui_combat_siege.Translate(0.337, 0.275);
            }
            return *icons_combat;
        });

    ui_combat_invasion.Render(
        [&]() -> Texture& {
            if(ui_combat_invasion.IsMouseOver()) {
                icons_combat->SetRect(icons_combat->Get(50));
                ui_combat_invasion.Translate(0.492, 0.276);
            } else {
                if(glareCounter < 181 || glareCounter >= 240) icons_combat->SetRect(icons_combat->Get(34));
                else if(glareCounter < 186) icons_combat->SetRect(icons_combat->Get(35));
                else if(glareCounter < 192) icons_combat->SetRect(icons_combat->Get(36));
                else if(glareCounter < 198) icons_combat->SetRect(icons_combat->Get(37));
                else if(glareCounter < 204) icons_combat->SetRect(icons_combat->Get(38));
                else if(glareCounter < 210) icons_combat->SetRect(icons_combat->Get(39));
                else if(glareCounter < 216) icons_combat->SetRect(icons_combat->Get(40));
                else if(glareCounter < 222) icons_combat->SetRect(icons_combat->Get(41));
                else if(glareCounter < 228) icons_combat->SetRect(icons_combat->Get(42));
                else if(glareCounter < 234) icons_combat->SetRect(icons_combat->Get(43));
                else if(glareCounter < 240) icons_combat->SetRect(icons_combat->Get(44));
                else if(glareCounter < 246) icons_combat->SetRect(icons_combat->Get(45));
                else if(glareCounter < 252) icons_combat->SetRect(icons_combat->Get(46));
                else if(glareCounter < 258) icons_combat->SetRect(icons_combat->Get(47));
                else if(glareCounter < 264) icons_combat->SetRect(icons_combat->Get(48));
                else if(glareCounter < 270) icons_combat->SetRect(icons_combat->Get(49));
                ui_combat_invasion.Translate(0.492, 0.275);
            }
            return *icons_combat;
        });

    ui_combat_multiplayer.Render(
        [&]() -> Texture& {
            if(ui_combat_multiplayer.IsMouseOver()) {
                icons_combat->SetRect(icons_combat->Get(67));
                ui_combat_multiplayer.Translate(0.648, 0.278);
            } else {
                if(glareCounter < 271) icons_combat->SetRect(icons_combat->Get(51));
                else if(glareCounter < 276) icons_combat->SetRect(icons_combat->Get(52));
                else if(glareCounter < 282) icons_combat->SetRect(icons_combat->Get(53));
                else if(glareCounter < 288) icons_combat->SetRect(icons_combat->Get(54));
                else if(glareCounter < 294) icons_combat->SetRect(icons_combat->Get(55));
                else if(glareCounter < 300) icons_combat->SetRect(icons_combat->Get(56));
                else if(glareCounter < 306) icons_combat->SetRect(icons_combat->Get(57));
                else if(glareCounter < 312) icons_combat->SetRect(icons_combat->Get(58));
                else if(glareCounter < 318) icons_combat->SetRect(icons_combat->Get(59));
                else if(glareCounter < 324) icons_combat->SetRect(icons_combat->Get(60));
                else if(glareCounter < 330) icons_combat->SetRect(icons_combat->Get(61));
                else if(glareCounter < 336) icons_combat->SetRect(icons_combat->Get(62));
                else if(glareCounter < 342) icons_combat->SetRect(icons_combat->Get(63));
                else if(glareCounter < 348) icons_combat->SetRect(icons_combat->Get(64));
                else if(glareCounter < 354) icons_combat->SetRect(icons_combat->Get(65));
                else if(glareCounter < 360) icons_combat->SetRect(icons_combat->Get(66));
                ui_combat_multiplayer.Translate(0.648, 0.276);
            }
            return *icons_combat;
        });
}

