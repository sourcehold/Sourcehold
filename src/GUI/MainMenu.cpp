#include <GUI/MainMenu.h>
#include <Rendering/Font.h>
#include <Assets.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
	manager(man),
	EventConsumer<Mouse>(man->GetHandler()),
	cred(man),
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

    aud_chantloop.LoadSong(manager->GetDirectory() / "fx/music/chantloop1.raw", true);

	gm1_icons_additional = manager->GetGm1(manager->GetDirectory() / "gm/interface_buttons.gm1").lock();
	gm1_icons_main = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end.gm1").lock();
	gm1_icons_combat = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_combat.gm1").lock();
	gm1_icons_economic = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_economics.gm1").lock();
	gm1_icons_builder = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end_builder.gm1").lock();

	tgx_bg_main = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_main.tgx").lock();
	tgx_bg_combat = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_combat.tgx").lock();
	tgx_bg_economic = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_economics.tgx").lock();
	tgx_bg_builder = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_builder.tgx").lock();

	aud_greetings.LoadEffect(GetGreetingsSound(), false);
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

	currentState = MAIN_MENU;

	aud_chantloop.Play();
	//aud_greetings.Play();
	
	while (manager->Running()) {
		manager->Clear();

		RenderBorder();

		//aud_greetings.Update();

		/* Update state */
		if (ui_exit.IsClicked()) {
			// TODO: exit prompt
			aud_chantloop.Stop();
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

		glareCounter = (int)(manager->GetTime() * 10.0);

		manager->Flush();
		manager->Sync();
	}

	aud_chantloop.Stop();
	return EXIT_GAME;
}

void MainMenu::onEventReceive(Mouse &event) {
    if(event.LmbDown()) {
    	if(selected == COMBAT_BACK_TO_MAIN || selected == ECO_BACK_TO_MAIN || selected == BUILDER_BACK_TO_MAIN) {
    		currentState = MAIN_MENU;
    	}else if(selected == MAIN_FIREFLY) {
    		/* Prevent unlimited spawning of credits  */
    		selected = NONE_SELECTED;

    		/* Fade out chantloop, screen -> black */
    		Uint8 alpha;
    		double fadeBase, now;
    		now = fadeBase = manager->GetTime();
    		aud_chantloop.SetFadeOut(1.0);
    		while(aud_chantloop.IsFading()) {
    			//manager->Clear();

    			alpha = 255 - Uint8(((now - (fadeBase + 1.0)) * 255.0) / 1.0);
    			//screen.SetAlphaMod(alpha);
    			//tgx_border->SetAlphaMod(alpha);
    			aud_chantloop.UpdateFade();

    			//RenderBorder();
    			//manager->Render(screen, mx, my);

    			manager->Sync();
    			manager->Flush();
    		}

    		/* Start credits loop */
    		cred.Play(false, true, true);
    		aud_chantloop.SetGain(1.0);
    		aud_chantloop.Resume();
    	}
    }
}

boost::filesystem::path MainMenu::GetGreetingsSound() {
	boost::filesystem::path snd = manager->GetDirectory() / "fx/speech/";
	int index = manager->GetUsernameIndex();
	if(index == -1) {
		snd /= "General_Startgame.wav";
	}else {
		snd /= "name" + std::to_string(index + 1) + ".wav";
	}

	return snd;
}

void MainMenu::RenderBorder() {
	Resolution res = manager->GetResolution();
	if (edition == STRONGHOLD_CLASSIC || res == RESOLUTION_800x600) {
		/* Don't render any border */
		manager->Render(*tgx_bg_main);
	}
	else if(edition == STRONGHOLD_HD) {
		manager->Render(*tgx_border, &border_rect);
		manager->SetTarget(&screen, mx, my, 1024, 768);
		manager->Render(*tgx_bg_main);
	}
}

void MainMenu::RenderMain() {
	/* Render the buttons */
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
			icons_main->SetRect(icons_main->Get(1 + (glareCounter % 14)));
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
			icons_main->SetRect(icons_main->Get(18 + (glareCounter % 14)));
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
			icons_main->SetRect(icons_main->Get(35 + (glareCounter % 14)));
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
			icons_main->SetRect(icons_main->Get(52 + (glareCounter % 14)));
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
            if(ui_back_to_main.IsMouseOver()) {
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
                icons_combat->SetRect(icons_combat->Get(16));
                ui_combat_campaign.Translate(0.183, 0.276);
            } else {
				icons_combat->SetRect(icons_combat->Get(1 + (glareCounter % 14)));
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
            	icons_combat->SetRect(icons_combat->Get(18 + (glareCounter % 14)));
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
            	icons_combat->SetRect(icons_combat->Get(35 + (glareCounter % 14)));
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
            	icons_combat->SetRect(icons_combat->Get(52 + (glareCounter % 14)));
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
            	icons_economics->SetRect(icons_economics->Get(1 + (glareCounter % 14)));
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
            	icons_economics->SetRect(icons_economics->Get(18 + (glareCounter % 14)));
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
            	icons_economics->SetRect(icons_economics->Get(35 + (glareCounter % 14)));
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

    ui_builder_working_map.Render(
        [&]() -> Texture& {
            if(ui_builder_working_map.IsMouseOver() && selected == NONE_SELECTED) {
				selected = BUILDER_WORKING_MAP;
                RenderMenuText(L"Load a Working Map");
                icons_builder->SetRect(icons_builder->Get(16));
                ui_builder_working_map.Translate(0.183, 0.276);
            } else {
            	icons_builder->SetRect(icons_builder->Get(1 + (glareCounter % 14)));
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
            	icons_builder->SetRect(icons_builder->Get(18 + (glareCounter % 14)));
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
            	icons_builder->SetRect(icons_builder->Get(35 + (glareCounter % 35)));
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
            	icons_builder->SetRect(icons_builder->Get(52 + (glareCounter % 14)));
                ui_builder_multiplayer_map.Translate(0.648, 0.276);
            }
            return *icons_builder;
        });
}
