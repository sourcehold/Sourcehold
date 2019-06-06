#include <GUI/Credits.h>
#include <Rendering/Font.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

/* Not translated, thus hardcoded */
const static wchar_t *credit_lines_firefly[] = {
	L"Game Design",
	L"Simon Bradbury",
	L"",
	L"Additional Design",
	L"Eric Oulette",
	L"",
	L"Game Programming",
	L"Simon Bradbury",
	L"Andrew Prime",
	L"Andrew McNab",
	L"",
	L"Executive Producer",
	L"Eric Oulette",
	L"",
	L"Art Director",
	L"Michael Best",
	L"",
	L"Artists",
	L"Jorge Cameo",
	L"Robert Thornley",
	L"Darren White",
	L"",
	L"Multiplayer Programming",
	L"Andrew Prime",
	L"",
	L"Sound Design and Original",
	L"Music composed by",
	L"Robert L.Euvino",
	L"",
	L"Manual and Story Writer",
	L"Casimir C. Windsor",
	L"",
	L"Quality Assurance Manager",
	L"Darren Thompson",
	L"",
	L"Firefly Testing",
	L"Phil Busuttil",
	L"Casimir C. Windsor",
	L"",
	L"Mandolin",
	L"Matthew Finck",
	L"",
	L"Voice Actors",
	L"Ray Greenoaken",
	L"Jerry Kersey",
	L"James Lawson",
	L"Anthony Mulligan",
	L"John Tearney",
	L"",
	L"Additional Vocals",
	L"Lydia Pidlusky",
	L"",
	L"Scenario Creatorss",
	L"Phil Busuttil",
	L"Eric Oulette",
	L"Darren Thompson",
	L"",
	L"Special Thanks To",
	L"Davis Lester",
};

Credits::Credits(std::shared_ptr<GameManager> man) :
	EventConsumer<Mouse>(man->GetHandler())
{
	manager = man;
}

Credits::~Credits() {
}

bool Credits::Play(bool endgame, bool fadein, bool loop) {
	playing = true;
	uint8_t currentImage = 0;

 	AudioSource music;
    std::shared_ptr<TgxFile> tgx_credits, tgx_1, tgx_2, tgx_3, tgx_4, tgx_firefly;

	if(endgame) {
		music.LoadSong(manager->GetDirectory() / "fx/music/Glory_06.raw", true);
		tgx_credits = manager->GetTgx(manager->GetDirectory() / "gfx/end_credit.tgx").lock();
	}else {
		music.LoadSong(manager->GetDirectory() / "fx/music/castlejam.raw", true);
		tgx_1 = manager->GetTgx(manager->GetDirectory() / "gfx/credits_1.tgx").lock();
		tgx_2 = manager->GetTgx(manager->GetDirectory() / "gfx/credits_2.tgx").lock();
		tgx_3 = manager->GetTgx(manager->GetDirectory() / "gfx/credits_3.tgx").lock();
		tgx_4 = manager->GetTgx(manager->GetDirectory() / "gfx/credits_4.tgx").lock();
	}

	tgx_firefly = manager->GetTgx(manager->GetDirectory() / "gfx/front_firefly_logo.tgx").lock();

	music.Play();

	Uint8 alpha = 255;
	double startTime = manager->GetTime();
	double fadeBase = startTime;

	int iw = endgame ? 1024 : 800, ih = endgame ? 768 : 600;
	int px = (manager->GetWidth() / 2) - (iw / 2);
	int py = (manager->GetHeight() / 2) - (ih / 2);

	int sx = (manager->GetWidth() / 2) + (200 / 2);
	int scrollOffset = 0;

	manager->MouseOff();

	Resolution res = manager->GetResolution();
	while (manager->Running() && playing) {
		manager->Clear();

		double now = manager->GetTime();
		double delta = now - fadeBase;

		if(now < fadeBase + 2.0) {
		    alpha = Uint8(((now - fadeBase) * 255.0) / 2.0);
		}else if(now < fadeBase + 16.0) {
		    alpha = 255;
		}else if(now < fadeBase + 18.0) {
		    alpha = 255 - Uint8(((now - (fadeBase + 16.0)) * 255.0) / 2.0);
		}else if(now > fadeBase + 18.0) {
		    alpha = 0;
		    currentImage = (currentImage + 1) % 4;
		    fadeBase = now;
		}

		if(endgame) {
			if(res == RESOLUTION_800x600) {
				/* Scale down */
				manager->Render(*tgx_credits);
			}else {
				/* Place in the middle */
				manager->Render(*tgx_credits, px, py);
			}
		}else {
			switch(currentImage) {
				case 0: {
					tgx_1->SetAlphaMod(alpha);
					manager->Render(*tgx_1, px, py);
				} break;
				case 1: {
					tgx_2->SetAlphaMod(alpha);
					manager->Render(*tgx_2, px, py);
				} break;
				case 2: {
					tgx_3->SetAlphaMod(alpha);
					manager->Render(*tgx_3, px, py);
				} break;
				case 3: {
					tgx_4->SetAlphaMod(alpha);
					manager->Render(*tgx_4, px, py);
				} break;
				default: break;
			}
		}

		/* Render the scroller */
		int sy = manager->GetHeight() - scrollOffset;
		scrollOffset = int((now - startTime) * 80.0);

		/* Firefly Studios */
		manager->Render(*tgx_firefly, sx, sy);
		sy += 122 + 30;

		for(int i = 0; i < sizeof(credit_lines_firefly) / sizeof(credit_lines_firefly[0]); i++, sy += 46) {
			const wchar_t *line = credit_lines_firefly[i];
			RenderText(line, sx, sy, 1, FONT_LARGE);
		}

		// TODO: Rest of credits, colored titles

		manager->Flush();
		manager->Sync();
	}

	manager->MouseOn();
	music.Stop();

	return true;
}

void Credits::onEventReceive(Mouse &event) {
    if(event.LmbDown() || event.RmbDown()) {
    	playing = false;
    }
}
