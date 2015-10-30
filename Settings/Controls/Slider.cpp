#include "Slider.h"

void Slider::Buddy(int buddyId, int position) {
    _buddyWnd = GetDlgItem(_parent, buddyId);
    SendMessage(_hWnd, TBM_SETBUDDY, (WPARAM) 0, (LPARAM) _buddyWnd);
}

int Slider::Position() {
    return (int) SendMessage(_hWnd, TBM_GETPOS, 0, 0);
}

void Slider::Position(int position) {
    SendMessage(_hWnd, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) position);
}

void Slider::Range(int lo, int hi) {
    SendMessage(_hWnd, TBM_SETRANGE, (WPARAM) TRUE, MAKELPARAM(lo, hi));
}

BOOL Slider::Notification(NMHDR *nHdr) {
    switch (nHdr->code) {
    case TRBN_THUMBPOSCHANGING:
        if (OnSlide) {
            NMTRBTHUMBPOSCHANGING *pc
                = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(nHdr);
            OnSlide(pc);
        }
    }

    return FALSE;
}
