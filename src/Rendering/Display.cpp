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
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::Open(const std::string &title, int width, int height, int index, bool fullscreen, bool noborder, bool nograb) {
    int param = /*SDL_WINDOW_RESIZABLE |*/ SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS;

    this->fullscreen = fullscreen;
    if(fullscreen) {
        param |= SDL_WINDOW_FULLSCREEN;
    }

    if(noborder) {
        param |= SDL_WINDOW_BORDERLESS;
    }

    /* Select display */
    int displays = SDL_GetNumVideoDisplays();
    if(index >= displays) index = 0;

    SDL_Rect *displayBounds = new SDL_Rect[displays];
    for(int i = 0; i < displays; i++) {
        displayBounds[i] = { 0, 0, 0, 0 };
        SDL_GetDisplayBounds(i, &displayBounds[i]);
    }

    window = SDL_CreateWindow(
        title.c_str(),
        displayBounds[index].x + (displayBounds[index].w / 2) - width / 2,
        displayBounds[index].y + (displayBounds[index].h / 2) - height / 2,
        width,
        height,
        param
    );
    if(!window) {
        Logger::error("GAME") << "Unable to create SDL2 window: " << SDL_GetError() << std::endl;
    }

    delete [] displayBounds;

    this->nograb = nograb;
    if(!nograb) {
        SDL_SetWindowGrab(window, SDL_TRUE);
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

void Display::Sync() {
	/* TODO */
	high_resolution_clock::time_point now = high_resolution_clock::now();

	Uint64 delta = std::chrono::duration_cast<std::chrono::nanoseconds>(now - tp).count();

	std::this_thread::sleep_for(
		std::chrono::milliseconds(10)
	);
}

void Display::GrabMouse() {
    if(!nograb && SDL_GetWindowGrab(window) == SDL_FALSE) {
        SDL_SetWindowGrab(window, SDL_TRUE);
    }
}

void Display::ReleaseMouse() {
    if(SDL_GetWindowGrab(window) == SDL_TRUE) {
        SDL_SetWindowGrab(window, SDL_FALSE);
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
