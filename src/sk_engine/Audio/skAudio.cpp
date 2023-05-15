#include "skAudio.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <unordered_map>

#include <Common/Common.h>


namespace sk_audio {
    namespace {
        std::vector<Mix_Music*> m_MUSIC;
        std::vector<Mix_Chunk*> m_SOUND;

        std::unordered_map <std::string, int> SOUND_MAP;

        int volume;
    }
    void Init() {
        Mix_Init(MIX_INIT_MP3);
        SDL_Init(SDL_INIT_AUDIO);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
        Mix_AllocateChannels(16);
    }
    void ShutDown() {
        for (auto music : m_MUSIC) if (music != nullptr) Mix_FreeMusic(music);
        m_MUSIC.clear();
        for (auto sound : m_SOUND) if (sound != nullptr) Mix_FreeChunk(sound);
        m_SOUND.clear();
        SOUND_MAP.clear();
    }

    int LoadSound(const std::string& path) {
        auto it = SOUND_MAP.find(path);
        if (it != SOUND_MAP.end()) return it->second;

        auto m = Mix_LoadWAV(path.c_str());
        if (m == nullptr) {
            std::cout << Mix_GetError();
            Error("cannot find sound file: " + path);
            return -1;
        }

        m_SOUND.push_back(m);
        return m_SOUND.size() - 1;
    }
    int LoadMusic(const std::string& path) {
        auto it = SOUND_MAP.find(path);
        if (it != SOUND_MAP.end()) return it->second;

        auto m = Mix_LoadMUS(path.c_str());
        if (m == nullptr) {
            std::cout << Mix_GetError();
            Error("cannot find music file: " + path);
            return -1;
        }

        m_MUSIC.push_back(m);
        return m_MUSIC.size() - 1;
    }

    void PlaySound(int index, int volume) {
        int channel = Mix_PlayChannel(-1, m_SOUND[index], 0);
        Mix_Volume(channel, volume);
    }
    void PlayMusic(int index, int volume) {
        Mix_VolumeMusic(volume);
        Mix_PlayMusic(m_MUSIC[index], -1);
    }
    void setVolume(int x, int volume) {
        volume = x;
        Mix_MasterVolume(volume);
    }
}