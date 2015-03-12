#pragma once

#include <condition_variable>
#include <string>
#include <thread>

class SoundPlayer {
public:
    SoundPlayer(std::wstring filePath);
    ~SoundPlayer();

    void Play(bool async = true);

private:
    std::condition_variable _cv;
    char *_sound;
    std::thread _thread;

    void PlayerThread();
};