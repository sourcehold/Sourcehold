#pragma once
#include "SDL/SDLBackend.h"
#include "Surface.h"

namespace Sourcehold {
namespace Rendering {
template <typename T, SDL_TextureAccess A>
using IFSTREAMING = std::enable_if<A == SDL_TEXTUREACCESS_STREAMING, T>;
template <typename T, SDL_TextureAccess A>
using IFSTATIC = std::enable_if<A == SDL_TEXTUREACCESS_STATIC, T>;
template <typename T, SDL_TextureAccess A>
using IFTARGET = std::enable_if<A == SDL_TEXTUREACCESS_TARGET, T>;

namespace {
//-----------------------------------------------------------------------------
// TextureAccessor
//-----------------------------------------------------------------------------
// RAII wrapper for SDL_Texture data access.
//-----------------------------------------------------------------------------
struct TextureAccessor {
  int pitch;
  uint32_t *data;
  Vector2<int> size;
  SDL_Texture *texture;

  TextureAccessor() = delete;
  TextureAccessor(SDL_Texture *texture_to_lock) : texture(texture_to_lock) {
    SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&data), &pitch);
    SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
  }

  ~TextureAccessor() {
    SDL_UnlockTexture(texture);
  }

 private:
};
}  // namespace

//-----------------------------------------------------------------------------
// Texture -- Prototype
// ----------------------------------------------------------------------------
template <SDL_TextureAccess A>
class Texture {
 public:
  constexpr static auto AccessType = A;

  Texture() = delete;
  Texture(Surface &surface);
  Texture(Vector2<int> size);
  ~Texture() = default;

  void Set(Vector2<int> pos, Color color);
  void SetAlphaMod(Uint8 alpha);
  void SetColorMod(Uint8 r, Uint8 g, Uint8 b);
  IFSTREAMING<void, A> Copy(Texture &other, Vector2<int> pos,
                            std::optional<Rect<int>> clip);
  void SetBlendMode(SDL_BlendMode mode);

  [[nodiscard]] SDL_Texture *Ptr() const noexcept {
    return texture_.get();
  }

 private:
  SDL::SDL_Texture_UQ texture_;
};
}  // namespace Rendering
}  // namespace Sourcehold
