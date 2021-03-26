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

 private:
  Renderer();
  ~Renderer();
  bool InitRenderer();
  void DestroyRenderer();

  SDL_Renderer *_renderer;
  SDL_Window *_window;
  Texture *_target = nullptr;
  Rect<int> _tr;
};
}  // namespace Rendering
}  // namespace Sourcehold
