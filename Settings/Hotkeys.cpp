#include "Hotkeys.h"

#include "../3RVX/Logger.h"
#include "../3RVX/Settings.h"
#include "../3RVX/SkinInfo.h"

#include "UIContext.h"
#include "resource.h"

DLGPROC Hotkeys::Command(unsigned short nCode, unsigned short ctrlId) {
    switch (nCode) {
    case BN_CLICKED:
        break;

    case CBN_SELCHANGE:
    }

    return FALSE;
}

DLGPROC Hotkeys::Notification(NMHDR *nHdr) {
    return FALSE;
}

void Hotkeys::LoadSettings() {
    Settings *settings = Settings::Instance();
}

void Hotkeys::SaveSettings() {
    if (_hWnd == NULL) {
        return;
    }

    CLOG(L"Saving: Hotkeys");
    Settings *settings = Settings::Instance();

}

