// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SoundPlayer.h"

#pragma comment(lib, "Strmiids.lib") 

#include "Logger.h" // <-- Has to be included here because DirectShow says so...

#include <Windows.h>
#include <dshow.h>

SoundPlayer::SoundPlayer(std::wstring filePath, int repeatLimit) :
_repeatLimit(repeatLimit) {

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
    _ready = false;
    _cv.notify_all();
    if (_thread.joinable()) {
        _thread.join();
    }

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
    if (_repeat < _repeatLimit) {
        ++_repeat;
    }
    _repeatMutex.unlock();
    _cv.notify_all();
}

void SoundPlayer::PlayerThread() {
    long evCode;
    REFERENCE_TIME start = 0;
    std::unique_lock<std::mutex> lock(_mutex);

    while (_ready == true) {
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

bool SoundPlayer::Ready() {
    return _ready;
}

void SoundPlayer::SafeRelease(IUnknown *p) {
    if (p) {
        p->Release();
        p = NULL;
    }
}