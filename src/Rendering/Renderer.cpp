#include <Rendering/Renderer.h>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Renderer::Renderer() : Camera(0, 0) {
}

Renderer::~Renderer() {
}

void Renderer::Init(SDL_Window *window) {
    this->window = window;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        Logger::error("GAME")  << "Unable to create SDL2 renderer: " << SDL_GetError() << std::endl;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Renderer::Update() {
    SDL_GetWindowSize(window, &width, &height);
}

void Renderer::Clear() {
    SDL_RenderClear(renderer);
}

void Renderer::Flush() {
    SDL_RenderPresent(renderer);
}

void Renderer::Render(Texture &texture, int x, int y, SDL_Rect *clip) {
    SDL_Rect rect = {
        x, y,
        texture.GetWidth(),
        texture.GetHeight()
    };

    if(clip) {
        rect.w = clip->w;
        rect.h = clip->h;
    }

    if(rect.x + rect.w > width || rect.y + rect.h > height) {
        
    };

    SDL_RenderCopyEx(
        renderer,
        texture.GetTexture(),
        clip,
        &rect,
        texture.GetAngle(),
        NULL,
        texture.GetFlip()
    );
}

void Renderer::Render(Texture &texture, int x, int y, int w, int h, SDL_Rect *clip) {
    /**
     * Source texture, specified by SDL_Rect, will
     * be stretched to fit the destination rect
     */
    SDL_Rect rect = {
        x, y,
        w, h
    };

    if(rect.x + rect.w > width || rect.y + rect.h > height) {
        
    };

    SDL_RenderCopyEx(
        renderer,
        texture.GetTexture(),
        clip,
        &rect,
        texture.GetAngle(),
        NULL,
        texture.GetFlip()
    );
}

void Renderer::Render(Texture &texture, double x, double y, SDL_Rect *clip) {
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    Render(texture, ix, iy, clip);
}

void Renderer::Render(Texture &texture, double x, double y, double w, double h, SDL_Rect *clip) {
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    int sx = ToCoordX(w);
    int sy = ToCoordY(h);
    Render(texture, ix, iy, sx, sy, clip);
}

void Renderer::Render(Texture &texture, SDL_Rect *clip) {
    SDL_RenderCopyEx(
        renderer,
        texture.GetTexture(),
        clip,
        NULL,
        texture.GetAngle(),
        NULL,
        texture.GetFlip()
    );
}

void Renderer::Render(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool solid) {
    SDL_SetRenderDrawColor(
        renderer,
        r, g, b, a
    );

    SDL_Rect rect = {
        x, y,
        w, h
    };

    if(rect.x + rect.w > width || rect.y + rect.h > height) {
        
    };
    
    if(solid) {
        SDL_RenderFillRect(
            renderer,
            &rect
        );
    }else {
        SDL_RenderDrawRect(
            renderer,
            &rect
        );
    }
}

void Renderer::Render(double x, double y, double w, double h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool solid) {
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    int sx = ToCoordX(w);
    int sy = ToCoordY(h);
    Render(ix, iy, sx, sy, r, g, b, a, solid);
}

double Renderer::NormalizeX(uint32_t c) {
    return (double)c / (double)GetWidth();
}

double Renderer::NormalizeY(uint32_t c) {
    return (double)c / (double)GetHeight();
}

uint32_t Renderer::ToCoordX(double c) {
    return (int)(c * GetWidth());
}

uint32_t Renderer::ToCoordY(double c) {
    return (int)(c * GetHeight());
}
