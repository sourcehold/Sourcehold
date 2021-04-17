#pragma once
#include "SDL/SDLBackend.h"
#include "SDL/TextureAccessor.h"
#include "Surface.h"

namespace Sourcehold {
namespace Rendering {
template <SDL_TextureAccess A, SDL_PixelFormatEnum P = SDL::PixelFormat>
struct TextureInfo {
  constexpr static auto AccessType = A;
  constexpr static auto PixelFormat = P;
};
using TextureInfoStatic = TextureInfo<SDL_TEXTUREACCESS_STATIC>;
using TextureInfoStreaming = TextureInfo<SDL_TEXTUREACCESS_STREAMING>;
using TextureInfoTarget = TextureInfo<SDL_TEXTUREACCESS_TARGET>;
using TextureInfoVideo =
    TextureInfo<SDL_TEXTUREACCESS_STREAMING, SDL_PIXELFORMAT_RGB888>;
//-----------------------------------------------------------------------------
// Texture -- Prototype
// ----------------------------------------------------------------------------
template <class TextureInfo>
class Texture : private TextureInfo {
 public:
  Texture() = default;
  Texture(const Surface& surface);
  Texture(Vector2<int> size);
  Texture(Texture&& other);
  Texture& operator=(Texture&& other);
  ~Texture() = default;

  void SetAlphaMod(Uint8 alpha);
  void SetColorMod(Uint8 r, Uint8 g, Uint8 b);
  void SetBlendMode(SDL_BlendMode mode);

  // Only available to TextureStreaming and TextureVideo
  // this will result in a linking error
  void Copy(Texture& other, Vector2<int> pos,
            std::optional<Rect<int>> clip = {});
  // Only available to TextureStreaming and TextureVideo
  // this will result in a linking error
  SDL::TextureAccessor Accessor();

  [[nodiscard]] SDL_Texture* Ptr() const noexcept {
    return texture_.get();
  }

  Vector2<int> size_;

 protected:
 private:
  SDL::SDL_Texture_UQ texture_;
};

using TextureStatic = Texture<TextureInfoStatic>;
using TextureStreaming = Texture<TextureInfoStreaming>;
using TextureTarget = Texture<TextureInfoTarget>;
using TextureVideo = Texture<TextureInfoVideo>;
}  // namespace Rendering
}  // namespace Sourcehold
