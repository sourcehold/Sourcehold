#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <iostream>
#include <vector>
#include <memory>

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

        /*
         * Handles everyting related to the game display,
         * owns the renderer
         */
        class Display : public Rendering::Renderer
        {
                const int FRAMES_PER_SECOND = 60;
                uint32_t timer = 0;
                int frame = 0;
                bool open = false, fullscreen = false;
            public:
                Display();
                Display(const Display &dp);
                ~Display();

                void Open(const std::string &title, int width, int height, bool fullscreen = false, bool noborder = false);
                void ToggleFullscreen();
                void Close();
                void StartTimer();
                void EndTimer();
                void Clear();

                bool IsOpen();
                uint32_t GetTicks();
                uint32_t GetSeconds();
            protected:
        };
    }
}
