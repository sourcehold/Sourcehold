#include <Rendering/Renderer.h>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Renderer::Renderer() : Camera(0, 0) {
}

Renderer::Renderer(const Renderer &rend) {
    window = rend.window;
    renderer = rend.renderer;
}

Renderer::~Renderer() {
}

void Renderer::Init() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Renderer::Flush() {
    SDL_RenderPresent(renderer);
}

void Renderer::Render(Texture &texture, int x, int y) {
    SDL_Rect rect = {
        x, y,
        texture.GetWidth(),
        texture.GetHeight()
    };
    SDL_RenderCopyEx(
        renderer,
        texture.GetTexture(),
        NULL,
        &rect,
        texture.GetAngle(),
        NULL,
        texture.GetFlip()
    );
}

void Renderer::Render(Texture &texture, int x, int y, int w, int h) {
    SDL_Rect rect = {
        x, y,
        w,
        h
    };
    SDL_RenderCopyEx(
        renderer,
        texture.GetTexture(),
        NULL,
        &rect,
        texture.GetAngle(),
        NULL,
        texture.GetFlip()
    );
}

void Renderer::Render(Texture &texture, double x, double y) {
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    Render(texture, ix, iy);
}

void Renderer::Render(Texture &texture, double x, double y, double w, double h) {
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    int sx = ToCoordX(w);
    int sy = ToCoordY(h);
    Render(texture, ix, iy, sx, sy);
}

void Renderer::Render(Texture &texture) {
    SDL_RenderCopyEx(
        renderer,
        texture.GetTexture(),
        NULL,
        NULL,
        texture.GetAngle(),
        NULL,
        texture.GetFlip()
    );
}

uint32_t Renderer::GetWidth() {
    int s;
    SDL_GetWindowSize(window, &s, NULL);
    return s;
}

uint32_t Renderer::GetHeight() {
    int s;
    SDL_GetWindowSize(window, NULL, &s);
    return s;
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
