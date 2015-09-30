// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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
