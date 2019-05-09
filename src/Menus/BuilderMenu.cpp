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
    int32_t glareCounter = 265;

    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        manager->Render(*tgx_bg);

        Draw(glareCounter);

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

        glareCounter = (glareCounter+1)%360;

        manager->Flush();
        manager->EndTimer();
    }

    return EXIT_GAME;
}

void BuilderMenu::Draw(const int32_t glareCounter)
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
                if(glareCounter >= 90) icons_builder->SetRect(icons_builder->Get(0));
                else if(glareCounter < 6) icons_builder->SetRect(icons_builder->Get(1));
                else if(glareCounter < 12) icons_builder->SetRect(icons_builder->Get(2));
                else if(glareCounter < 18) icons_builder->SetRect(icons_builder->Get(3));
                else if(glareCounter < 24) icons_builder->SetRect(icons_builder->Get(4));
                else if(glareCounter < 30) icons_builder->SetRect(icons_builder->Get(5));
                else if(glareCounter < 36) icons_builder->SetRect(icons_builder->Get(6));
                else if(glareCounter < 42) icons_builder->SetRect(icons_builder->Get(7));
                else if(glareCounter < 48) icons_builder->SetRect(icons_builder->Get(8));
                else if(glareCounter < 54) icons_builder->SetRect(icons_builder->Get(9));
                else if(glareCounter < 60) icons_builder->SetRect(icons_builder->Get(10));
                else if(glareCounter < 66) icons_builder->SetRect(icons_builder->Get(11));
                else if(glareCounter < 72) icons_builder->SetRect(icons_builder->Get(12));
                else if(glareCounter < 78) icons_builder->SetRect(icons_builder->Get(13));
                else if(glareCounter < 84) icons_builder->SetRect(icons_builder->Get(14));
                else if(glareCounter < 90) icons_builder->SetRect(icons_builder->Get(15));
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
                if(glareCounter < 91 || glareCounter >= 180) icons_builder->SetRect(icons_builder->Get(17));
                else if(glareCounter < 96) icons_builder->SetRect(icons_builder->Get(18));
                else if(glareCounter < 102) icons_builder->SetRect(icons_builder->Get(19));
                else if(glareCounter < 114) icons_builder->SetRect(icons_builder->Get(21));
                else if(glareCounter < 108) icons_builder->SetRect(icons_builder->Get(20));
                else if(glareCounter < 120) icons_builder->SetRect(icons_builder->Get(22));
                else if(glareCounter < 126) icons_builder->SetRect(icons_builder->Get(23));
                else if(glareCounter < 132) icons_builder->SetRect(icons_builder->Get(24));
                else if(glareCounter < 138) icons_builder->SetRect(icons_builder->Get(25));
                else if(glareCounter < 144) icons_builder->SetRect(icons_builder->Get(26));
                else if(glareCounter < 150) icons_builder->SetRect(icons_builder->Get(27));
                else if(glareCounter < 156) icons_builder->SetRect(icons_builder->Get(28));
                else if(glareCounter < 162) icons_builder->SetRect(icons_builder->Get(29));
                else if(glareCounter < 168) icons_builder->SetRect(icons_builder->Get(30));
                else if(glareCounter < 174) icons_builder->SetRect(icons_builder->Get(31));
                else if(glareCounter < 180) icons_builder->SetRect(icons_builder->Get(32));
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
                if(glareCounter < 181 || glareCounter >= 240) icons_builder->SetRect(icons_builder->Get(34));
                else if(glareCounter < 186) icons_builder->SetRect(icons_builder->Get(35));
                else if(glareCounter < 192) icons_builder->SetRect(icons_builder->Get(36));
                else if(glareCounter < 198) icons_builder->SetRect(icons_builder->Get(37));
                else if(glareCounter < 204) icons_builder->SetRect(icons_builder->Get(38));
                else if(glareCounter < 210) icons_builder->SetRect(icons_builder->Get(39));
                else if(glareCounter < 216) icons_builder->SetRect(icons_builder->Get(40));
                else if(glareCounter < 222) icons_builder->SetRect(icons_builder->Get(41));
                else if(glareCounter < 228) icons_builder->SetRect(icons_builder->Get(42));
                else if(glareCounter < 234) icons_builder->SetRect(icons_builder->Get(43));
                else if(glareCounter < 240) icons_builder->SetRect(icons_builder->Get(44));
                else if(glareCounter < 246) icons_builder->SetRect(icons_builder->Get(45));
                else if(glareCounter < 252) icons_builder->SetRect(icons_builder->Get(46));
                else if(glareCounter < 258) icons_builder->SetRect(icons_builder->Get(47));
                else if(glareCounter < 264) icons_builder->SetRect(icons_builder->Get(48));
                else if(glareCounter < 270) icons_builder->SetRect(icons_builder->Get(49));
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
                if(glareCounter < 271) icons_builder->SetRect(icons_builder->Get(51));
                else if(glareCounter < 276) icons_builder->SetRect(icons_builder->Get(52));
                else if(glareCounter < 282) icons_builder->SetRect(icons_builder->Get(53));
                else if(glareCounter < 288) icons_builder->SetRect(icons_builder->Get(54));
                else if(glareCounter < 294) icons_builder->SetRect(icons_builder->Get(55));
                else if(glareCounter < 300) icons_builder->SetRect(icons_builder->Get(56));
                else if(glareCounter < 306) icons_builder->SetRect(icons_builder->Get(57));
                else if(glareCounter < 312) icons_builder->SetRect(icons_builder->Get(58));
                else if(glareCounter < 318) icons_builder->SetRect(icons_builder->Get(59));
                else if(glareCounter < 324) icons_builder->SetRect(icons_builder->Get(60));
                else if(glareCounter < 330) icons_builder->SetRect(icons_builder->Get(61));
                else if(glareCounter < 336) icons_builder->SetRect(icons_builder->Get(62));
                else if(glareCounter < 342) icons_builder->SetRect(icons_builder->Get(63));
                else if(glareCounter < 348) icons_builder->SetRect(icons_builder->Get(64));
                else if(glareCounter < 354) icons_builder->SetRect(icons_builder->Get(65));
                else if(glareCounter < 360) icons_builder->SetRect(icons_builder->Get(66));
                ui_builder_multiplayer_map.Translate(0.648, 0.276);
            }
            return *icons_builder;
        });
}

