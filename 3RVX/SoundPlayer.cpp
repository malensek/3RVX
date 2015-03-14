#include "SoundPlayer.h"

#include <dshow.h>
#include <objbase.h>
#include <chrono>
#include <mutex>
#include <uuids.h>

#define PLAY_SYNC SND_MEMORY
#define PLAY_ASYNC (SND_ASYNC | SND_MEMORY)

SoundPlayer::SoundPlayer(std::wstring filePath) {

    HRESULT hr;

    hr = CoCreateInstance(
        CLSID_FilterGraph,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder,
        (void **) &_graphBuilder);

    if (FAILED(hr)) {

    }

    hr = _graphBuilder->QueryInterface(IID_IMediaControl, (void **) _mediaCtrl);
    hr = _graphBuilder->QueryInterface(IID_IMediaEventEx, (void **) _mediaEv);
    hr = _graphBuilder->QueryInterface(IID_IMediaSeeking, (void **) _mediaSeek);
    hr = _graphBuilder->RenderFile(filePath.c_str(), NULL);

    _thread = std::thread(&SoundPlayer::PlayerThread, this);
}

SoundPlayer::~SoundPlayer() {
    _mediaSeek->Release();
    _mediaEv->Release();
    _mediaCtrl->Release();
    _graphBuilder->Release();
}

void SoundPlayer::Play(bool async) {
    _cv.notify_all();
}

void SoundPlayer::PlayerThread() {
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    long evCode;
    REFERENCE_TIME start = 0;

    while (true) {
        _cv.wait(lock);

        _mediaCtrl->Run();
        _mediaEv->WaitForCompletion(INFINITE, &evCode);
        _mediaSeek->SetPositions(
            &start, AM_SEEKING_AbsolutePositioning,
            NULL, AM_SEEKING_NoPositioning);
    }
}