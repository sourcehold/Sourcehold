#pragma once

#include <cinttypes>
#include <vector>
#include <memory>

#include <SDL.h>

namespace Sourcehold {
namespace Rendering {
/**
 * Texture wrapper class, able to create and modify
 * textures, external modification is possible
 * by locking/unlocking the texture stream and obtaining
 * a pointer to the raw pixel data
 */
class Renderer;
class Surface;
class Texture {
  SDL_Texture *texture = nullptr;
  Uint32 *pixels = nullptr;

 public:
  Texture() = default;
  Texture(const Texture &tex);
  ~Texture();

  /**
   * Allocate a new texture. This will turn the class into
   * a streaming texture.
   * Use LockTexture / UnlockTexture to access the pixel data
   */
  bool AllocNewStreaming(int width, int height,
                         int format = SDL_PIXELFORMAT_RGBA4444);

  /**
   * Allocate a new texture using an existing surface. The texture
   * is static and has no read/write support
   */
  bool AllocFromSurface(Surface &surface);

  /**
   * Allocate a new texture in target mode. The pixel
   * data is manipulated via SDL functions, after calling
   * SetTarget. Reset the render target by calling ResetTarget
   * afterwards.
   */
  bool AllocNewTarget(int width, int height,
                      int format = SDL_PIXELFORMAT_RGBA4444);

  void UpdateTexture();
  void Destroy();

  /* Texture manipulation */
  void LockTexture();
  void UnlockTexture();
  void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void Rotate(double angle);
  void SetAlphaMod(Uint8 alpha);
  void SetColorMod(Uint8 r, Uint8 g, Uint8 b);
  void Copy(Texture &other, uint32_t x, uint32_t y, SDL_Rect *rect = nullptr);
  void SetBlendMode(SDL_BlendMode mode);

  int GetWidth();
  int GetHeight();
  void GetDim(int *w, int *h);

  Uint32 *GetData();

  inline bool IsValid() { return texture; }
  inline SDL_Texture *GetTexture() { return texture; }

 protected:
  Uint32 ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
  void FromPixel(Uint32 value, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
};
}  // namespace Rendering
}  // namespace Sourcehold
