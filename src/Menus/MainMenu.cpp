#include <Menus/MainMenu.h>

#include <Rendering/Font.h>

using namespace Sourcehold::Menus;
using namespace Sourcehold::Rendering;

MainMenu::MainMenu(std::shared_ptr<GameManager> man) :
	manager(man),
	/* UI stuff */
	screen(man),
	ui_exit(man),
	ui_firefly(man),
	ui_tutorial(man),
	ui_combat(man),
	ui_economic(man),
	ui_builder(man),
	ui_load(man),
	ui_settings(man)
{
	edition = manager->GetEdition();

	if (edition == STRONGHOLD_HD) {
		tgx_border = manager->GetTgx(manager->GetDirectory() / "gfx/SH1_Back.tgx").lock();
		screen.AllocNewTarget(1024, 768);
	}

	gm1_icons_additional = manager->GetGm1(manager->GetDirectory() / "gm/interface_buttons.gm1").lock();
	gm1_icons_main = manager->GetGm1(manager->GetDirectory() / "gm/icons_front_end.gm1").lock();
	tgx_bg1 = manager->GetTgx(manager->GetDirectory() / "gfx/frontend_main.tgx").lock();

	aud_greetings.LoadEffect(manager->GetDirectory() / "fx/speech/General_Startgame.wav", false);
	aud_exit.LoadEffect(manager->GetDirectory() / "fx/speech/General_Quitgame.wav", false);
}

MainMenu::~MainMenu() {

}

UIState MainMenu::EnterMenu() {
	int mx = (manager->GetWidth() / 2) - (1024 / 2);
	int my = (manager->GetHeight() / 2) - (768 / 2);

	manager->SetTarget(&screen, mx, my, 1024, 768);

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

	ui_settings.Scale(0.1, 0.16);
	ui_settings.Translate(0.726, 0.511);
	
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
	int32_t glareCounter = 265;

	aud_greetings.Play();
	
	Resolution res = manager->GetResolution();
	while (manager->Running()) {
		manager->Clear();

		if (edition == STRONGHOLD_CLASSIC || res == RESOLUTION_800x600) {
			/* Don't render any border */
			manager->Render(*tgx_bg1);
			Draw(glareCounter);
		}
		else if(edition == STRONGHOLD_HD) {
			/**
			* Render the border 'zoomed in' so that the
			* menu can be placed in the middle without scaling.
			*/
			SDL_Rect rect;
			rect.x = (1920 - manager->GetWidth()) / 2;
			rect.y = (1200 - manager->GetHeight()) / 2;
			rect.w = manager->GetWidth();
			rect.h = manager->GetHeight();

			manager->Render(*tgx_border, &rect);
		
			manager->SetTarget(&screen, mx, my, 1024, 768);
			manager->Render(*tgx_bg1);
			Draw(glareCounter);
			manager->ResetTarget();

			manager->Render(screen, mx, my);
		}

		aud_greetings.Update();

		if (ui_exit.IsClicked()) {
			aud_exit.Play();
			// TODO: Exit promt //
			currentState = EXIT_GAME;
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

		}
		else if (ui_settings.IsClicked()) {
			currentState = SETTINGS_MENU;
		}
		else if (ui_firefly.IsClicked()) {

		}

		if (currentState != MAIN_MENU) {
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

		glareCounter = (glareCounter + 1) % 360;

		manager->Flush();
		manager->Sync();
	}

	return EXIT_GAME;
}

void MainMenu::Draw(const int32_t glareCounter) {
	/* Render user interface */
	selected = MAIN_NONE;

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
		if (ui_tutorial.IsMouseOver() && selected == MAIN_NONE) {
			selected = MAIN_TUTORIAL;
			RenderMenuText(L"Tutorial");
			icons_main->SetRect(icons_main->Get(73));
		}
		else icons_main->SetRect(icons_main->Get(72));
		return *icons_main;
	});

	ui_combat.Render(
		[&]() -> Texture& {
		if (ui_combat.IsMouseOver() && selected == MAIN_NONE) {
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
		if (ui_economic.IsMouseOver() && selected == MAIN_NONE) {
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
		if (ui_builder.IsMouseOver() && selected == MAIN_NONE) {
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
		if (ui_load.IsMouseOver() && selected == MAIN_NONE) {
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
		if (ui_firefly.IsMouseOver() && selected == MAIN_NONE) {
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
		if (ui_settings.IsMouseOver() && selected == MAIN_NONE) {
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
