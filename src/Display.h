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
                bool open = false;
            public:
                Display();
                ~Display();

                void OpenWindowed(std::string title, int width, int height);
                void Fullscreen();
                void Close();
                void Update();
                void Clear();

                bool IsOpen();
                SDL_Renderer *GetRenderer();
                Rendering::Context CreateContext();
            protected:
        };
    }
}
