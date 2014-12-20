#include "SoundPlayer.h"

#include <Windows.h>
#include <mmsystem.h>
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

    _sound = new char[chars];
    file.read(_sound, chars);
    file.close();
}

SoundPlayer::~SoundPlayer() {
    /* Since we're using SND_MEMORY and SND_ASYNC, we need to stop playback
     * before freeing any memory: */
    PlaySound(NULL, 0, 0);
    delete[] _sound;
}

void SoundPlayer::Play(bool async) {
    PlaySound(NULL, 0, 0);
    PlaySound((LPCWSTR) _sound, NULL, (async ? PLAY_ASYNC : PLAY_SYNC));
}