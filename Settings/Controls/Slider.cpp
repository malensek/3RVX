#include "Slider.h"

#include <CommCtrl.h>
int Slider::Position() {
    return (int) SendMessage(_hWnd, TBM_GETPOS, 0, 0);
}

void Slider::Position(int position) {
    SendMessage(_hWnd, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) position);
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
