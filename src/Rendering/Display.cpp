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

Display::Display(const Display &dp) :
    Renderer(dp)
{
    timer = dp.timer;
    frame = dp.frame;
    open = dp.open;
    Renderer::window = dp.window;
    Renderer::renderer = dp.renderer;
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

    Renderer::window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, param);
    if(!Renderer::window) {
        Logger::error("GAME") << "Unable to create SDL2 window: " << SDL_GetError() << std::endl;
    }

    Renderer::renderer = SDL_CreateRenderer(Renderer::window, -1, SDL_RENDERER_ACCELERATED);
    if(!Renderer::renderer) {
        Logger::error("GAME")  << "Unable to create SDL2 renderer: " << SDL_GetError() << std::endl;
    }
    Renderer::Init();

    open = true;
}

void Display::ToggleFullscreen() {
    fullscreen = !fullscreen;

    int err = SDL_SetWindowFullscreen(Renderer::window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    if(err < 0) {
        Logger::error("GAME")  << "Unable to switch to fullscreen: " << SDL_GetError() << std::endl;
    }
}

void Display::Close() {
    SDL_DestroyWindow(Renderer::window);
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
