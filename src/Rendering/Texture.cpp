#include "Rendering/Texture.h"
#include "Rendering/Surface.h"
#include "SDL/TextureAccessor.h"

#include "System/Logger.h"

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::SDL;

template <>
TextureStatic::Texture(Surface &surface) {
  texture_ = MakeTextureUQ(SDLBackend::Renderer(), surface.Ptr());
  if (!texture_) {
    auto err = "Unable to create texture from surface: " +  //
               std::string(SDL_GetError());
    throw std::runtime_error(err);
  }

  SDL_SetTextureBlendMode(texture_.get(), BlendMode);
}

template <SDL_TextureAccess A>
Texture<A>::Texture(Vector2<int> size) {
  texture_ = MakeTextureUQ(SDLBackend::Renderer(), PixelFormat, A,  //
                           size.x, size.y);
  if (!texture_) {
    auto err = "Unable to create texture from surface: " +  //
               std::string(SDL_GetError());
    throw std::runtime_error(err);
  }

  SDL_SetTextureBlendMode(texture_.get(), BlendMode);
}

template <SDL_TextureAccess A>
void Texture<A>::SetAlphaMod(Uint8 alpha) {
  SDL_SetTextureAlphaMod(texture_.get(), alpha);
}

template <SDL_TextureAccess A>
void Texture<A>::SetColorMod(Uint8 r, Uint8 g, Uint8 b) {
  SDL_SetTextureColorMod(texture_.get(), r, g, b);
}

template <SDL_TextureAccess A>
void Texture<A>::SetBlendMode(SDL_BlendMode mode) {
  SDL_SetTextureBlendMode(texture_.get(), mode);
}

template <SDL_TextureAccess A>
void Texture<A>::Set(Vector2<int> pos, Color color) {
  static_assert(true, "don't call");
  assert(A == SDL_TEXTUREACCESS_STREAMING);
  auto write = TextureAccessor(texture_.get());
  write.data[At(pos, write.size.x)] = AsPixel(color);
}

template <>
void TextureStreaming::Copy(Texture &other, Vector2<int> pos,
                            std::optional<Rect<int>> clip) {
  auto write = TextureAccessor(texture_.get());
  auto read = TextureAccessor(other.Ptr());
  Vector2<int> read_offset{0, 0};
  if (clip.has_value()) {
    read_offset.x = clip->x;
    read_offset.y = clip->y;
    read.size.x = clip->w;
    read.size.y = clip->h;
  }

  if (read.size.x + pos.x > write.size.x ||
      read.size.y + pos.y > write.size.y) {
    // TODO: assert here
    Logger::error(RENDERING) << "Attempted to copy a texture which is too "
                                "large for the target (or goes out of bounds)!"
                             << std::endl;
    return;
  }

  for (auto y = 0; y < read.size.y; ++y) {
    for (auto x = 0; x < read.size.x; ++x) {
      auto write_pos = At(pos + Vector2<int>{x, y}, write.size.x);
      auto read_pos = At(read_offset + Vector2<int>{x, y}, read.size.x);
      write.data[write_pos] = read.data[read_pos];
    }
  }
}
template <>
TextureAccessor TextureStreaming::Accessor() {
  return TextureAccessor(texture_.get());
}
