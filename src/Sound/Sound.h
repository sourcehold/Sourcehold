#pragma once

#include <boost/filesystem/fstream.hpp>

#include <cinttypes>
#include <cstdio>
#include <map>

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
                /* SDL stuff */
                SDL_AudioSpec spec;
                SDL_AudioDeviceID dev;
                /* Songs */
                //std::map<>
                /* Effects */
                FILE *song = NULL;
                bool playing = false, repeating = false;
            public:
                Sound();
                ~Sound();

                bool Init();
                bool PlayMusic(boost::filesystem::path path, bool repeat = false);
                bool PlayEffect(boost::filesystem::path path, bool repeat = false);
                
                bool IsPlaying();
            protected:
                void PrintError();
                void AudioCallback(Uint8 *stream, int len);
        };
    }
}
