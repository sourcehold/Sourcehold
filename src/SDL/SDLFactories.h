#pragma once
#include "SDLHelpers.h"
namespace Sourcehold {
namespace SDL {
// https://wiki.libsdl.org/SDL_CreateRGBSurface
SDL_Surface_UQ MakeSurfaceUQ(Uint32 flags,                      //
                             int width, int height, int depth,  //
                             Uint32 Rmask, Uint32 Gmask, Uint32 Bmask,
                             Uint32 Amask);
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceFrom
SDL_Surface_UQ MakeSurfaceUQ(void* pixels,                      //
                             int width, int height, int depth,  //
                             int pitch, Uint32 Rmask, Uint32 Gmask,
                             Uint32 Bmask, Uint32 Amask);
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormat
SDL_Surface_UQ MakeSurfaceUQ(Uint32 flags,                      //
                             int width, int height, int depth,  //
                             Uint32 format);
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormatFrom
SDL_Surface_UQ MakeSurfaceUQ(void* pixels,                      //
                             int width, int height, int depth,  //
                             int pitch,                         //
                             Uint32 format);
// https://wiki.libsdl.org/SDL_CreateRGBSurface
SDL_Surface_SH MakeSurfaceSH(Uint32 flags,                      //
                             int width, int height, int depth,  //
                             Uint32 Rmask, Uint32 Gmask, Uint32 Bmask,
                             Uint32 Amask);
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceFrom
SDL_Surface_SH MakeSurfaceSH(void* pixels,                      //
                             int width, int height, int depth,  //
                             int pitch, Uint32 Rmask, Uint32 Gmask,
                             Uint32 Bmask, Uint32 Amask);
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormat
SDL_Surface_SH MakeSurfaceSH(Uint32 flags,                      //
                             int width, int height, int depth,  //
                             Uint32 format);
// https://wiki.libsdl.org/SDL_CreateRGBSurfaceWithFormatFrom
SDL_Surface_SH MakeSurfaceSH(void* pixels,                      //
                             int width, int height, int depth,  //
                             int pitch,                         //
                             Uint32 format);
// https://wiki.libsdl.org/SDL_CreateTexture
SDL_Texture_UQ MakeTextureUQ(SDL_Renderer* renderer,  //
                             Uint32 format,
                             int access,  //
                             int w, int h);
// https://wiki.libsdl.org/SDL_CreateTextureFromSurface
SDL_Texture_UQ MakeTextureUQ(SDL_Renderer*, SDL_Surface*);
// https://wiki.libsdl.org/SDL_CreateTexture
SDL_Texture_SH MakeTextureSH(SDL_Renderer* renderer,  //
                             Uint32 format,
                             int access,  //
                             int w, int h);
// https://wiki.libsdl.org/SDL_CreateTextureFromSurface
SDL_Texture_SH MakeTextureSH(SDL_Renderer*, SDL_Surface*);
// https://wiki.libsdl.org/SDL_CreateWindow
SDL_Window_UQ MakeWindowUQ(const char* title, int x, int y, int w, int h,
                           Uint32 flags);
// https://wiki.libsdl.org/SDL_CreateRenderer
SDL_Renderer_UQ MakeRendererUQ(SDL_Window* window, int index, Uint32 flags);
}  // namespace SDL
}  // namespace Sourcehold
