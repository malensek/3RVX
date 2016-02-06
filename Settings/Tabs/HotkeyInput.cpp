#include "HotkeyInput.h"

#include "../../3RVX/Logger.h"
#include "../Controls/Controls.h"
#include "KeyGrabber.h"

HotkeyInput::HotkeyInput(HWND parent) :
Dialog(parent, MAKEINTRESOURCE(IDD_HOTKEYPROMPT)) {

    _prompt = new Label(LBL_PROMPT, *this);

    _cancel = new Button(BTN_CANCELKEYS, *this);
    _cancel->OnClick = [this]() {
        EndDialog(this->DialogHandle(), 0);
        return true;
    };

}