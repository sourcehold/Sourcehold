#pragma once

#include <string>
#include <cinttypes>
#include <SDL.h>

#include "Rendering/Texture.h"
#include "Rendering/Shapes.h"
#include "Rendering/Resolution.h"
#include "Rendering/Color.h"

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
  void Render(Texture &texture, Vector2<int> pos, SDL_Rect *clip = nullptr);
  void Render(Texture &texture, Rect<int> space, SDL_Rect *clip = nullptr);

  /**
   * Render a texture to the whole screen
   */
  void Render(Texture &texture, SDL_Rect *clip = nullptr);
  void Fill(Color color = {0, 0, 0});

  void RenderRect(Rect<int> rect, Color color = {0, 0, 0}, bool solid = false);
  void RenderLine(Line<int> line, Color color = {255, 255, 255});

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
