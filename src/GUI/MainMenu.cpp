#include <GUI/MainMenu.h>

#include <Rendering/Font.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
	manager(man),
	/* UI stuff */
	screen(man),
	ui_back_to_main(man),
	ui_exit(man),
	ui_firefly(man),
	ui_tutorial(man),
	ui_combat(man),
	ui_economic(man),
	ui_builder(man),
	ui_load(man),
	ui_settings(man),
	ui_combat_campaign(man),
	ui_combat_siege(man),
	ui_combat_invasion(man),
	ui_combat_multiplayer(man),
	ui_economic_campaign(man),
	ui_economic_mission(man),
	ui_economic_free_build(man),
	ui_builder_working_map(man),
	ui_builder_stand_alone_mission(man),
	ui_builder_siege_that(man),
	ui_builder_multiplayer_map(man)
{
	edition = manager->GetEdition();

	if (edition == STRONGHOLD_HD) {
		tgx_border = manager->GetTgx(manager->GetDirectory() / "gfx/SH1_Back.tgx").lock();
		screen.AllocNewTarget(1024, 768);
	}

	gm1_icons_additional = manager->GetGm1(manager->GetDirectory() / "gm/interface_buttons.gm1").lock();
	gm1_icons_main = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end.gm1").lock();
	gm1_icons_combat = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_combat.gm1").lock();
	gm1_icons_economic = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_economics.gm1").lock();
	gm1_icons_builder = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_builder.gm1").lock();

	tgx_bg_main = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_main.tgx").lock();
	tgx_bg_combat = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_combat.tgx").lock();
	tgx_bg_economic = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_economics.tgx").lock();
	tgx_bg_builder = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_builder.tgx").lock();

	aud_greetings.LoadEffect(manager->GetDirectory() / "fx/speech/General_Startgame.wav", false);
	aud_exit.LoadEffect(manager->GetDirectory() / "fx/speech/General_Quitgame.wav", false);

	/**
	* Render the border 'zoomed in' so that the
	* menu can be placed in the middle without scaling.
	*/
	border_rect.x = (1920 - manager->GetWidth()) / 2;
	border_rect.y = (1200 - manager->GetHeight()) / 2;
	border_rect.w = manager->GetWidth();
	border_rect.h = manager->GetHeight();

	mx = (manager->GetWidth() - 1024) / 2;
	my = (manager->GetHeight() - 768) / 2;
}

MainMenu::~MainMenu() {

}

