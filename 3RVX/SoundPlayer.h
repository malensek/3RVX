#pragma once

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

struct IGraphBuilder;
struct IMediaControl;
struct IMediaEventEx;
struct IMediaSeeking;
struct IUnknown;

/// <summary>
/// Handles playback of sounds using DirectShow. We use DirectShow here instead
/// of the PlaySound API because performance seems better and more consistent
/// overall.
/// </summary>
class SoundPlayer {
public:
    SoundPlayer(std::wstring filePath);
    ~SoundPlayer();

    /// <summary>Plays the sound associated with this SoundPlayer.</summary>
    void Play();

private:
    IGraphBuilder *_graphBuilder;
    IMediaControl *_mediaCtrl;
    IMediaEventEx *_mediaEv;
    IMediaSeeking *_mediaSeek;

    bool _ready;

    int _repeat;
    std::mutex _repeatMutex;

    std::thread _thread;
    std::condition_variable _cv;
    std::mutex _mutex;

    void PlayerThread();
    void SafeRelease(IUnknown *p);
};