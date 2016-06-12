// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "KeyboardOSD.h"

KeyboardOSD::KeyboardOSD() :
OSD(L"3RVX-KeyOSDDispatcher"),
_mWnd(L"3RVX-KeyboardOSD", L"3RVX-KeyboardOSD") {

}

KeyboardOSD::~KeyboardOSD() {

}

void KeyboardOSD::Hide() {

}

void KeyboardOSD::HideIcon() {

}

void KeyboardOSD::ProcessHotkeys(HotkeyInfo &hki) {

}

void KeyboardOSD::OnDisplayChange() {
    InitMeterWnd(_mWnd);
}

LRESULT
KeyboardOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return OSD::WndProc(hWnd, message, wParam, lParam);
}
