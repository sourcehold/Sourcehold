#pragma once

#include <Config.h>
#include <Rendering.h>

namespace OpenSH
{
    namespace Rendering
    {
        class Texture
        {
                SDL_Texture *texture;
            public:
                Texture(Context &ctx);
                ~Texture();
            protected:
        };
    }
}
