#pragma once

#include <cinttypes>
#include <string>

namespace Sourcehold
{
    namespace System
    {
        /*
         * Game startup options, modified via config file
         * or command line arguments
         */
        struct GameOptions
        {
            std::string config;
            bool debug = false;
            bool noborder = false;
            bool fullscreen = false;
            bool nothread = false;
            uint16_t width,height;
        };
    }
}
