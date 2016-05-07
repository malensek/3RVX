// Copyright (c) 2016, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "HotkeyInput.h"

#include "../../3RVX/Logger.h"
#include "../Controls/Controls.h"
#include "KeyGrabber.h"

HotkeyInput::HotkeyInput(HWND parent) :
Dialog(parent, MAKEINTRESOURCE(IDD_HOTKEYPROMPT)) {

}

void HotkeyInput::Initialize() {
    _prompt = new Label(LBL_PROMPT, *this);
    KeyGrabber::Instance()->SetHwnd(DialogHandle());
    KeyGrabber::Instance()->Grab();
}
