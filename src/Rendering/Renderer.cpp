#include <Rendering/Renderer.h>
#include <Rendering/Display.h>
#include <System/Logger.h>

using namespace Sourcehold;
using namespace Rendering;
using namespace System;

static SDL_Renderer *_renderer;
static SDL_Window *_window;
static Texture *_target = nullptr;
static double tx,ty,tw,th;

bool Rendering::InitRenderer()
{
    _window = GetWindow();
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if(!_renderer) {
        Logger::error("GAME")  << "Unable to create SDL2 renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void Rendering::DestroyRenderer()
{
    if(_renderer) SDL_DestroyRenderer(_renderer);
}

void Rendering::UpdateRenderer()
{

}

void Rendering::ClearDisplay()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);
}

void Rendering::FlushDisplay()
{
    SDL_RenderPresent(_renderer);
}

void Rendering::SetTarget(Texture *target, double x, double y, double w, double h)
{
    tx = x;
    ty = y;
    tw = w;
    th = h;
    _target = target;
    if(SDL_SetRenderTarget(_renderer, target->GetTexture()) < 0) {
#if 0 // Creates an issue with directX when window out of focus
        Logger::error("RENDERING") << "SDL_SetRenderTarget() failed: " << SDL_GetError() << std::endl;
#endif
    }
}

void Rendering::SetTarget(Texture *target, int x, int y, int w, int h)
{
    tx = NormalizeX(x);
    ty = NormalizeY(y);
    tw = NormalizeX(w);
    th = NormalizeY(h);
    _target = target;
    if(SDL_SetRenderTarget(_renderer, target->GetTexture()) < 0) {
#if 0
        Logger::error("RENDERING") << "SDL_SetRenderTarget() failed: " << SDL_GetError() << std::endl;
#endif
    }
}

void Rendering::ResetTarget()
{
    _target = nullptr;
    tx = ty = 0.0;
    tw = th = 1.0;
    SDL_SetRenderTarget(_renderer, nullptr);
}

void Rendering::Render(Texture &texture, int x, int y, SDL_Rect *clip)
{
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
        _renderer,
        texture.GetTexture(),
        clip,
        &rect,
        texture.GetAngle(),
        nullptr,
        texture.GetFlip()
    );
}

void Rendering::Render(Texture &texture, int x, int y, int w, int h, SDL_Rect *clip)
{
    /**
     * Source texture, specified by SDL_Rect, will
     * be stretched to fit the destination rect
     */
    SDL_Rect rect = {
        x, y,
        w, h
    };

    SDL_RenderCopyEx(
        _renderer,
        texture.GetTexture(),
        clip,
        &rect,
        texture.GetAngle(),
        nullptr,
        texture.GetFlip()
    );
}

void Rendering::Render(Texture &texture, double x, double y, SDL_Rect *clip)
{
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    Render(texture, ix, iy, clip);
}

void Rendering::Render(Texture &texture, double x, double y, double w, double h, SDL_Rect *clip)
{
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    int sx = ToCoordX(w);
    int sy = ToCoordY(h);
    Render(texture, ix, iy, sx, sy, clip);
}

void Rendering::Render(Texture &texture, SDL_Rect *clip)
{
    SDL_RenderCopyEx(
        _renderer,
        texture.GetTexture(),
        clip,
        nullptr,
        texture.GetAngle(),
        nullptr,
        texture.GetFlip()
    );
}

void Rendering::Fill(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderFillRect(_renderer, nullptr);
}

void Rendering::RenderRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool solid)
{
    SDL_SetRenderDrawColor(
        _renderer,
        r, g, b, a
    );

    SDL_Rect rect = {
        x, y,
        w, h
    };

    if(solid) {
        SDL_RenderFillRect(
            _renderer,
            &rect
        );
    }
    else {
        SDL_RenderDrawRect(
            _renderer,
            &rect
        );
    }
}

void Rendering::RenderRect(double x, double y, double w, double h, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool solid)
{
    int ix = ToCoordX(x);
    int iy = ToCoordY(y);
    int sx = ToCoordX(w);
    int sy = ToCoordY(h);
    RenderRect(ix, iy, sx, sy, r, g, b, a, solid);
}

void Rendering::RenderLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetRenderDrawColor(_renderer, r, b, g, SDL_ALPHA_OPAQUE);
    int err = SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
}

void Rendering::RenderLine(double x1, double y1, double x2, double y2, Uint8 r, Uint8 g, Uint8 b)
{
    RenderLine(
        (int)ToCoordX(x1),
        (int)ToCoordY(y1),
        (int)ToCoordX(x2),
        (int)ToCoordY(y2),
        r,g,b
    );
}

SDL_BlendMode Rendering::GetAlphaKeyBlendMode()
{
#if 0
    return SDL_ComposeCustomBlendMode(
               SDL_BLENDFACTOR_ONE, /* The original image */
               SDL_BLENDFACTOR_ZERO, /* The color mask */
               SDL_BLENDOPERATION_SUBTRACT,
               SDL_BLENDFACTOR_ONE,
               SDL_BLENDFACTOR_ONE,
               SDL_BLENDOPERATION_ADD
           );
#else
    return SDL_BLENDMODE_BLEND;
#endif
}

double Rendering::NormalizeX(int32_t c)
{
    return (double)c / (double)GetWidth();
}

double Rendering::NormalizeY(int32_t c)
{
    return (double)c / (double)GetHeight();
}

int32_t Rendering::ToCoordX(double c)
{
    return (int)(c * (double)GetWidth());
}

int32_t Rendering::ToCoordY(double c)
{
    return (int)(c * (double)GetHeight());
}

double Rendering::NormalizeTargetX(int32_t c)
{
    int w = GetWidth(), h = GetHeight();
    SDL_Texture *target = SDL_GetRenderTarget(_renderer);
    if(target != NULL) {
        SDL_QueryTexture(target, NULL, NULL, &w, &h);
    }

    return (double)c / (double)w;
}

double Rendering::NormalizeTargetY(int32_t c)
{
    int w = GetWidth(), h = GetHeight();
    SDL_Texture *target = SDL_GetRenderTarget(_renderer);
    if(target != NULL) {
        SDL_QueryTexture(target, NULL, NULL, &w, &h);
    }

    return (double)c / (double)h;
}

int32_t Rendering::ToTargetCoordX(double c)
{
    if(!_target) return ToCoordX(c);
    return (int)(c * (double)GetTargetWidth());
}

int32_t Rendering::ToTargetCoordY(double c)
{
    if(!_target) return ToCoordY(c);
    return (int)(c * (double)GetTargetHeight());
}

double Rendering::GetTargetWidth()
{
    if(!_target) return 1.0;
    return tw;
}

double Rendering::GetTargetHeight()
{
    if(!_target) return 1.0;
    return th;
}

double Rendering::GetTargetX()
{
    if(!_target) return 0.0;
    return tx;
}

double Rendering::GetTargetY()
{
    if(!_target) return 0.0;
    return ty;
}

SDL_Renderer *Rendering::GetRenderer()
{
    return _renderer;
}
