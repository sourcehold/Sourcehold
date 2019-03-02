#pragma once

#include <cinttypes>
#include <string>

namespace Sourcehold
{
    namespace System
    {
        struct GameOptions
        {
            std::string config;
            bool debug = false;
            bool noborder = false;
            bool fullscreen = false;
            uint16_t width,height;
        };
    }
}
