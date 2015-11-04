// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "ProgressBar.h"

void ProgressBar::Range(long min, long max) {
    SendMessage(_hWnd, PBM_SETRANGE32, min, max);
}

void ProgressBar::Position(long pos) {
    SendMessage(_hWnd, PBM_SETPOS, pos, 0);
}

void ProgressBar::Marquee(bool enabled, int refresh) {
    if (enabled) {
        AddWindowAttribute(GWL_STYLE, PBS_MARQUEE);
    } else {
        RemoveWindowAttribute(GWL_STYLE, PBS_MARQUEE);
    }

    SendMessage(_hWnd, PBM_SETMARQUEE, enabled, refresh);
}

IFACEMETHODIMP ProgressBar::OnProgress(
        unsigned long ulProgress, unsigned long ulProgressMax,
        unsigned long ulStatusCode, LPCWSTR szStatusText) {

    if (_progressMax == 0 && ulProgressMax != 0) {
        _progressMax = ulProgressMax;
        Marquee(false);
        Range((long) 0, (long) ulProgressMax);
    }

    if (ulProgress > 0) {
        Position((long) ulProgress);
    }

    return StatusCallback::OnProgress(
        ulProgress,
        ulProgressMax,
        ulStatusCode,
        szStatusText);
}