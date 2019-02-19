#pragma once

#include <cinttypes>
#include <vector>
#include <string>

#include <Config.h>
#include <System/Logger.h>

#include <Audio/AudioSource.h>

namespace Sourcehold
{
    namespace Audio
    {
        class Playlist
        {
                std::vector<AudioSource> songs;
                bool repeat, playing;
                uint32_t current;
            public:
                Playlist(uint32_t hint = 4, bool repeat = false);
                Playlist(std::vector<std::string> paths, bool repeat = false);
                Playlist(const Playlist& list);
                ~Playlist();

                void Push(AudioSource &song);
                void Play(uint32_t index = 0);
                void Pause();
                void Resume();
                void Stop();
                void Skip();
                void Clear();
                void Update();

                bool IsPlaying();
                bool IsRepeating();
                AudioSource& GetCurrent();
            protected:
        };
    }
}
