#pragma once

#include <cinttypes>
#include <memory>

#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>

namespace Sourcehold
{
    namespace Game
    {
        using namespace Rendering;

        class StaticElement : public Texture
        {
                std::shared_ptr<Renderer> renderer;
                bool shown = true;
                double nx, ny, nw, nh;
            public:
                StaticElement(std::shared_ptr<Renderer> rend, double x = 0.0, double y = 0.0);
                StaticElement(const StaticElement &elem);
                ~StaticElement();

                void Hide();
                void Show();
                void Destroy();

                void Translate(uint32_t x, uint32_t y);
                void Translate(double x, double y);
                void Scale(uint32_t w, uint32_t h);
                void Scale(double w, double h);

                void Render();
            protected:
        };
    }
}