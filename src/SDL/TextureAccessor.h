#pragma once

#include "SDLBackend.h"
#include "Rendering/Color.h"

namespace Sourcehold {
namespace SDL {
//-----------------------------------------------------------------------------
// TextureAccessor
//-----------------------------------------------------------------------------
// RAII wrapper for SDL_Texture data access.
// Use with Streaming Textures
//-----------------------------------------------------------------------------
struct TextureAccessor {
  TextureAccessor() = delete;
  TextureAccessor(SDL_Texture *texture_to_lock);
  TextureAccessor(TextureAccessor &other) = delete;
  TextureAccessor(TextureAccessor &&other) = delete;
  TextureAccessor operator=(const TextureAccessor &other) = delete;
  TextureAccessor operator=(TextureAccessor &&other) = delete;

  ~TextureAccessor();

  [[nodiscard]] Rendering::Pixel *begin() const noexcept;
  [[nodiscard]] Rendering::Pixel *end() const noexcept;
  [[nodiscard]] const Rendering::Pixel *cbegin() const noexcept;
  [[nodiscard]] const Rendering::Pixel *cend() const noexcept;

  int pitch;
  Rendering::Pixel *data;
  Rendering::Vector2<int> size;
  SDL_Texture *texture;
  uint32_t format;
  int access;
};
}  // namespace SDL
}  // namespace Sourcehold
