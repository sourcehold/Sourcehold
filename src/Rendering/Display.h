#pragma once

#include <SDL.h>

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#include <Events/Event.h>
#include <Events/Keyboard.h>

#include <System/Logger.h>

#include <Rendering/Renderer.h>

#include <Events/Event.h>
#include <Events/EventHandler.h>
#include <Events/Keyboard.h>

namespace Sourcehold
{
    namespace Rendering
    {
        using namespace Events;

		using namespace std::chrono;

        /**
         * Handles everyting related to the game display,
         * constructs and destroys the renderer
         */
        class Display : public Rendering::Renderer
        {
			const uint32_t TARGET_FPS = 60;
			const uint64_t TARGET_TIME_NS = 1000000000 / TARGET_FPS;
			uint32_t fps;
			high_resolution_clock::time_point tp;
            bool open = false, fullscreen = false, nograb = false;
            SDL_Window *window;
        public:
            Display();
            Display(const Display&) = delete;
            ~Display();

            void Open(const std::string &title, int width, int height, int index = 0, bool fullscreen = false, bool noborder = false, bool nograb = false);
            void ToggleFullscreen();
            void Sync();
            void GrabMouse();
            void ReleaseMouse();

            bool IsOpen();
            uint32_t GetTicks();
            uint32_t GetSeconds();
        };
    }
}
