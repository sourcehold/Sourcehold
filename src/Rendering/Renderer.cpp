#include <Rendering/Renderer.h>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Renderer::Renderer() {

}

Renderer::Renderer(const Renderer &rend) {
    window = rend.window;
    renderer = rend.renderer;
}

Renderer::~Renderer() {
    
}

void Renderer::Flush() {
    SDL_RenderPresent(renderer);
}

void Renderer::RenderTexture(Texture &texture, int x, int y) {    
    SDL_Rect rect = {
        x, y,
        texture.GetWidth(),
        texture.GetHeight()
    };
    SDL_RenderCopy(
        renderer,
        texture.GetTexture(),
        NULL,
        &rect
    );
}

void Renderer::RenderTextureScale(Texture &texture, int x, int y, int w, int h) {
    SDL_Rect rect = {
        x, y,
        w,
        h
    };
    SDL_RenderCopy(
        renderer,
        texture.GetTexture(),
        NULL,
        &rect
    );
}

void Renderer::RenderTexture(Texture &texture, double x, double y) {
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    RenderTexture(texture, ix, iy);
}

void Renderer::RenderTextureScale(Texture &texture, double x, double y, double w, double h) {
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    int sx = ToCoordX(w);
    int sy = ToCoordY(h);
    RenderTextureScale(texture, ix, iy, sx, sy);
}

void Renderer::RenderTextureFullscreen(Texture &texture) {
    SDL_RenderCopy(
        renderer,
        texture.GetTexture(),
        NULL,
        NULL
    );
}

Texture Renderer::CreateTexture() {
    return Texture(renderer);
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
