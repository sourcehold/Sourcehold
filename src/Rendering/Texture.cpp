#include "Rendering/Texture.h"
#include "Rendering/Surface.h"

#include "System/Logger.h"

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::SDL;

template <SDL_TextureAccess A>
Texture<A>::Texture(Surface &surface) {
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
void Texture<A>::Set(Vector2<int> pos, Color color) {
  static_assert(true, "don't call");
  auto write = TextureAccessor(texture_.get());
  write.data[At(pos, write.size.x)] = AsPixel(color);
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
IFSTREAMING<void, A> Texture<A>::Copy(Texture &other, Vector2<int> pos,
                                      std::optional<Rect<int>> clip) {
  auto write = TextureAccessor(texture_.get());
  auto read = TextureAccessor(other.Ptr());
  Vector2<int> offset{0, 0};
  if (clip.has_value()) {
    offset.x = clip->x;
    offset.y = clip->y;
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

  {
    Vector2<int> iter;
    for (iter.x = 0; iter.x < read.size.x; ++iter.x) {
      for (iter.y = 0; iter.y < read.size.y; ++iter.y) {
        auto read_index = At(iter + offset, read.size.y);
        auto write_index = At(iter + pos, write.size.y);

        write.data[write_index] = read.data[read_index];
      }
    }
  }
}

template <SDL_TextureAccess A>
void Texture<A>::SetBlendMode(SDL_BlendMode mode) {
  SDL_SetTextureBlendMode(texture_.get(), mode);
}
