#include "EditBox.h"

void EditBox::Clear() {
    Text(L"");
}

BOOL CALLBACK EditBox::Command(unsigned short nCode) {
    switch (nCode) {
    case EN_CHANGE:
        if (OnTextChange) {
            return OnTextChange();
        }
    }

    return FALSE;
}
