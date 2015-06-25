#include "SoundPlayer.h"

#include "Logger.h" // <-- Has to be included here because DirectShow says so...

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
        CLOG(L"Failed to create GraphBuilder");
    }
    
    hr = _graphBuilder->QueryInterface(
        IID_IMediaControl, (void **) &_mediaCtrl);
    if (FAILED(hr)) {
        return;
    }

    hr = _graphBuilder->QueryInterface(
        IID_IMediaEventEx, (void **) &_mediaEv);
    if (FAILED(hr)) {
        return;
    }

    hr = _graphBuilder->QueryInterface(
        IID_IMediaSeeking, (void **) &_mediaSeek);
    if (FAILED(hr)) {
        return;
    }

    hr = _graphBuilder->RenderFile(filePath.c_str(), NULL);
    if (FAILED(hr)) {
        return;
    }

    _ready = true;
    _thread = std::thread(&SoundPlayer::PlayerThread, this);
}

SoundPlayer::~SoundPlayer() {
    SafeRelease(_mediaSeek);
    SafeRelease(_mediaEv);
    SafeRelease(_mediaCtrl);
    SafeRelease(_graphBuilder);
}

void SoundPlayer::Play() {
    if (!_ready) {
        CLOG(L"Error: sound player not ready");
        return;
    }

    _repeatMutex.lock();
    if (_repeat < 4) {
        ++_repeat;
    }
    _repeatMutex.unlock();
    _cv.notify_all();
}

void SoundPlayer::PlayerThread() {
    long evCode;
    REFERENCE_TIME start = 0;
    std::unique_lock<std::mutex> lock(_mutex);

    while (true) {
        _repeatMutex.lock();
        if (_repeat == 0) {
            _repeatMutex.unlock();
            _cv.wait(lock);
            /* To handle spurious wakeups, re-do the repeat check: */
            continue;
        }
        _repeatMutex.unlock();

        _mediaCtrl->Run();
        _mediaEv->WaitForCompletion(INFINITE, &evCode);
        _mediaCtrl->Pause();
        _mediaSeek->SetPositions(
            &start, AM_SEEKING_AbsolutePositioning,
            NULL, AM_SEEKING_NoPositioning);

        _repeatMutex.lock();
        --_repeat;
        _repeatMutex.unlock();
    }
}

void SoundPlayer::SafeRelease(IUnknown *p) {
    if (p) {
        p->Release();
    }
}