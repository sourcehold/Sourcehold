#include "SDLFactories.h"
#include "SDLHelpers.h"
namespace Sourcehold {
namespace SDL {
// https://wiki.libsdl.org/SDL_CreateRGBSurface
SDL_Surface_UQ MakeSurfaceUQ(Uint32 flags,                      //
                             int width, int height, int depth,  //
                             Uint32 Rmask, Uint32 Gmask, Uint32 Bmask,
                             Uint32 Amask) {
  return SDL_Surface_UQ{SDL_CreateRGBSurface(flags, width, height, depth, Rmask,
                                             Gmask, Bmask, Amask)};
}
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceFrom
SDL_Surface_UQ MakeSurfaceUQ(void* pixels,                      //
                             int width, int height, int depth,  //
                             int pitch, Uint32 Rmask, Uint32 Gmask,
                             Uint32 Bmask, Uint32 Amask) {
  return SDL_Surface_UQ{SDL_CreateRGBSurfaceFrom(
      pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask)};
}
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormat
SDL_Surface_UQ MakeSurfaceUQ(Uint32 flags,                      //
                             int width, int height, int depth,  //
                             Uint32 format) {
  return SDL_Surface_UQ{
      SDL_CreateRGBSurfaceWithFormat(flags, width, height, depth, format)};
}
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormatFrom
SDL_Surface_UQ MakeSurfaceUQ(void* pixels,                      //
                             int width, int height, int depth,  //
                             int pitch,                         //
                             Uint32 format) {
  return SDL_Surface_UQ{SDL_CreateRGBSurfaceWithFormatFrom(
      pixels, width, height, depth, pitch, format)};
}
// https://wiki.libsdl.org/SDL_CreateRGBSurface
SDL_Surface_SH MakeSurfaceSH(Uint32 flags,                      //
                             int width, int height, int depth,  //
                             Uint32 Rmask, Uint32 Gmask, Uint32 Bmask,
                             Uint32 Amask) {
  return SDL_Surface_SH{SDL_CreateRGBSurface(flags, width, height, depth, Rmask,
                                             Gmask, Bmask, Amask),
                        Details::SDL_Deleter()};
}
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceFrom
SDL_Surface_SH MakeSurfaceSH(void* pixels,                      //
                             int width, int height, int depth,  //
                             int pitch, Uint32 Rmask, Uint32 Gmask,
                             Uint32 Bmask, Uint32 Amask) {
  return SDL_Surface_SH{
      SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, Rmask,
                               Gmask, Bmask, Amask),
      Details::SDL_Deleter()};
}
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormat
SDL_Surface_SH MakeSurfaceSH(Uint32 flags,                      //
                             int width, int height, int depth,  //
                             Uint32 format) {
  return SDL_Surface_SH{
      SDL_CreateRGBSurfaceWithFormat(flags, width, height, depth, format),
      Details::SDL_Deleter()};
}
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormatFrom
SDL_Surface_SH MakeSurfaceSH(void* pixels,                      //
                             int width, int height, int depth,  //
                             int pitch,                         //
                             Uint32 format) {
  return SDL_Surface_SH{SDL_CreateRGBSurfaceWithFormatFrom(
                            pixels, width, height, depth, pitch, format),
                        Details::SDL_Deleter()};
}
// https://wiki.libsdl.org/SDL_CreateTexture
SDL_Texture_UQ MakeTextureUQ(SDL_Renderer* renderer,  //
                             Uint32 format,
                             int access,  //
                             int w, int h) {
  return SDL_Texture_UQ{SDL_CreateTexture(renderer, format, access, w, h)};
}
// https://wiki.libsdl.org/SDL_CreateTextureFromSurface
SDL_Texture_UQ MakeTextureUQ(SDL_Renderer* renderer, SDL_Surface* surface) {
  return SDL_Texture_UQ{SDL_CreateTextureFromSurface(renderer, surface)};
}
// https://wiki.libsdl.org/SDL_CreateTexture
SDL_Texture_SH MakeTextureSH(SDL_Renderer* renderer,  //
                             Uint32 format,
                             int access,  //
                             int w, int h) {
  return SDL_Texture_SH{SDL_CreateTexture(renderer, format, access, w, h),
                        Details::SDL_Deleter()};
}
// https://wiki.libsdl.org/SDL_CreateTextureFromSurface
SDL_Texture_SH MakeTextureSH(SDL_Renderer* renderer, SDL_Surface* surface) {
  return SDL_Texture_SH{SDL_CreateTextureFromSurface(renderer, surface),
                        Details::SDL_Deleter()};
}
// https://wiki.libsdl.org/SDL_CreateWindow
SDL_Window_UQ MakeWindowUQ(const char* title, int x, int y, int w, int h,
                           Uint32 flags) {
  return SDL_Window_UQ{SDL_CreateWindow(title, x, y, w, h, flags)};
}
// https://wiki.libsdl.org/SDL_CreateRenderer
SDL_Renderer_UQ MakeRendererUQ(SDL_Window* window, int index, Uint32 flags) {
  return SDL_Renderer_UQ{SDL_CreateRenderer(window, index, flags)};
}
}  // namespace SDL
}  // namespace Sourcehold
