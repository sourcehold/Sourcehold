#include "TextureAccessor.h"
#include <SDL_error.h>
#include <stdexcept>
using namespace Sourcehold::SDL;
using namespace Sourcehold::Rendering;

TextureAccessor::TextureAccessor(SDL_Texture* texture_to_lock)
    : texture(texture_to_lock) {
  auto err = SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&data),
                             &pitch);
  if (err) {
    auto msg =
        std::string("failed to create TextureAccessor: ") + SDL_GetError();
    throw std::runtime_error(msg);
  }
  err = SDL_QueryTexture(texture, &format, &access, &size.x, &size.y);
  if (err) {
    auto msg = std::string("failed to query texture: ") + SDL_GetError();
    throw std::runtime_error(msg);
  }
}

TextureAccessor::~TextureAccessor() {
  SDL_UnlockTexture(texture);
}

Pixel* TextureAccessor::begin() const noexcept {
  return &data[0];
}

Pixel* TextureAccessor::end() const noexcept {
  return &data[size.x * size.y];
}

const Pixel* TextureAccessor::cbegin() const noexcept {
  return begin();
}

const Pixel* TextureAccessor::cend() const noexcept {
  return end();
}
