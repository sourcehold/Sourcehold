#include <Rendering/Display.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

Display::Display() :
    Renderer()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        Logger::error("RENDERING") << SDL_GetError() << std::endl;
    }
}

Display::~Display() {
    SDL_Quit();
}

void Display::Open(const std::string &title, int width, int height, bool fullscreen, bool noborder) {
    int param = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS;

    this->fullscreen = fullscreen;
    if(fullscreen) {
        param |= SDL_WINDOW_FULLSCREEN;
    }
    
    if(noborder) {
        param |= SDL_WINDOW_BORDERLESS;
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, param);
    if(!window) {
        Logger::error("GAME") << "Unable to create SDL2 window: " << SDL_GetError() << std::endl;
    }

    Renderer::Init(window);

    open = true;
}

void Display::ToggleFullscreen() {
    fullscreen = !fullscreen;

    int err = SDL_SetWindowFullscreen(window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    if(err < 0) {
        Logger::error("GAME")  << "Unable to switch to fullscreen: " << SDL_GetError() << std::endl;
    }
}

void Display::Close() {
    SDL_DestroyWindow(window);
}

void Display::StartTimer() {
    timer = SDL_GetTicks();
}

void Display::EndTimer() {
    int32_t ntimer = SDL_GetTicks();
    int32_t delta = ntimer - timer;
    timer = ntimer;
    if(delta < 1000 / FRAMES_PER_SECOND) {
        SDL_Delay((1000 / FRAMES_PER_SECOND) - delta);
    }
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
