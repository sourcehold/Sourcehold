#pragma once

#include <cinttypes>
#include <memory>

#include <GameManager.h>

#include <Events/Event.h>
#include <Events/Mouse.h>

#include <Rendering/Texture.h>
#include <Rendering/Font.h>

#include <Audio/AudioSource.h>
#include <Parsers/TgxFile.h>

namespace Sourcehold
{
    namespace GUI
    {
    	using namespace Game;
    	using namespace Audio;
    	using namespace Events;

    	/**
    	 * This handles the credits sequences,
    	 * the one in the main menu and the endgame credits.
    	 */
    	class Credits : protected EventConsumer<Mouse>
    	{
    	public:
    		Credits(std::shared_ptr<GameManager> man);
    		Credits(const Credits&) = delete;
    		~Credits();

    		bool Play(bool endgame, bool fadein, bool loop);
    	protected:
    		void onEventReceive(Mouse &event) override;

            uint8_t currentImage = 0;
    		bool playing = false;
    		std::shared_ptr<GameManager> manager;
    		AudioSource music;
    		std::shared_ptr<TgxFile> tgx_credits, tgx_1, tgx_2, tgx_3, tgx_4;
    	};
    }
}
