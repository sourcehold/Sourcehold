#pragma once

#include <cinttypes>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>

#include <Config.h>
#include <System/Logger.h>

#include <Audio/AudioSource.h>
#include <Audio/Playlist.h>

namespace Sourcehold
{
    namespace Audio
    {
        /* Sound super class */
        class SoundManager
        {
                /* OpenAL stuff */
                ALCdevice *device;
                ALCcontext *context;
            public:
                SoundManager();
                SoundManager(const SoundManager &snd);
                ~SoundManager();

                bool Init();
            protected:
                void PrintError();
        };
    }
}
