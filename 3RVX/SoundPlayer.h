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
    /// <summary>
    /// Creates a new SoundPlayer for the given audio file path.
    /// </summary>
    /// <param name="filePath>Path to an audio file to play.</param>
    /// <param name="repeatLimit">
    /// Restricts the maximum number of plays that can queued up.
    /// </param>
    SoundPlayer(std::wstring filePath, int repeatLimit = 4);
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
    int _repeatLimit;
    std::mutex _repeatMutex;

    std::thread _thread;
    std::condition_variable _cv;
    std::mutex _mutex;

    void PlayerThread();
    void SafeRelease(IUnknown *p);
};