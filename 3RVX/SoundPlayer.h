#pragma once

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

struct IGraphBuilder;
struct IMediaControl;
struct IMediaEventEx;
struct IMediaSeeking;

class SoundPlayer {
public:
    SoundPlayer(std::wstring filePath);
    ~SoundPlayer();

    bool Play();

private:
    IGraphBuilder *_graphBuilder;
    IMediaControl *_mediaCtrl;
    IMediaEventEx *_mediaEv;
    IMediaSeeking *_mediaSeek;

    bool _playing;

    std::thread _thread;
    std::condition_variable _cv;
    std::mutex _mutex;

    void PlayerThread();
};