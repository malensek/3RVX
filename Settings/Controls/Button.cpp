#include "Button.h"

BOOL CALLBACK Button::Command(unsigned short nCode) {
    switch (nCode) {
    case BN_CLICKED:
        if (OnClick) {
            return OnClick();
        }
    }

    return FALSE;
}
