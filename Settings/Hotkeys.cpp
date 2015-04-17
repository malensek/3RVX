#include "Hotkeys.h"

#include <CommCtrl.h>

#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "../3RVX/SkinInfo.h"

#include "UIContext.h"
#include "resource.h"

void Hotkeys::Initialize() {
    INIT_CONTROL(LST_KEYS, ListBox, _keyList);
    INIT_CONTROL(BTN_ADD, Button, _add);
    INIT_CONTROL(BTN_REMOVE, Button, _remove);

    INIT_CONTROL(BTN_KEYS, Button, _keys);
    INIT_CONTROL(CMB_ACTION, ComboBox, _action);
}

void Hotkeys::LoadSettings() {
    Settings *settings = Settings::Instance();

    /* Make highlighted items span the entire row in the list view */
    _ctxt->AddWindowExStyle(LST_KEYS, LVS_EX_FULLROWSELECT);

    RECT dims = _ctxt->GetWindowDimensions(LST_KEYS);
    int width = dims.right - dims.left;

    _ctxt->AddListColumn(LST_KEYS, 0, L"Hotkeys", (int) (width * .485));
    _ctxt->AddListColumn(LST_KEYS, 0, L"Action", (int) (width * .445));
}

void Hotkeys::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    CLOG(L"Saving: Hotkeys");
    Settings *settings = Settings::Instance();
}

