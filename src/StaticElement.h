#pragma once

#include <Config.h>

namespace Sourcehold
{
    namespace Game
    {
        class StaticElement
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