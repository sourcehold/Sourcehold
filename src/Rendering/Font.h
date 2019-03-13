#pragma once

#include <cinttypes>
#include <vector>
#include <memory>

#include <Rendering/Texture.h>
#include <Rendering/Renderer.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class Font
        {
                std::shared_ptr<Renderer> renderer;
            public:
                Font(std::shared_ptr<Renderer> rend);
                ~Font();
                Font(const Font&) = delete;
        };
    }
}
