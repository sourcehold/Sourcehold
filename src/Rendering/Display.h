#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <iostream>

#include <Config.h>

#include <System/Logger.h>

#include <Rendering/Renderer.h>

#include <Events/Event.h>
#include <Events/Keyboard.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class Display :
            public Rendering::Renderer,
            public Events::Keyboard
        {
                SDL_Event event;
                const int FRAMES_PER_SECOND = 60;
                uint32_t timer = 0;
                int frame = 0;
                bool open = false;
            public:
                Display();
                Display(const Display &dp);
                ~Display();

                void Open(std::string title, int width, int height, bool fullscreen = false);
                void Fullscreen();
                void Close();
                void HandleEvents();
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
