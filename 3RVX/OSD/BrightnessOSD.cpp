// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "BrightnessOSD.h"

#include "../HotkeyInfo.h"
#include "../Skin/OSDComponent.h"
#include "../Skin/Skin.h"
#include "../Skin/SkinManager.h"

BrightnessOSD::BrightnessOSD() :
OSD(L"3RVX-BrightnessDispatcher"),
_mWnd(L"3RVX-BrightnessOSD", L"3RVX-BrightnessOSD") {

    /*
    SkinManager *skin = SkinManager::Instance();
    _mWnd.BackgroundImage(skin->EjectOSD()->background);
    _mWnd.EnableGlass(skin->EjectOSD()->mask);
    _mWnd.Update();
    */

    OSD::InitMeterWnd(_mWnd);
}

void BrightnessOSD::Hide() {
    _mWnd.Hide(false);
}

void BrightnessOSD::ProcessHotkeys(HotkeyInfo &hki) {
    switch (hki.action) {

    }
}

LRESULT
BrightnessOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return OSD::WndProc(hWnd, message, wParam, lParam);
}

