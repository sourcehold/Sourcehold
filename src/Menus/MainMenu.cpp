#include <Menus/MainMenu.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
    manager(man),
    /* UI stuff */
    ui_exit(man),
    ui_firefly(man),
    ui_tutorial(man),
    ui_combat(man),
    ui_economic(man),
    ui_builder(man),
    ui_load(man),
    ui_settings(man)
{
    gm1_icons_additional = manager->GetGm1(manager->GetDirectory() / "gm/interface_buttons.gm1").lock();
    gm1_icons_main = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end.gm1").lock();
    tgx_bg1 = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_main.tgx").lock();
}

MainMenu::~MainMenu() {

}


UIState MainMenu::EnterMenu() {
    /* Exit button */
    ui_exit.Translate(0.12, 0.67);
    ui_exit.Scale(0.17578125, 0.234375);

    /* 'Start credits' button */
    ui_firefly.Translate(0.423, 0.774);
    ui_firefly.Scale(0.1337890625, 0.098958334);

    /* 'Start tutorial' button */
    ui_tutorial.Translate(0.691, 0.687);
    ui_tutorial.Scale(0.17578125, 0.234375);

    /* The asset files dont show it but the glass effect of the buttons moves the
       background one pixel to the right and one pixel up in the actual game. */
    ui_combat.Scale(0.17578125, 0.234375);
    ui_economic.Scale(0.17578125, 0.234375);
    ui_builder.Scale(0.17578125, 0.234375);
    ui_load.Scale(0.17578125, 0.234375);

    ui_settings.Scale(0.113, 0.152);
    ui_settings.Translate(0.6, 0.6);

    ui_exit.Show();
    ui_firefly.Show();
    ui_tutorial.Show();
    ui_combat.Show();
    ui_economic.Show();
    ui_builder.Show();
    ui_load.Show();
    ui_settings.Show();

    UIState currentState = MAIN_MENU;

    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(*tgx_bg1);

        Draw();

        if(ui_exit.IsClicked()) {
            currentState = EXIT_GAME;
        } else if(ui_combat.IsClicked()) {
            currentState = COMBAT_MENU;
        } else if(ui_economic.IsClicked()) {
            currentState = ECONOMICS_MENU;
        } else if(ui_builder.IsClicked()) {
            currentState = BUILDER_MENU;
        } else if(ui_load.IsClicked()) {
            currentState = LOAD_SAVED_MENU;
        } else if(ui_tutorial.IsClicked()) {

        } else if(ui_settings.IsClicked()) {
            currentState = SETTINGS_MENU;
        } else if(ui_firefly.IsClicked()) {

        }

        if(currentState != MAIN_MENU){
            ui_exit.Hide();
            ui_firefly.Hide();
            ui_tutorial.Hide();
            ui_combat.Hide();
            ui_economic.Hide();
            ui_builder.Hide();
            ui_load.Hide();
            ui_settings.Hide();

            return currentState;
        }

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_GAME;
}

void MainMenu::Draw()
{
    manager->Render(*tgx_bg1);

    /* Render user interface */
    auto icons_main = gm1_icons_main->GetTextureAtlas().lock();
    auto icons_additional = gm1_icons_additional->GetTextureAtlas().lock();
    ui_exit.Render(
        [&]() -> Texture& {
            if(ui_exit.IsMouseOver()) icons_main->SetRect(icons_main->Get(69));
            else icons_main->SetRect(icons_main->Get(68));
            return *icons_main;
        });

    ui_tutorial.Render(
        [&]() -> Texture& {
            if(ui_tutorial.IsMouseOver()) icons_main->SetRect(icons_main->Get(73));
            else icons_main->SetRect(icons_main->Get(72));
            return *icons_main;
        });

    ui_combat.Render(
        [&]() -> Texture& {
            if(ui_combat.IsMouseOver()) {
                icons_main->SetRect(icons_main->Get(16));
                ui_combat.Translate(0.183, 0.276);
            } else {
                icons_main->SetRect(icons_main->Get(0));
                ui_combat.Translate(0.182, 0.277);
            }
            return *icons_main;
        });

    ui_economic.Render(
        [&]() -> Texture& {
            if(ui_economic.IsMouseOver()) {
                icons_main->SetRect(icons_main->Get(33));
                ui_economic.Translate(0.336, 0.276);
            } else {
                icons_main->SetRect(icons_main->Get(17));
                ui_economic.Translate(0.337, 0.275);
            }
            return *icons_main;
        });

    ui_builder.Render(
        [&]() -> Texture& {
            if(ui_builder.IsMouseOver()) {
                icons_main->SetRect(icons_main->Get(50));
                ui_builder.Translate(0.492, 0.276);
            } else {
                icons_main->SetRect(icons_main->Get(34));
                ui_builder.Translate(0.492, 0.275);
            }
            return *icons_main;
        });

    ui_load.Render(
        [&]() -> Texture& {
            if(ui_load.IsMouseOver()) {
                icons_main->SetRect(icons_main->Get(67));
                ui_load.Translate(0.648, 0.278);
            } else {
                icons_main->SetRect(icons_main->Get(51));
                ui_load.Translate(0.648, 0.276);

            }
            return *icons_main;
        });

    ui_firefly.Render(
            [&]() -> Texture& {
                if(ui_firefly.IsMouseOver()) {
                    icons_additional->SetRect(icons_additional->Get(77));
                } else {
                    icons_additional->SetRect(icons_additional->Get(78));
                }
                return *icons_additional;
            });
    ui_settings.Render(
            [&]() -> Texture& {
                if(ui_settings.IsMouseOver()) {
                    icons_additional->SetRect(icons_additional->Get(88));
                } else {
                    icons_additional->SetRect(icons_additional->Get(89));
                }
                return *icons_additional;
            });
}

