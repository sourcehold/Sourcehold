#include <GUI/Credits.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Credits::Credits(std::shared_ptr<GameManager> man) :
	EventConsumer<Mouse>(man->GetHandler())
{
	manager = man;
}

Credits::~Credits() {
}

bool Credits::Play(bool endgame, bool fadein, bool loop) {
	playing = true;

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

	music.Play();

	Uint8 alpha = 255;
	double startTime = manager->GetTime();
	double fadeBase = startTime;

	int iw = endgame ? 1024 : 800, ih = endgame ? 768 : 600;
	int px = (manager->GetWidth() / 2) - (iw / 2);
	int py = (manager->GetHeight() / 2) - (ih / 2);

	Resolution res = manager->GetResolution();
	while (manager->Running() && playing) {
		manager->Clear();

		double now = manager->GetTime();
		double delta = now - startTime;

		if(delta > 18.0) {
			currentImage = (currentImage + 1) % 4;
			startTime = now;
		}
		if(now < fadeBase + 2.0) {
		    alpha = Uint8(((now - fadeBase) * 255.0) / 2.0);
		}else if(now < fadeBase + 16.0) {
		    alpha = 255;
		}else if(now < fadeBase + 18.0) {
		    alpha = 255 - Uint8(((now - (fadeBase + 16.0)) * 255.0) / 2.0);
		}else if(now > fadeBase + 18.0) {
		    alpha = 0;
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

		manager->Flush();
		manager->Sync();
	}

	music.Stop();
	return true;
}

void Credits::onEventReceive(Mouse &event) {
    if(event.LmbDown() || event.RmbDown()) {
    	playing = false;
    }
}
