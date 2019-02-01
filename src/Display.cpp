#include <Display.h>

using namespace OpenSH::Game;
using namespace OpenSH::System;

Display::Display() {
    SDL_Init(SDL_INIT_EVERYTHING);
}

Display::~Display() {
    SDL_Quit();
}

void Display::OpenWindowed(std::string title, int width, int height) {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    if(!window) {
        Logger::LogToConsole("Unable to create SDL2 window!", Logger::ERROR);
        Logger::LogToConsole(SDL_GetError(), Logger::WARNING);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        Logger::LogToConsole("Unable to create SDL2 renderer!", Logger::ERROR);
        Logger::LogToConsole(SDL_GetError(), Logger::WARNING);
    }

    open = true;
}

void Display::Fullscreen() {
    int err = SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(err < 0) {
        Logger::LogToConsole("Unable to switch to fullscreen!", Logger::WARNING);
        Logger::LogToConsole(SDL_GetError(), Logger::WARNING);
    }
}

void Display::Close() {
    SDL_DestroyWindow(window);
}

void Display::Update() {
    SDL_RenderPresent(renderer);

    SDL_Delay(10);

    while(SDL_PollEvent(&event) != 0) {
        if(event.type == SDL_QUIT) {
            open = false;
        }
    }
}

void Display::Clear() {
    SDL_RenderClear(renderer);
}

bool Display::IsOpen() {
    return open;
}

SDL_Renderer *Display::GetRenderer() {
    return renderer;
}
