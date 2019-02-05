#include <Sound/Sound.h>

using namespace OpenSH::System;
using namespace OpenSH::Sound;

Sound::Sound() {
}

Sound::~Sound() {
    Mix_CloseAudio();
}

bool Sound::Init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        PrintError();
        return false;
    }

    /* Stronghold raw's use unsigned 16-bit
       little endian pcm at a sampling rate of 44100hz
     */
    SDL_memset(&spec, 0, sizeof(SDL_AudioSpec));
    spec.freq = 44100;
    spec.format = AUDIO_S16LSB;
    spec.channels = 1;
    spec.samples = 1024*8;
    spec.userdata = this;
    spec.callback = [](void *userdata, Uint8 *stream, int len) {
        ((Sound*)userdata)->AudioCallback(stream, len);
    };

    dev = SDL_OpenAudioDevice(NULL, 0, &spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
    SDL_PauseAudioDevice(dev, 0);

    return true;
}

bool Sound::PlayMusic(boost::filesystem::path path, bool repeat) {
    if(playing) return false;
    
    song = std::fopen(path.native().c_str(), "rb");
    if(!song) {
        Logger::error("SOUND") << "Unable to load music '" << path.native() << "'!" << std::endl;
        return false;
    }
    playing = true;
    repeating = repeat;
    return true;
}

bool Sound::IsPlaying() {
    return playing;
}

void Sound::PrintError() {
    Logger::error("SOUND") << SDL_GetError() << std::endl;
}

void Sound::AudioCallback(Uint8 *stream, int len) {
    Uint16 *dest = (Uint16*)stream;
    len /= sizeof(Uint16);

    memset(dest, 0, sizeof(Uint16) * len);
    if(!playing) return;
    if(!std::fread(dest, sizeof(Uint16), len, song)) {
        if(repeating) {
            std::fseek(song, 0, SEEK_SET);
        }else {
            std::fclose(song);
            playing = false;
        }
    }
}
