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
                int width, height;
            public:
                Texture();
                Texture(Context &ctx);
                ~Texture();

                bool AllocNew(int width, int height, int format);
                void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
                void UpdateTexture();
                void SetContext(Context &ctx);

                SDL_Texture *GetTexture();
                uint32_t *GetData();
                int GetWidth();
                int GetHeight();
            protected:
        };
    }
}
