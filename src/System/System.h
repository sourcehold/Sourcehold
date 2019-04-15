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
            std::string dataDir;
            bool debug = false;
            bool noborder = false;
            bool fullscreen = false;
            bool nothread = false;
            bool nosound = false;
            bool nograb = false;
            int color = -1;
            uint16_t width,height;
            uint16_t ndisp;
        };
    }
}
