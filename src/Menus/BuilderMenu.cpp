#include <Menus/BuilderMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

BuilderMenu::BuilderMenu(std::shared_ptr<GameManager> man) :
    manager(man),
    /* UI stuff */
    ui_back_to_main(man),
    ui_builder_working_map(man),
    ui_builder_stand_alone_mission(man),
    ui_builder_siege_that(man),
    ui_builder_multiplayer_map(man)
{
    gm1_icons_builder = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_builder.gm1").lock();
    tgx_bg = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_builder.tgx").lock();
}

BuilderMenu::~BuilderMenu() {

}


UIState BuilderMenu::EnterMenu() {
    ui_builder_multiplayer_map.Scale(0.17578125, 0.234375);
    ui_builder_siege_that.Scale(0.17578125, 0.234375);
    ui_builder_stand_alone_mission.Scale(0.17578125, 0.234375);
    ui_builder_working_map.Scale(0.17578125, 0.234375);

    ui_back_to_main.Scale(0.17578125, 0.234375);
    ui_back_to_main.Translate(0.12, 0.67);

    ui_back_to_main.Show();
    ui_builder_multiplayer_map.Show();
    ui_builder_siege_that.Show();
    ui_builder_stand_alone_mission.Show();
    ui_builder_working_map.Show();


    UIState currentState = BUILDER_MENU;
    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(*tgx_bg);

        Draw();

        if(ui_back_to_main.IsClicked()){
            currentState = MAIN_MENU;
        } else if(ui_builder_working_map.IsClicked()){
            currentState = WORKING_MAP_MENU;
        } else if(ui_builder_stand_alone_mission.IsClicked()){
            currentState = STAND_ALONE_MISSION_MENU;
        } else if(ui_builder_siege_that.IsClicked()){
            currentState = SIEGE_THAT_MENU;
        } else if(ui_builder_multiplayer_map.IsClicked()){
            currentState = MULTIPLAYER_MAP_MENU;
        }

        if(currentState != BUILDER_MENU){
            ui_back_to_main.Hide();
            ui_builder_multiplayer_map.Hide();
            ui_builder_siege_that.Hide();
            ui_builder_stand_alone_mission.Hide();
            ui_builder_working_map.Hide();
            return currentState;
        }

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_GAME;
}

void BuilderMenu::Draw()
{
    manager->Render(*tgx_bg);
    auto icons_builder = gm1_icons_builder->GetTextureAtlas().lock();
    ui_back_to_main.Render(
        [&]() -> Texture& {
            if(ui_back_to_main.IsMouseOver()) icons_builder->SetRect(icons_builder->Get(71));
            else icons_builder->SetRect(icons_builder->Get(70));
            return *icons_builder;
        });

    ui_builder_working_map.Render(
        [&]() -> Texture& {
            if(ui_builder_working_map.IsMouseOver()) {
                icons_builder->SetRect(icons_builder->Get(16));
                ui_builder_working_map.Translate(0.183, 0.276);
            } else {
                icons_builder->SetRect(icons_builder->Get(0));
                ui_builder_working_map.Translate(0.182, 0.277);
            }
            return *icons_builder;
        });

    ui_builder_stand_alone_mission.Render(
        [&]() -> Texture& {
            if(ui_builder_stand_alone_mission.IsMouseOver()) {
                icons_builder->SetRect(icons_builder->Get(33));
                ui_builder_stand_alone_mission.Translate(0.336, 0.276);
            } else {
                icons_builder->SetRect(icons_builder->Get(17));
                ui_builder_stand_alone_mission.Translate(0.337, 0.275);
            }
            return *icons_builder;
        });

    ui_builder_siege_that.Render(
        [&]() -> Texture& {
            if(ui_builder_siege_that.IsMouseOver()) {
                icons_builder->SetRect(icons_builder->Get(50));
                ui_builder_siege_that.Translate(0.492, 0.276);
            } else {
                icons_builder->SetRect(icons_builder->Get(34));
                ui_builder_siege_that.Translate(0.492, 0.275);
            }
            return *icons_builder;
        });

    ui_builder_multiplayer_map.Render(
        [&]() -> Texture& {
            if(ui_builder_multiplayer_map.IsMouseOver()) {
                icons_builder->SetRect(icons_builder->Get(67));
                ui_builder_multiplayer_map.Translate(0.648, 0.278);
            } else {
                icons_builder->SetRect(icons_builder->Get(51));
                ui_builder_multiplayer_map.Translate(0.648, 0.276);
            }
            return *icons_builder;
        });
}

