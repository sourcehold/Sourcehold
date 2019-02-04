#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <iostream>

#include <Config.h>
#include <Logger.h>
#include <Rendering.h>

namespace OpenSH
{
    namespace Game
    {
        class Display
        {
                SDL_Event event;
                SDL_Window *window;
                SDL_Renderer *renderer;
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
                void Flush();
                void Clear();

                bool IsOpen();
                SDL_Renderer *GetRenderer();
                Rendering::Context CreateContext();
            protected:
        };
    }
}
