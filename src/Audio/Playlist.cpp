#include <Audio/Playlist.h>

using namespace Sourcehold::Audio;
using namespace Sourcehold::System;

Playlist::Playlist(uint32_t hint, bool repeat) :
    repeat(repeat),
    playing(false),
    current(0)
{
    this->songs.reserve(hint);
}

Playlist::Playlist(std::vector<std::string> paths, bool repeat) :
    repeat(repeat),
    playing(false)
{
    for(const auto& p : paths) {
        songs.push_back(
            AudioSource(p, false)
        );
    }

    this->current = 0;
}

Playlist::Playlist(const Playlist& list) :
    repeat(list.repeat),
    playing(list.playing),
    current(list.current)
{ }

Playlist::~Playlist() {
    Clear();
}

void Playlist::Push(AudioSource &song) {
    songs.push_back(song);
}

void Playlist::Play(uint32_t index) {
    /* Does the playlist contain any songs? */
    if(songs.empty()) {
        Logger::warning("AUDIO") << "No songs to play!" << std::endl;
        return;
    }

    /* Is the given index valid? */
    if(index > songs.size()-1) {
        Logger::warning("AUDIO") << "Song index out of bounds, defaulting to last!" << std::endl;
        index = songs.size()-1;
    }

    /* Are we currently playing another song? */
    if(playing) {
        songs.at(current).Stop();
    }

    playing = true;
    current = index;

    songs.at(index).Play();
}

void Playlist::Pause() {
    playing = false;
    songs.at(current).Pause();
}

void Playlist::Resume() {
    playing = true;
    songs.at(current).Resume();
}

void Playlist::Stop() {
    playing = false;
    Clear();
}

void Playlist::Skip() {
    if(current != songs.size()-1) {
        /* Skip to next song */
        Play(current + 1);
    }else {
        /* Else return to first or stop */
        if(repeat) Play(0);
        else Pause();
    }
}

void Playlist::Clear() {
    songs.clear();
}

void Playlist::Update() {
    if(songs.empty() || !playing) return;

    if(!songs.front().IsPlaying()) {
        if(repeat) {
            /* Are we at the end yet? */
            if(current != songs.size()-1) {
                /* No -> next one */
                Play(current + 1);
            }else {
                /* Yes -> Go back to the start */
                Play(0);
            }
        }else if(current != songs.size()-1) {
            /* Not repeating, not at the end yet */
            Play(current + 1);
        }else {
            /* End of playlist */
            playing = false;
            current = 0;
        }
    }
}

bool Playlist::IsPlaying() {
    return playing;
}

bool Playlist::IsRepeating() {
    return repeat;
}

AudioSource& Playlist::GetCurrent() {
    return songs.at(current);
}
