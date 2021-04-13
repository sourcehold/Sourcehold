#include "Rendering/Renderer.h"
#include "Rendering/Display.h"
#include "SDL/SDLBackend.h"

using namespace Sourcehold::Rendering;
using namespace Sourcehold::SDL;

void Renderer::Clear() noexcept {
  SDL_SetRenderDrawColor(SDLBackend::Renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(SDLBackend::Renderer());
}

void Renderer::Flush() noexcept {
  SDL_RenderPresent(SDLBackend::Renderer());
}

template <class TextureInfo>
void Renderer::Draw(const Texture<TextureInfo> &texture, Vector2<int> dest,
                    std::optional<Rect<int>> clip) noexcept {
  Draw(texture, {dest.x, dest.y, texture.size_.x, texture.size_.y}, clip);
}
template <class TextureInfo>
void Renderer::Draw(const Texture<TextureInfo> &texture, Rect<int> dest,
                    std::optional<Rect<int>> clip) noexcept {
  SDL_RenderCopy(SDLBackend::Renderer(), texture.Ptr(),                    //
                 clip.has_value() ? ToSDLRectPtr(clip.value()) : nullptr,  //
                 ToSDLRectPtr(dest));
}
template <class TextureInfo>
void Renderer::Draw(const Texture<TextureInfo> &texture,
                    std::optional<Rect<int>> clip) noexcept {
  SDL_RenderCopy(SDLBackend::Renderer(), texture.Ptr(),  //
                 clip.has_value() ? ToSDLRectPtr(clip.value()) : nullptr,
                 nullptr);
}
void Renderer::Draw(Rect<int> rect, Color color, bool fill) noexcept {
  SetDrawColor(color);
  fill ? SDL_RenderFillRect(SDLBackend::Renderer(), ToSDLRectPtr(rect))
       : SDL_RenderDrawRect(SDLBackend::Renderer(), ToSDLRectPtr(rect));
}
void Renderer::Draw(Line<int> line, Color color) noexcept {
  SetDrawColor(color);
  SDL_RenderDrawLine(SDLBackend::Renderer(), line.x1, line.y1, line.x2,
                     line.y2);
}

void Renderer::Fill(Color color) noexcept {
  SetDrawColor(color);
  SDL_RenderFillRect(SDLBackend::Renderer(), nullptr);
}
void Renderer::SetDrawColor(Color color) noexcept {
  SDL_SetRenderDrawColor(SDLBackend::Renderer(),  //
                         color.r, color.g, color.b, color.a);
}

template void Renderer::Draw(const TextureStatic &texture, Vector2<int> dest,
                             std::optional<Rect<int>> clip) noexcept;
template void Renderer::Draw(const TextureStreaming &texture, Vector2<int> dest,
                             std::optional<Rect<int>> clip) noexcept;
template void Renderer::Draw(const TextureVideo &texture, Vector2<int> dest,
                             std::optional<Rect<int>> clip) noexcept;

template void Renderer::Draw(const TextureStatic &texture, Rect<int> dest,
                             std::optional<Rect<int>> clip) noexcept;
template void Renderer::Draw(const TextureStreaming &texture, Rect<int> dest,
                             std::optional<Rect<int>> clip) noexcept;
template void Renderer::Draw(const TextureVideo &texture, Rect<int> dest,
                             std::optional<Rect<int>> clip) noexcept;

template void Renderer::Draw(const TextureStatic &texture,
                             std::optional<Rect<int>> clip) noexcept;
template void Renderer::Draw(const TextureStreaming &texture,
                             std::optional<Rect<int>> clip) noexcept;
template void Renderer::Draw(const TextureVideo &texture,
                             std::optional<Rect<int>> clip) noexcept;

