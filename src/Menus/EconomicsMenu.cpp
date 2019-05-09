#include <Menus/EconomicsMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

EconomicsMenu::EconomicsMenu(std::shared_ptr<GameManager> man) :
    manager(man),
    /* UI stuff */
    ui_economic_campaign(man),
    ui_economic_mission(man),
    ui_economic_free_build(man),
    ui_back_to_main(man)
{
    gm1_icons_economics = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_economics.gm1").lock();
    tgx_bg = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_economics.tgx").lock();
}

EconomicsMenu::~EconomicsMenu() {

}


UIState EconomicsMenu::EnterMenu() {
    ui_economic_campaign.Scale(0.17578125, 0.234375);
    ui_economic_free_build.Scale(0.17578125, 0.234375);
    ui_economic_mission.Scale(0.17578125, 0.234375);

    ui_back_to_main.Scale(0.17578125, 0.234375);
    ui_back_to_main.Translate(0.12, 0.67);

    ui_back_to_main.Show();
    ui_economic_campaign.Show();
    ui_economic_free_build.Show();
    ui_economic_mission.Show();

    UIState currentState = ECONOMICS_MENU;
    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(*tgx_bg);

        Draw();

        if(ui_back_to_main.IsClicked()){
            currentState = MAIN_MENU;
        } else if(ui_economic_campaign.IsClicked()){
            currentState = ECONOMICS_CAMPAIGN_MENU;
        } else if(ui_economic_mission.IsClicked()){
            currentState = ECONOMICS_MISSION_MENU;
        } else if(ui_economic_free_build.IsClicked()){
            currentState = FREE_BUILD_MENU;
        }

        if(currentState != ECONOMICS_MENU) {
            ui_back_to_main.Hide();
            ui_economic_campaign.Hide();
            ui_economic_free_build.Hide();
            ui_economic_mission.Hide();
            return currentState;
        }

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_GAME;
}

void EconomicsMenu::Draw()
{
    manager->Render(*tgx_bg);
    auto icons_economics = gm1_icons_economics->GetTextureAtlas().lock();
    ui_back_to_main.Render(
        [&]() -> Texture& {
            if(ui_back_to_main.IsMouseOver()) icons_economics->SetRect(icons_economics->Get(52));
            else icons_economics->SetRect(icons_economics->Get(51));
            return *icons_economics;
        });

    ui_economic_campaign.Render(
        [&]() -> Texture& {
            if(ui_economic_campaign.IsMouseOver()) {
                icons_economics->SetRect(icons_economics->Get(16));
                ui_economic_campaign.Translate(0.262, 0.276);
            } else {
                icons_economics->SetRect(icons_economics->Get(0));
                ui_economic_campaign.Translate(0.261, 0.277);
            }
            return *icons_economics;
        });

    ui_economic_mission.Render(
        [&]() -> Texture& {
            if(ui_economic_mission.IsMouseOver()) {
                icons_economics->SetRect(icons_economics->Get(33));
                ui_economic_mission.Translate(0.416, 0.277);
            } else {
                icons_economics->SetRect(icons_economics->Get(17));
                ui_economic_mission.Translate(0.4165, 0.277);
            }
            return *icons_economics;
        });

    ui_economic_free_build.Render(
        [&]() -> Texture& {
            if(ui_economic_free_build.IsMouseOver()) {
                icons_economics->SetRect(icons_economics->Get(50));
                ui_economic_free_build.Translate(0.571, 0.277);
            } else {
                icons_economics->SetRect(icons_economics->Get(34));
                ui_economic_free_build.Translate(0.571, 0.277);
            }
            return *icons_economics;
        });
}


