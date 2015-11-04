// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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
