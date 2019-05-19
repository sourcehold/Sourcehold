#include <Menus/EconomicsMenu.h>

using namespace Sourcehold::Menus;
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
    /**
     * In original game the glare effect follows the same pattern for all
     * menus. That means that since in this menu the fourth button is missing
     * there is a pause between glare effect being rendered on third button
     * and first. Also, since the glare effect starts in all the menus on
     * fourth button, in this menu there is a pause in the beginning and the
     * effect is renderd on the first button after that.
     */
    int32_t glareCounter = 265;

    while(manager->Running()) {
        manager->Clear();

        manager->Render(*tgx_bg);

        Draw(glareCounter);

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

        glareCounter = (glareCounter+1)%360;

        manager->Flush();
        manager->Sync();
    }

    return EXIT_GAME;
}

void EconomicsMenu::Draw(const int32_t glareCounter)
{
	selected = ECO_NONE;

    manager->Render(*tgx_bg);
    auto icons_economics = gm1_icons_economics->GetTextureAtlas().lock();
    ui_back_to_main.Render(
        [&]() -> Texture& {
            if(ui_back_to_main.IsMouseOver()) {
				selected = ECO_BACK_TO_MAIN;
                RenderMenuText(L"Back to Main Menu");
                icons_economics->SetRect(icons_economics->Get(52));
            }
            else icons_economics->SetRect(icons_economics->Get(51));
            return *icons_economics;
        });

    ui_economic_campaign.Render(
        [&]() -> Texture& {
            if(ui_economic_campaign.IsMouseOver() && selected == ECO_NONE) {
				selected = ECO_CAMPAIGN;
                RenderMenuText(L"Play the Economics Campaign");
                icons_economics->SetRect(icons_economics->Get(16));
                ui_economic_campaign.Translate(0.262, 0.276);
            } else {
                if(glareCounter >= 90) icons_economics->SetRect(icons_economics->Get(0));
                else if(glareCounter < 6) icons_economics->SetRect(icons_economics->Get(1));
                else if(glareCounter < 12) icons_economics->SetRect(icons_economics->Get(2));
                else if(glareCounter < 18) icons_economics->SetRect(icons_economics->Get(3));
                else if(glareCounter < 24) icons_economics->SetRect(icons_economics->Get(4));
                else if(glareCounter < 30) icons_economics->SetRect(icons_economics->Get(5));
                else if(glareCounter < 36) icons_economics->SetRect(icons_economics->Get(6));
                else if(glareCounter < 42) icons_economics->SetRect(icons_economics->Get(7));
                else if(glareCounter < 48) icons_economics->SetRect(icons_economics->Get(8));
                else if(glareCounter < 54) icons_economics->SetRect(icons_economics->Get(9));
                else if(glareCounter < 60) icons_economics->SetRect(icons_economics->Get(10));
                else if(glareCounter < 66) icons_economics->SetRect(icons_economics->Get(11));
                else if(glareCounter < 72) icons_economics->SetRect(icons_economics->Get(12));
                else if(glareCounter < 78) icons_economics->SetRect(icons_economics->Get(13));
                else if(glareCounter < 84) icons_economics->SetRect(icons_economics->Get(14));
                else if(glareCounter < 90) icons_economics->SetRect(icons_economics->Get(15));
                ui_economic_campaign.Translate(0.261, 0.277);
            }
            return *icons_economics;
        });

    ui_economic_mission.Render(
        [&]() -> Texture& {
            if(ui_economic_mission.IsMouseOver() && selected == ECO_NONE) {
				selected = ECO_MISSION;
                RenderMenuText(L"Play an Economics Mission");
                icons_economics->SetRect(icons_economics->Get(33));
                ui_economic_mission.Translate(0.416, 0.277);
            } else {
                if(glareCounter < 91 || glareCounter >= 180) icons_economics->SetRect(icons_economics->Get(17));
                else if(glareCounter < 96) icons_economics->SetRect(icons_economics->Get(18));
                else if(glareCounter < 102) icons_economics->SetRect(icons_economics->Get(19));
                else if(glareCounter < 114) icons_economics->SetRect(icons_economics->Get(21));
                else if(glareCounter < 108) icons_economics->SetRect(icons_economics->Get(20));
                else if(glareCounter < 120) icons_economics->SetRect(icons_economics->Get(22));
                else if(glareCounter < 126) icons_economics->SetRect(icons_economics->Get(23));
                else if(glareCounter < 132) icons_economics->SetRect(icons_economics->Get(24));
                else if(glareCounter < 138) icons_economics->SetRect(icons_economics->Get(25));
                else if(glareCounter < 144) icons_economics->SetRect(icons_economics->Get(26));
                else if(glareCounter < 150) icons_economics->SetRect(icons_economics->Get(27));
                else if(glareCounter < 156) icons_economics->SetRect(icons_economics->Get(28));
                else if(glareCounter < 162) icons_economics->SetRect(icons_economics->Get(29));
                else if(glareCounter < 168) icons_economics->SetRect(icons_economics->Get(30));
                else if(glareCounter < 174) icons_economics->SetRect(icons_economics->Get(31));
                else if(glareCounter < 180) icons_economics->SetRect(icons_economics->Get(32));
                ui_economic_mission.Translate(0.4165, 0.277);
            }
            return *icons_economics;
        });

    ui_economic_free_build.Render(
        [&]() -> Texture& {
            if(ui_economic_free_build.IsMouseOver() && selected == ECO_NONE) {
				selected = ECO_FREEBUILD;
                RenderMenuText(L"Free Build");
                icons_economics->SetRect(icons_economics->Get(50));
                ui_economic_free_build.Translate(0.571, 0.277);
            } else {
                if(glareCounter < 181 || glareCounter >= 240) icons_economics->SetRect(icons_economics->Get(34));
                else if(glareCounter < 186) icons_economics->SetRect(icons_economics->Get(35));
                else if(glareCounter < 192) icons_economics->SetRect(icons_economics->Get(36));
                else if(glareCounter < 198) icons_economics->SetRect(icons_economics->Get(37));
                else if(glareCounter < 204) icons_economics->SetRect(icons_economics->Get(38));
                else if(glareCounter < 210) icons_economics->SetRect(icons_economics->Get(39));
                else if(glareCounter < 216) icons_economics->SetRect(icons_economics->Get(40));
                else if(glareCounter < 222) icons_economics->SetRect(icons_economics->Get(41));
                else if(glareCounter < 228) icons_economics->SetRect(icons_economics->Get(42));
                else if(glareCounter < 234) icons_economics->SetRect(icons_economics->Get(43));
                else if(glareCounter < 240) icons_economics->SetRect(icons_economics->Get(44));
                else if(glareCounter < 246) icons_economics->SetRect(icons_economics->Get(45));
                else if(glareCounter < 252) icons_economics->SetRect(icons_economics->Get(46));
                else if(glareCounter < 258) icons_economics->SetRect(icons_economics->Get(47));
                else if(glareCounter < 264) icons_economics->SetRect(icons_economics->Get(48));
                else if(glareCounter < 270) icons_economics->SetRect(icons_economics->Get(49));
                ui_economic_free_build.Translate(0.571, 0.277);
            }
            return *icons_economics;
        });
}


