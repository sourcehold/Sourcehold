#pragma once

#include <string>
#include <cinttypes>
#include <SDL.h>

#include "Rendering/Texture.h"
#include "Rendering/Shapes.h"
#include "Rendering/Resolution.h"

#include "System/Config.h"

namespace Sourcehold {
namespace Rendering {
class Renderer {
 public:
  static Renderer &Instance() {
    static Renderer renderer_;
    return renderer_;
  }

  void Update();
  void Clear();
  void Flush();

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

  SDL_Renderer *sdl_renderer_;
  Texture *target_ = nullptr;
  Rect<int> bounds_;

 private:
  Renderer();
  ~Renderer();
  bool InitRenderer();
  void DestroyRenderer();

  SDL_Window *sdl_window_;
};
}  // namespace Rendering
}  // namespace Sourcehold
