#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <iostream>

#include <Config.h>
#include <System/Logger.h>
#include <Rendering/Renderer.h>

namespace OpenSH
{
    namespace Rendering
    {
        class Display : public Rendering::Renderer
        {
                SDL_Event event;
                const int FRAMES_PER_SECOND = 30;
                uint32_t timer = 0;
                int frame = 0;
                bool open = false;
            public:
                Display();
                ~Display();

                void OpenWindowed(std::string title, int width, int height);
                void Fullscreen();
                void Close();
                void HandleEvents();
                void StartTimer();
                void EndTimer();
                void Clear();

                bool IsOpen();
            protected:
        };
    }
}
