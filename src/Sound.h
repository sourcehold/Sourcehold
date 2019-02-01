#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <Config.h>
#include <Logger.h>

namespace OpenSH
{
    namespace Sound
    {
        /* Sound super class */
        class Sound
        {
                ALCdevice *device;
            public:
                Sound();
                ~Sound();

                bool Init();
            protected:
                void PrintError();
        };
    }
}