UIState MainMenu::EnterMenu() {
	if(edition == STRONGHOLD_HD) {
		manager->SetTarget(&screen, mx, my, 1024, 768);		
	}

	/* Init the buttons */
	ui_exit.Translate(0.12, 0.67);
	ui_exit.Scale(0.17578125, 0.234375);

	ui_firefly.Translate(0.423, 0.774);
	ui_firefly.Scale(0.1337890625, 0.098958334);

	ui_tutorial.Translate(0.691, 0.687);
	ui_tutorial.Scale(0.17578125, 0.234375);

	ui_combat.Scale(0.17578125, 0.234375);
	ui_economic.Scale(0.17578125, 0.234375);
	ui_builder.Scale(0.17578125, 0.234375);
	ui_load.Scale(0.17578125, 0.234375);

	ui_settings.Scale(0.1, 0.16);
	ui_settings.Translate(0.726, 0.511);
	
    ui_combat_campaign.Scale(0.17578125, 0.234375);
    ui_combat_invasion.Scale(0.17578125, 0.234375);
    ui_combat_multiplayer.Scale(0.17578125, 0.234375);
    ui_combat_siege.Scale(0.17578125, 0.234375);

    ui_back_to_main.Scale(0.17578125, 0.234375);
    ui_back_to_main.Translate(0.12, 0.67);

    ui_economic_campaign.Scale(0.17578125, 0.234375);
    ui_economic_free_build.Scale(0.17578125, 0.234375);
    ui_economic_mission.Scale(0.17578125, 0.234375);

    ui_builder_multiplayer_map.Scale(0.17578125, 0.234375);
    ui_builder_siege_that.Scale(0.17578125, 0.234375);
    ui_builder_stand_alone_mission.Scale(0.17578125, 0.234375);
    ui_builder_working_map.Scale(0.17578125, 0.234375);

	manager->ResetTarget();

	ui_exit.Show();
	ui_firefly.Show();
	ui_tutorial.Show();
	ui_combat.Show();
	ui_economic.Show();
	ui_builder.Show();
	ui_load.Show();
	ui_settings.Show();

	UIState currentState = MAIN_MENU;
	glareCounter = 265;

	aud_greetings.Play();
	
	Resolution res = manager->GetResolution();
	while (manager->Running()) {
		manager->Clear();

		if (edition == STRONGHOLD_CLASSIC || res == RESOLUTION_800x600) {
			/* Don't render any border */
			manager->Render(*tgx_bg_main);
		}
		else if(edition == STRONGHOLD_HD) {
			manager->Render(*tgx_border, &border_rect);
			manager->SetTarget(&screen, mx, my, 1024, 768);
			manager->Render(*tgx_bg_main);
		}

		aud_greetings.Update();

		/* Update state */
		if (ui_exit.IsClicked()) {
			// TODO: exit prompt
			return EXIT_GAME;
		}
		else if (ui_combat.IsClicked()) {
			currentState = COMBAT_MENU;
		}
		else if (ui_economic.IsClicked()) {
			currentState = ECONOMICS_MENU;
		}
		else if (ui_builder.IsClicked()) {
			currentState = BUILDER_MENU;
		}
		else if (ui_load.IsClicked()) {
			currentState = LOAD_SAVED_MENU;
		}
		else if (ui_tutorial.IsClicked()) {
			currentState = TUTORIAL;
		}
		else if (ui_settings.IsClicked()) {
			currentState = SETTINGS_MENU;
		}
		else if (ui_firefly.IsClicked()) {
			currentState = CREDITS;
		}

		/* Render the current menu on top of the background */
		switch(currentState) {
			case MAIN_MENU: RenderMain(); break;
			case COMBAT_MENU: RenderCombat(); break;
			case ECONOMICS_MENU: RenderEconomic(); break;
			case BUILDER_MENU: RenderBuilder(); break;
			default: break;
		}

		manager->ResetTarget();
		manager->Render(screen, mx, my);

		// TODO: sync to fps
		glareCounter = (glareCounter + 1) % 360;

		manager->Flush();
		manager->Sync();
	}

	return EXIT_GAME;
}

