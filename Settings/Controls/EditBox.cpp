#include "EditBox.h"

void EditBox::Clear() {
    Text(L"");
}

DLGPROC EditBox::Command(unsigned short nCode) {
    switch (nCode) {
    case EN_CHANGE:
        if (OnTextChange) {
            return (DLGPROC) OnTextChange();
        }
    }

    return FALSE;
}
