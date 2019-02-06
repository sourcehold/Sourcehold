#pragma once

#include <boost/filesystem/fstream.hpp>

#include <cstdio>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <Config.h>
#include <System/Logger.h>

namespace Sourcehold
{
    namespace Sound
    {
        /* Sound super class */
        class Sound
        {
                SDL_AudioSpec spec;
                SDL_AudioDeviceID dev;
                FILE *song = NULL;
                bool playing = false, repeating = false;
            public:
                Sound();
                ~Sound();

                bool Init();
                bool PlayMusic(boost::filesystem::path path, bool repeat = false);
                bool IsPlaying();
            protected:
                void PrintError();
                void AudioCallback(Uint8 *stream, int len);
        };
    }
}