void MainMenu::RenderMain() {
	/**
	* Render the buttons
	* TODO: Create a look-up table for the
	* glare indices. Too much fucking copy+paste!
	*/
	selected = NONE_SELECTED;

	auto icons_main = gm1_icons_main->GetTextureAtlas().lock();
	auto icons_additional = gm1_icons_additional->GetTextureAtlas().lock();
	ui_exit.Render(
		[&]() -> Texture& {
		if (ui_exit.IsMouseOver()) {
			selected = MAIN_EXIT;
			RenderMenuText(L"Exit Stronghold");
			icons_main->SetRect(icons_main->Get(69));
		}
		else icons_main->SetRect(icons_main->Get(68));
		return *icons_main;
	});

	ui_tutorial.Render(
		[&]() -> Texture& {
		if (ui_tutorial.IsMouseOver() && selected == NONE_SELECTED) {
			selected = MAIN_TUTORIAL;
			RenderMenuText(L"Tutorial");
			icons_main->SetRect(icons_main->Get(73));
		}
		else icons_main->SetRect(icons_main->Get(72));
		return *icons_main;
	});

	ui_combat.Render(
		[&]() -> Texture& {
		if (ui_combat.IsMouseOver() && selected == NONE_SELECTED) {
			selected = MAIN_COMBAT;
			RenderMenuText(L"Combat-based games");
			icons_main->SetRect(icons_main->Get(16));
			ui_combat.Translate(0.183, 0.276);
		}
		else {
			if (glareCounter >= 90) icons_main->SetRect(icons_main->Get(0));
			else if (glareCounter < 6) icons_main->SetRect(icons_main->Get(1));
			else if (glareCounter < 12) icons_main->SetRect(icons_main->Get(2));
			else if (glareCounter < 18) icons_main->SetRect(icons_main->Get(3));
			else if (glareCounter < 24) icons_main->SetRect(icons_main->Get(4));
			else if (glareCounter < 30) icons_main->SetRect(icons_main->Get(5));
			else if (glareCounter < 36) icons_main->SetRect(icons_main->Get(6));
			else if (glareCounter < 42) icons_main->SetRect(icons_main->Get(7));
			else if (glareCounter < 48) icons_main->SetRect(icons_main->Get(8));
			else if (glareCounter < 54) icons_main->SetRect(icons_main->Get(9));
			else if (glareCounter < 60) icons_main->SetRect(icons_main->Get(10));
			else if (glareCounter < 66) icons_main->SetRect(icons_main->Get(11));
			else if (glareCounter < 72) icons_main->SetRect(icons_main->Get(12));
			else if (glareCounter < 78) icons_main->SetRect(icons_main->Get(13));
			else if (glareCounter < 84) icons_main->SetRect(icons_main->Get(14));
			else if (glareCounter < 90) icons_main->SetRect(icons_main->Get(15));

			ui_combat.Translate(0.182, 0.277);
		}
		return *icons_main;
	});

	ui_economic.Render(
		[&]() -> Texture& {
		if (ui_economic.IsMouseOver() && selected == NONE_SELECTED) {
			selected = MAIN_ECONOMIC;
			RenderMenuText(L"Economic-based games");
			icons_main->SetRect(icons_main->Get(33));
			ui_economic.Translate(0.336, 0.276);
		}
		else {
			if (glareCounter < 91 || glareCounter >= 180) icons_main->SetRect(icons_main->Get(17));
			else if (glareCounter < 96) icons_main->SetRect(icons_main->Get(18));
			else if (glareCounter < 102) icons_main->SetRect(icons_main->Get(19));
			else if (glareCounter < 114) icons_main->SetRect(icons_main->Get(21));
			else if (glareCounter < 108) icons_main->SetRect(icons_main->Get(20));
			else if (glareCounter < 120) icons_main->SetRect(icons_main->Get(22));
			else if (glareCounter < 126) icons_main->SetRect(icons_main->Get(23));
			else if (glareCounter < 132) icons_main->SetRect(icons_main->Get(24));
			else if (glareCounter < 138) icons_main->SetRect(icons_main->Get(25));
			else if (glareCounter < 144) icons_main->SetRect(icons_main->Get(26));
			else if (glareCounter < 150) icons_main->SetRect(icons_main->Get(27));
			else if (glareCounter < 156) icons_main->SetRect(icons_main->Get(28));
			else if (glareCounter < 162) icons_main->SetRect(icons_main->Get(29));
			else if (glareCounter < 168) icons_main->SetRect(icons_main->Get(30));
			else if (glareCounter < 174) icons_main->SetRect(icons_main->Get(31));
			else if (glareCounter < 180) icons_main->SetRect(icons_main->Get(32));
			ui_economic.Translate(0.337, 0.275);
		}
		return *icons_main;
	});

	ui_builder.Render(
		[&]() -> Texture& {
		if (ui_builder.IsMouseOver() && selected == NONE_SELECTED) {
			selected = MAIN_BUILDER;
			RenderMenuText(L"Map Editor");
			icons_main->SetRect(icons_main->Get(50));
			ui_builder.Translate(0.492, 0.276);
		}
		else {
			if (glareCounter < 181 || glareCounter >= 240) icons_main->SetRect(icons_main->Get(34));
			else if (glareCounter < 186) icons_main->SetRect(icons_main->Get(35));
			else if (glareCounter < 192) icons_main->SetRect(icons_main->Get(36));
			else if (glareCounter < 198) icons_main->SetRect(icons_main->Get(37));
			else if (glareCounter < 204) icons_main->SetRect(icons_main->Get(38));
			else if (glareCounter < 210) icons_main->SetRect(icons_main->Get(39));
			else if (glareCounter < 216) icons_main->SetRect(icons_main->Get(40));
			else if (glareCounter < 222) icons_main->SetRect(icons_main->Get(41));
			else if (glareCounter < 228) icons_main->SetRect(icons_main->Get(42));
			else if (glareCounter < 234) icons_main->SetRect(icons_main->Get(43));
			else if (glareCounter < 240) icons_main->SetRect(icons_main->Get(44));
			else if (glareCounter < 246) icons_main->SetRect(icons_main->Get(45));
			else if (glareCounter < 252) icons_main->SetRect(icons_main->Get(46));
			else if (glareCounter < 258) icons_main->SetRect(icons_main->Get(47));
			else if (glareCounter < 264) icons_main->SetRect(icons_main->Get(48));
			else if (glareCounter < 270) icons_main->SetRect(icons_main->Get(49));

			ui_builder.Translate(0.492, 0.275);
		}
		return *icons_main;
	});

	ui_load.Render(
		[&]() -> Texture& {
		if (ui_load.IsMouseOver() && selected == NONE_SELECTED) {
			selected = MAIN_LOAD;
			RenderMenuText(L"Load a Saved Game");
			icons_main->SetRect(icons_main->Get(67));
			ui_load.Translate(0.648, 0.278);
		}
		else {
			if (glareCounter < 271) icons_main->SetRect(icons_main->Get(51));
			else if (glareCounter < 276) icons_main->SetRect(icons_main->Get(52));
			else if (glareCounter < 282) icons_main->SetRect(icons_main->Get(53));
			else if (glareCounter < 288) icons_main->SetRect(icons_main->Get(54));
			else if (glareCounter < 294) icons_main->SetRect(icons_main->Get(55));
			else if (glareCounter < 300) icons_main->SetRect(icons_main->Get(56));
			else if (glareCounter < 306) icons_main->SetRect(icons_main->Get(57));
			else if (glareCounter < 312) icons_main->SetRect(icons_main->Get(58));
			else if (glareCounter < 318) icons_main->SetRect(icons_main->Get(59));
			else if (glareCounter < 324) icons_main->SetRect(icons_main->Get(60));
			else if (glareCounter < 330) icons_main->SetRect(icons_main->Get(61));
			else if (glareCounter < 336) icons_main->SetRect(icons_main->Get(62));
			else if (glareCounter < 342) icons_main->SetRect(icons_main->Get(63));
			else if (glareCounter < 348) icons_main->SetRect(icons_main->Get(64));
			else if (glareCounter < 354) icons_main->SetRect(icons_main->Get(65));
			else if (glareCounter < 360) icons_main->SetRect(icons_main->Get(66));

			ui_load.Translate(0.648, 0.276);
		}
		return *icons_main;
	});

	ui_firefly.Render(
		[&]() -> Texture& {
		if (ui_firefly.IsMouseOver() && selected == NONE_SELECTED) {
			selected = MAIN_FIREFLY;
			RenderMenuText(L"Credits");
			icons_additional->SetRect(icons_additional->Get(78));
		}
		else {
			icons_additional->SetRect(icons_additional->Get(77));
		}
		return *icons_additional;
	});

	ui_settings.Render(
		[&]() -> Texture& {
		if (ui_settings.IsMouseOver() && selected == NONE_SELECTED) {
			selected = MAIN_SETTINGS;
			RenderMenuText(L"Options");
			icons_additional->SetRect(icons_additional->Get(89));
		}
		else {
			icons_additional->SetRect(icons_additional->Get(88));
		}
		return *icons_additional;
	});
}

