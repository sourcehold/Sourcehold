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
	}else {
		music.LoadSong(manager->GetDirectory() / "fx/music/castlejam.raw", true);
	}

	music.Play();
	while (manager->Running() && playing) {
		manager->Clear();

		// TODO //

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
