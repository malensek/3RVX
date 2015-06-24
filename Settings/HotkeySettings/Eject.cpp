#include "Eject.h"

Eject::Eject(Hotkeys *hk) :
HotkeySettings(hk) {

}

void Eject::LoadHotkeyInfo(HotkeyInfo &hki) {
    /* List action description */
    if (hki.HasArg(0)) {
        ActionDescription(
            _translator->TranslateAndReplace(
            L"Eject Drive: {1}",
            hki.args[0]));
    } else {
        ActionDescription(_translator->Translate(L"Eject Drive"));
    }


}