void MainMenu::RenderCombat() {
	selected = NONE_SELECTED;

    manager->Render(*tgx_bg_combat);
    auto icons_combat = gm1_icons_combat->GetTextureAtlas().lock();
    ui_back_to_main.Render(
        [&]() -> Texture& {
            if(ui_back_to_main.IsMouseOver()){
				selected = COMBAT_BACK_TO_MAIN;
                RenderMenuText(L"Back to Main Menu");
                icons_combat->SetRect(icons_combat->Get(69));
            }
            else icons_combat->SetRect(icons_combat->Get(68));
            return *icons_combat;
        });

    ui_combat_campaign.Render(
        [&]() -> Texture& {
            if(ui_combat_campaign.IsMouseOver() && selected == NONE_SELECTED) {
				selected = COMBAT_CAMPAIGN;
                RenderMenuText(L"Play the Military Campaign");
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
            if(ui_combat_siege.IsMouseOver() && selected == NONE_SELECTED) {
				selected = COMBAT_SIEGE;
                RenderMenuText(L"Play a Siege");
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
            if(ui_combat_invasion.IsMouseOver() && selected == NONE_SELECTED) {
				selected = COMBAT_INVASION;
                RenderMenuText(L"Play an Invasion");
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
            if(ui_combat_multiplayer.IsMouseOver() && selected == NONE_SELECTED) {
				selected = COMBAT_MULTIPLAYER;
                RenderMenuText(L"Play a Multiplayer Game");
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

void MainMenu::RenderEconomic() {
	selected = NONE_SELECTED;

    manager->Render(*tgx_bg_economic);
    auto icons_economics = gm1_icons_economic->GetTextureAtlas().lock();
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
            if(ui_economic_campaign.IsMouseOver() && selected == NONE_SELECTED) {
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
            if(ui_economic_mission.IsMouseOver() && selected == NONE_SELECTED) {
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
            if(ui_economic_free_build.IsMouseOver() && selected == NONE_SELECTED) {
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

void MainMenu::RenderBuilder() {
	selected = NONE_SELECTED;

    manager->Render(*tgx_bg_builder);
    auto icons_builder = gm1_icons_builder->GetTextureAtlas().lock();
    ui_back_to_main.Render(
        [&]() -> Texture& {
            if(ui_back_to_main.IsMouseOver()) {
				selected = BUILDER_BACK_TO_MAIN;
                RenderMenuText(L"Back to Main Menu");
                icons_builder->SetRect(icons_builder->Get(71));
            }
            else icons_builder->SetRect(icons_builder->Get(70));
            return *icons_builder;
        });

	uint32_t glareIndex = glareCounter % 16;

    ui_builder_working_map.Render(
        [&]() -> Texture& {
            if(ui_builder_working_map.IsMouseOver() && selected == NONE_SELECTED) {
				selected = BUILDER_WORKING_MAP;
                RenderMenuText(L"Load a Working Map");
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
            if(ui_builder_stand_alone_mission.IsMouseOver() && selected == NONE_SELECTED) {
				selected = BUILDER_STANDALONE;
                RenderMenuText(L"New Stand-Alone Mission");
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
            if(ui_builder_siege_that.IsMouseOver() && selected == NONE_SELECTED) {
				selected = BUILDER_SIEGE;
                RenderMenuText(L"New 'Siege That' Mission");
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
            if(ui_builder_multiplayer_map.IsMouseOver() && selected == NONE_SELECTED) {
				selected = BUILDER_MULTIPLAYER;
                RenderMenuText(L"New Multiplayer Map");
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
