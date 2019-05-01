#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;

Renderer::Renderer() : Camera(0, 0) {
}

Renderer::~Renderer() {
    if(renderer) SDL_DestroyRenderer(renderer);
}

void Renderer::Init(SDL_Window *window) {
    this->window = window;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        Logger::error("GAME")  << "Unable to create SDL2 renderer: " << SDL_GetError() << std::endl;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    /* Window size */
    SDL_AddEventWatch(ResizeEventWatcher, static_cast<void*>(this));
    SDL_GetWindowSize(window, &width, &height);
}

void Renderer::Update() {
}

void Renderer::Clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void Renderer::Flush() {
    SDL_RenderPresent(renderer);
}

void Renderer::SetTarget(Texture *target, double x, double y, double w, double h) {
    tx = x;
    ty = y;
    tw = w;
    th = h;
    this->target = target;
    SDL_SetRenderTarget(renderer, target->GetTexture());
}

void Renderer::ResetTarget() {
    target = nullptr;
    SDL_SetRenderTarget(renderer, NULL);
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

void Renderer::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(renderer, r, b, g, SDL_ALPHA_OPAQUE);
    int err = SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Renderer::DrawLine(double x1, double y1, double x2, double y2, Uint8 r, Uint8 g, Uint8 b) {
    DrawLine(
        (int)ToCoordX(x1),
        (int)ToCoordY(y1),
        (int)ToCoordX(x2),
        (int)ToCoordY(y2),
        r,g,b
        );
}

double Renderer::NormalizeX(uint32_t c) {
    return (double)c / (double)GetWidth();
}

double Renderer::NormalizeY(uint32_t c) {
    return (double)c / (double)GetHeight();
}

uint32_t Renderer::ToCoordX(double c) {
    return (int)(c * (double)GetWidth());
}

uint32_t Renderer::ToCoordY(double c) {
    return (int)(c * (double)GetHeight());
}

double Renderer::NormalizeTargetX(uint32_t c) {
    int w = width, h = height;
    SDL_Texture *target = SDL_GetRenderTarget(renderer);
    if(target != NULL) {
        SDL_QueryTexture(target, NULL, NULL, &w, &h);
    }

    return (double)c / (double)w;
}

double Renderer::NormalizeTargetY(uint32_t c) {
    int w = width, h = height;
    SDL_Texture *target = SDL_GetRenderTarget(renderer);
    if(target != NULL) {
        SDL_QueryTexture(target, NULL, NULL, &w, &h);
    }

    return (double)c / (double)h;
}

uint32_t Renderer::ToTargetCoordX(double c) {
    if(!target) return ToCoordX(c);
    return (int)(c * (double)GetTargetWidth());
}

uint32_t Renderer::ToTargetCoordY(double c) {
    if(!target) return ToCoordY(c);
    return (int)(c * (double)GetTargetHeight());
}

double Renderer::GetTargetWidth() {
    if(!target) return 1.0;
    return tw;
}

double Renderer::GetTargetHeight() {
    if(!target) return 1.0;
    return th;
}

double Renderer::GetTargetX() {
    if(!target) return 0.0;
    return tx;
}

double Renderer::GetTargetY() {
    if(!target) return 0.0;
    return ty;
}

int Renderer::GetMouseX() {
    int p;
    SDL_GetMouseState(&p, NULL);
    return p;
}

int Renderer::GetMouseY() {
    int p;
    SDL_GetMouseState(NULL, &p);
    return p;

}

int Renderer::ResizeEventWatcher(void *data, SDL_Event *event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_Window *win = SDL_GetWindowFromID(event->window.windowID);
        Renderer *rend = static_cast<Renderer*>(data);
        if (win == rend->window) {
            SDL_GetWindowSize(win, &rend->width, &rend->height);
        }
    }

    return 0;
}
