#pragma once

#include <winioctl.h>
#include <dshow.h>
#include <ks.h>
#include <ksmedia.h>

#include <condition_variable>
#include <string>
#include <thread>


class SoundPlayer {
public:
    SoundPlayer(std::wstring filePath);
    ~SoundPlayer();

    void Play(bool async = true);

private:
    IGraphBuilder *_graphBuilder;
    IMediaControl *_mediaCtrl;
    IMediaEvent *_mediaEv;
    IMediaSeeking *_mediaSeek;

    std::condition_variable _cv;
    char *_sound;
    std::thread _thread;

    void PlayerThread();
};