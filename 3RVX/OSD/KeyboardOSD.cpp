// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "KeyboardOSD.h"

KeyboardOSD::KeyboardOSD() :
OSD(L"3RVX-KeyOSDDispatcher"),
_mWnd(L"3RVX-KeyboardOSD", L"3RVX-KeyboardOSD") {
    RAWINPUTDEVICE rin[1] = { 0 };
    rin[0].dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
    rin[0].hwndTarget = this->Handle();
    rin[0].usUsage = 6;
    rin[0].usUsagePage = 1;
    BOOL x = RegisterRawInputDevices(&rin[0], 1, sizeof(RAWINPUTDEVICE));
    if (x == FALSE) {
        Logger::LogLastError();
    }
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
    switch (message) {
    case WM_INPUT: {
        HRAWINPUT hri = (HRAWINPUT) lParam;
        UINT pcbSz;

        /* Determine the size of the RAWINPUT structure */
        GetRawInputData((HRAWINPUT) lParam, RID_INPUT,
            NULL, &pcbSz, sizeof(RAWINPUTHEADER));
        LPBYTE lpb = new BYTE[pcbSz];

        /* Retrieve raw input data */
        GetRawInputData((HRAWINPUT) lParam, RID_INPUT,
            lpb, &pcbSz, sizeof(RAWINPUTHEADER));
        RAWINPUT *raw = (RAWINPUT *) lpb;

        USHORT vk = raw->data.keyboard.VKey;
        USHORT flags = raw->data.keyboard.Flags;
        if (vk == VK_CAPITAL && (flags & 0x1)) {
            bool locked = ((GetKeyState(VK_CAPITAL) & 0x1) != 0);
            CLOG(L"lock state: %s", locked ? L"ON" : L"OFF");
        }

		if (vk == VK_NUMLOCK && (flags & 0x1)) {
			bool locked = ((GetKeyState(VK_NUMLOCK) & 0x1) != 0);
			CLOG(L"numlock state: %s", locked ? L"ON" : L"OFF");
		}

        delete[] lpb;
        break;
    }
    }

    return OSD::WndProc(hWnd, message, wParam, lParam);
}
