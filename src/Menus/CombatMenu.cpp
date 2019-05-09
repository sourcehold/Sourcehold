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
    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(*tgx_bg);

        Draw();

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

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_GAME;
}

void CombatMenu::Draw()
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
                icons_combat->SetRect(icons_combat->Get(0));
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
                icons_combat->SetRect(icons_combat->Get(17));
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
                icons_combat->SetRect(icons_combat->Get(34));
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
                icons_combat->SetRect(icons_combat->Get(51));
                ui_combat_multiplayer.Translate(0.648, 0.276);
            }
            return *icons_combat;
        });
}

