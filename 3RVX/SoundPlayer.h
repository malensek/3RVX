#pragma once

#include <string>

class SoundPlayer {
public:
    SoundPlayer(std::wstring filePath);
    ~SoundPlayer();

    void Play(bool async = true);

private:
    char *_sound;
};