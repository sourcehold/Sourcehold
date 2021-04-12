#pragma once
#include "SDL/SDLBackend.h"
#include "SDL/TextureAccessor.h"
#include "Surface.h"

namespace Sourcehold {
namespace Rendering {
//-----------------------------------------------------------------------------
// Texture -- Prototype
// ----------------------------------------------------------------------------
template <SDL_TextureAccess A>
class Texture {
 public:
  constexpr static auto AccessType = A;

  // Returns TextureStatic
  Texture(Surface &surface);
  Texture(Vector2<int> size);
  ~Texture() = default;

  void SetAlphaMod(Uint8 alpha);
  void SetColorMod(Uint8 r, Uint8 g, Uint8 b);
  void SetBlendMode(SDL_BlendMode mode);

  void Set(Vector2<int> pos, Color color);
  // Only available to TextureStreaming
  void Copy(Texture &other, Vector2<int> pos,
            std::optional<Rect<int>> clip = {});
  // Only available to TextureStreaming
  SDL::TextureAccessor Accessor();

  [[nodiscard]] SDL_Texture *Ptr() const noexcept {
    return texture_.get();
  }

 protected:
  Texture() = default;

 private:
  SDL::SDL_Texture_UQ texture_;
};

using TextureStatic = Texture<SDL_TEXTUREACCESS_STATIC>;
using TextureStreaming = Texture<SDL_TEXTUREACCESS_STREAMING>;
using TextureTarget = Texture<SDL_TEXTUREACCESS_TARGET>;
}  // namespace Rendering
}  // namespace Sourcehold
