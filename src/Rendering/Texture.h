#pragma once

#include <vector>

#include <Config.h>
#include <System/Logger.h>
#include <Rendering/Rendering.h>

namespace OpenSH
{
    namespace Rendering
    {
        class Texture
        {
                Context ctx;
                SDL_Texture *texture;
                std::vector<unsigned char> pixels;
                uint32_t width, height;
            public:
                Texture();
                Texture(Context &ctx);
                ~Texture();

                bool AllocNew(uint32_t width, uint32_t height, uint32_t format);
                void SetPixel(uint32_t x, uint32_t y);
                void UpdateTexture();
                void Render(uint32_t x, uint32_t y);
                void SetContext(Context &ctx);
            protected:
        };
    }
}
