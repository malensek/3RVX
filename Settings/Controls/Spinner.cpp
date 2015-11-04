// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Spinner.h"

#include <CommCtrl.h>
#include <sstream>

void Spinner::Buddy(int buddyId) {
    _buddyId = buddyId;
    _buddyWnd = GetDlgItem(_parent, buddyId);
}

void Spinner::Enable() {
    EnableWindow(_hWnd, TRUE);
    EnableWindow(_buddyWnd, TRUE);
}

void Spinner::Disable() {
    EnableWindow(_hWnd, FALSE);
    EnableWindow(_buddyWnd, FALSE);
}

void Spinner::Range(int lo, int hi) {
    SendMessage(_hWnd, UDM_SETRANGE32, lo, hi);
}

bool Spinner::Text(std::wstring text) {
    return SetDlgItemText(_parent, _buddyId, text.c_str()) == TRUE;
}

bool Spinner::Text(int value) {
    return SetDlgItemInt(_parent, _buddyId, value, TRUE) == TRUE;
}

std::wstring Spinner::Text() {
    wchar_t text[MAX_EDITSTR];
    GetDlgItemText(_parent, _buddyId, text, MAX_EDITSTR);
    return std::wstring(text);
}

BOOL CALLBACK Spinner::Notification(NMHDR *nHdr) {
    switch (nHdr->code) {
    case UDN_DELTAPOS:
        if (OnSpin) {
            NMUPDOWN *ud = (NMUPDOWN *) nHdr;
            return OnSpin(ud);
        } else {
            return FALSE;
        }
    }

    return FALSE;
}
