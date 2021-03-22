#pragma once

#include <string>
#include <cinttypes>
#include <SDL.h>

#include "Rendering/Texture.h"
#include "Rendering/Shapes.h"

#include "System/Config.h"

namespace Sourcehold {
namespace Rendering {
enum Resolution : int {
  RESOLUTION_800x600,
  RESOLUTION_1024x768,
  RESOLUTION_1280x720,
  RESOLUTION_1280x1024,
  RESOLUTION_1360x768,
  RESOLUTION_1366x768,
  RESOLUTION_1440x900,
  RESOLUTION_1600x900,
  RESOLUTION_1600x1200,
  RESOLUTION_1680x1050,
  RESOLUTION_1920x1080,
  RESOLUTION_DYNAMIC
};

bool InitRenderer();
void DestroyRenderer();
void UpdateRenderer();
void ClearDisplay();
void FlushDisplay();

/**
 * Redirect all rendering operations to the target
 */
void SetTarget(Texture *target, Rect<int> rect);
void ResetTarget();

/**
 * Render a texture
 */
void Render(Texture &texture, int x, int y, SDL_Rect *clip = nullptr);
void Render(Texture &texture, int x, int y, int w, int h,
            SDL_Rect *clip = nullptr);

/**
 * Render a texture to the whole screen
 */
void Render(Texture &texture, SDL_Rect *clip = nullptr);
void Fill(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);

void RenderRect(Rect<int> rect, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0,
                Uint8 a = 255, bool solid = false);
void RenderLine(Line<int> line, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

/**
 * Get custom blend mode for keying out
 * certain pixels based on src
 */
SDL_BlendMode GetAlphaKeyBlendMode();

Rect<int> GetTarget();
SDL_Renderer *GetRenderer();
}  // namespace Rendering
}  // namespace Sourcehold
