#include "SoundPlayer.h"

#include <Windows.h>
#include <dshow.h>
#pragma comment(lib, "Strmiids.lib") 

SoundPlayer::SoundPlayer(std::wstring filePath) {

    HRESULT hr;

    hr = CoCreateInstance(
        CLSID_FilterGraph,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder,
        (void **) &_graphBuilder);

    if (FAILED(hr)) {
        OutputDebugString(L"gb failed");
    }
    
    hr = _graphBuilder->QueryInterface(IID_IMediaControl, (void **) &_mediaCtrl);
    hr = _graphBuilder->QueryInterface(IID_IMediaEventEx, (void **) &_mediaEv);
    hr = _graphBuilder->QueryInterface(IID_IMediaSeeking, (void **) &_mediaSeek);
    hr = _graphBuilder->RenderFile(filePath.c_str(), NULL);

    _thread = std::thread(&SoundPlayer::PlayerThread, this);
}

SoundPlayer::~SoundPlayer() {
    _mediaSeek->Release();
    _mediaEv->Release();
    _mediaCtrl->Release();
    _graphBuilder->Release();
}

bool SoundPlayer::Play() {
    if (_playing) {
        return false;
    }

    _mutex.lock();
    _playing = true;
    _cv.notify_all();
    _mutex.unlock();
    return true;
}

void SoundPlayer::PlayerThread() {
    long evCode;
    REFERENCE_TIME start = 0;
    std::unique_lock<std::mutex> lock(_mutex);

    while (true) {
        _cv.wait(lock);

        if (!_playing) {
            continue;
        }

        _mediaCtrl->Run();
        _mediaEv->WaitForCompletion(INFINITE, &evCode);
        _mediaCtrl->Pause();
        _mediaSeek->SetPositions(
            &start, AM_SEEKING_AbsolutePositioning,
            NULL, AM_SEEKING_NoPositioning);

        _playing = false;
    }
}