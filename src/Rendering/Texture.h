#pragma once

#include <vector>

#include <Config.h>
#include <System/Logger.h>
#include <Rendering/Rendering.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class Texture
        {
                Context ctx;
                SDL_Texture *texture;
                std::vector<uint32_t> pixels;
                uint32_t width, height;
            public:
                Texture();
                Texture(Context &ctx);
                ~Texture();

                bool AllocNew(uint32_t width, uint32_t height, uint32_t format);
                void SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
                void UpdateTexture();
                void SetContext(Context &ctx);

                SDL_Texture *GetTexture();
                uint32_t GetWidth();
                uint32_t GetHeight();
            protected:
        };
    }
}
