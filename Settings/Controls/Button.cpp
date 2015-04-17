#include "Button.h"

DLGPROC Button::Command(unsigned short nCode) {
    switch (nCode) {
    case BN_CLICKED:
        if (OnClick) {
            return (DLGPROC) OnClick();
        }
    }

    return FALSE;
}
