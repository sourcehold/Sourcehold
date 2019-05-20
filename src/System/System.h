#pragma once

#include <cinttypes>
#include <string>

#include <Rendering/Renderer.h>

namespace Sourcehold
{
    namespace System
    {
		enum StrongholdEdition {
			STRONGHOLD_CLASSIC,
			STRONGHOLD_COLLECTION,
			STRONGHOLD_HD
		};

		using namespace Rendering;
		
		/**
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
			Resolution resolution;
			uint16_t ndisp;
        };
    }
}
