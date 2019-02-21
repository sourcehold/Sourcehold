#pragma once

#include <Config.h>
#include <Rendering/Rendering.h>
#include <Rendering/Texture.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class StaticElement : public Texture
        {
                bool shown = true;
            public:
                StaticElement();
                ~StaticElement();

                virtual void Render();
                virtual void Hide();
                virtual void Show();
                virtual void Destroy();
            protected:
        };
    }
}
