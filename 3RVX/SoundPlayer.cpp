#include "SoundPlayer.h"

#include <Windows.h>
#include <chrono>
#include <mmsystem.h>
#include <mutex>
#include <fstream>

#define PLAY_SYNC SND_MEMORY
#define PLAY_ASYNC (SND_ASYNC | SND_MEMORY)

SoundPlayer::SoundPlayer(std::wstring filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (file.rdstate() & std::ios::failbit) {
        /* Open failed */
        throw std::runtime_error("Audio file not found");
    }

    file.seekg(0, std::ios::end);
    std::streamsize chars = file.tellg();
    file.seekg(0, std::ios::beg);

    _sound = new char[(unsigned int) chars];
    file.read(_sound, chars);
    file.close();

    //_thread = std::thread(&SoundPlayer::PlayerThread, this);
}

SoundPlayer::~SoundPlayer() {
    /* Since we're using SND_MEMORY and SND_ASYNC, we need to stop playback
     * before freeing any memory: */
    PlaySound(NULL, 0, 0);
    delete[] _sound;
}

void SoundPlayer::Play(bool async) {
    //_cv.notify_all();
    PlaySound(NULL, 0, 0);
    PlaySound((LPCWSTR) _sound, NULL, SND_MEMORY | SND_NOSTOP);
}

void SoundPlayer::PlayerThread() {
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    while (true) {
        _cv.wait(lock);
        //PlaySound(NULL, 0, 0);
        PlaySound((LPCWSTR) _sound, NULL, SND_MEMORY | SND_NOSTOP);
        std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }

}