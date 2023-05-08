#pragma once

#include <string>

namespace sk_audio {
    void Init();
    void ShutDown();

    // volume range is 0 - 128
    void setVolume(int x);

    int LoadSound(const std::string& path);
    int LoadMusic(const std::string& path);

    void PlaySound(int index);
    void PlayMusic(int index);
}