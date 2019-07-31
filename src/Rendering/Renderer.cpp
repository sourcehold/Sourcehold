#include <Rendering/Renderer.h>
#include <Rendering/Display.h>
#include <System/Logger.h>

using namespace Sourcehold;
using namespace Rendering;
using namespace System;

static SDL_Renderer *_renderer;
static SDL_Window *_window;
static Texture *_target = nullptr;
static Rect<double> _tr;

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

void Rendering::SetTarget(Texture *target, Rect<double> rect)
{
    _tr = rect;
    _target = target;
    if(SDL_SetRenderTarget(_renderer, target->GetTexture()) < 0) {
#if 0 // Creates an issue with directX when window out of focus
        Logger::error("RENDERING") << "SDL_SetRenderTarget() failed: " << SDL_GetError() << std::endl;
#endif
    }
}

void Rendering::SetTarget(Texture *target, Rect<int> rect)
{
    _tr.x = NormalizeX(rect.x);
    _tr.y = NormalizeY(rect.y);
    _tr.w = NormalizeX(rect.w);
    _tr.h = NormalizeY(rect.h);
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
    _tr = { 0.0, 0.0, 1.0, 1.0 };
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

void Rendering::RenderRect(Rect<int> rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool solid)
{
    SDL_SetRenderDrawColor(
        _renderer,
        r, g, b, a
    );

    if(solid) {
        SDL_RenderFillRect(
            _renderer,
            (SDL_Rect*)&rect
        );
    }
    else {
        SDL_RenderDrawRect(
            _renderer,
            (SDL_Rect*)&rect
        );
    }
}

void Rendering::RenderRect(Rect<double> rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool solid)
{
    Rect<int> realR = {
        ToCoordX(rect.x),
        ToCoordY(rect.y),
        ToCoordX(rect.w),
        ToCoordY(rect.h)
    };
    RenderRect(realR, r, g, b, a, solid);
}

void Rendering::RenderLine(Line<int> line, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetRenderDrawColor(_renderer, r, b, g, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(_renderer, line.x1, line.y1, line.x2, line.y2);
}

void Rendering::RenderLine(Line<double> line, Uint8 r, Uint8 g, Uint8 b)
{
    Line<int> realL = {
        ToCoordX(line.x1),
        ToCoordY(line.y1),
        ToCoordX(line.x2),
        ToCoordY(line.y2)
    };

    RenderLine(
        realL,
        r,g,b
    );
}

SDL_BlendMode Rendering::GetAlphaKeyBlendMode()
{
#if 0
    return SDL_ComposeCustomBlendMode(
               SDL_BLENDFACTOR_ONE,
               SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR,
               SDL_BLENDOPERATION_ADD,
               SDL_BLENDFACTOR_SRC_ALPHA,
               SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR,
               SDL_BLENDOPERATION_SUBTRACT
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
    return _tr.w;
}

double Rendering::GetTargetHeight()
{
    if(!_target) return 1.0;
    return _tr.h;
}

double Rendering::GetTargetX()
{
    if(!_target) return 0.0;
    return _tr.x;
}

double Rendering::GetTargetY()
{
    if(!_target) return 0.0;
    return _tr.y;
}

SDL_Renderer *Rendering::GetRenderer()
{
    return _renderer;
}
