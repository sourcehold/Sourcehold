#include <Rendering/Display.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Display::Display() : Renderer() {
    SDL_Init(SDL_INIT_EVERYTHING);
}

Display::Display(const Display &dp) : Renderer(dp) {
    timer = dp.timer;
    frame = dp.frame;
    open = dp.open;
    Renderer::window = dp.window;
    Renderer::renderer = dp.renderer;
}

Display::~Display() {
    SDL_Quit();
}

void Display::OpenWindowed(std::string title, int width, int height) {
    Renderer::window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    if(!Renderer::window) {
        Logger::error("GAME") << "Unable to create SDL2 window: " << SDL_GetError() << std::endl;
    }

    Renderer::renderer = SDL_CreateRenderer(Renderer::window, -1, SDL_RENDERER_ACCELERATED);
    if(!Renderer::renderer) {
        Logger::error("GAME")  << "Unable to create SDL2 renderer: " << SDL_GetError() << std::endl;
    }

    SDL_SetRenderDrawBlendMode(Renderer::renderer, SDL_BLENDMODE_BLEND);

    open = true;
}

void Display::Fullscreen() {
    int err = SDL_SetWindowFullscreen(Renderer::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(err < 0) {
        Logger::error("GAME")  << "Unable to switch to fullscreen: " << SDL_GetError() << std::endl;
    }
}

void Display::Close() {
    SDL_DestroyWindow(Renderer::window);
}

void Display::HandleEvents() {
    while(SDL_PollEvent(&event) != 0) {
        if(event.type == SDL_QUIT) {
            open = false;
        }
    }
}

void Display::StartTimer() {
    timer = SDL_GetTicks();
}

void Display::EndTimer() {
    uint32_t ntimer = SDL_GetTicks();
    uint32_t delta = ntimer - timer;
    timer = ntimer;
    if(delta < 1000 / FRAMES_PER_SECOND) {
        SDL_Delay((1000 / FRAMES_PER_SECOND) - delta);
    }
}

void Display::Clear() {
    SDL_RenderClear(Renderer::renderer);
}

bool Display::IsOpen() {
    return open;
}

uint32_t Display::GetTicks() {
    return SDL_GetTicks();
}

uint32_t Display::GetSeconds() {
    return SDL_GetTicks() / 1000;
}
