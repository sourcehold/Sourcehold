#include "Rendering/Texture.h"
#include "Rendering/Surface.h"
#include "SDL/TextureAccessor.h"

#include "System/Logger.h"

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;
using namespace Sourcehold::SDL;

template <>
TextureStatic::Texture(const Surface &surface) {
  texture_ = MakeTextureUQ(SDLBackend::Renderer(), surface.Ptr());
  if (!texture_) {
    auto err = "Unable to create texture from surface: " +  //
               std::string(SDL_GetError());
    throw std::runtime_error(err);
  }

  SDL_SetTextureBlendMode(texture_.get(), BlendMode);

  size_ = {surface.Ptr()->w, surface.Ptr()->h};
}

template <class TextureInfo>
Texture<TextureInfo>::Texture(Vector2<int> size) : size_(size) {
  texture_ = MakeTextureUQ(SDLBackend::Renderer(), TextureInfo::PixelFormat,
                           TextureInfo::AccessType,  //
                           size.x, size.y);
  if (!texture_) {
    auto err = "Unable to create texture from surface: " +  //
               std::string(SDL_GetError());
    throw std::runtime_error(err);
  }

  SDL_SetTextureBlendMode(texture_.get(), BlendMode);
}

template <class TextureInfo>
Texture<TextureInfo>::Texture(Texture &&other)
    : size_(other.size_), texture_(std::move(other.texture_)) {
}

template <class TextureInfo>
Texture<TextureInfo> &Texture<TextureInfo>::operator=(Texture &&other) {
  size_ = other.size_;
  texture_ = std::move(other.texture_);
  return *this;
}

template <class TextureInfo>
void Texture<TextureInfo>::SetAlphaMod(Uint8 alpha) {
  SDL_SetTextureAlphaMod(texture_.get(), alpha);
}

template <class TextureInfo>
void Texture<TextureInfo>::SetColorMod(Uint8 r, Uint8 g, Uint8 b) {
  SDL_SetTextureColorMod(texture_.get(), r, g, b);
}

template <class TextureInfo>
void Texture<TextureInfo>::SetBlendMode(SDL_BlendMode mode) {
  SDL_SetTextureBlendMode(texture_.get(), mode);
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
void TextureVideo::Copy(Texture &other, Vector2<int> pos,
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

template <>
TextureAccessor TextureVideo::Accessor() {
  return TextureAccessor(texture_.get());
}

template <>
TextureStreaming::Texture(const Surface &surface) {
  texture_ = MakeTextureUQ(SDLBackend::Renderer(), TextureInfo::PixelFormat,
                           TextureInfo::AccessType, surface.Ptr()->w,
                           surface.Ptr()->h);
  auto accessor = Accessor();
  std::copy(std::begin(surface), std::end(surface), std::begin(accessor));
}

template class Sourcehold::Rendering::Texture<TextureInfoStatic>;
template class Sourcehold::Rendering::Texture<TextureInfoStreaming>;
template class Sourcehold::Rendering::Texture<TextureInfoTarget>;
template class Sourcehold::Rendering::Texture<TextureInfoVideo>;

