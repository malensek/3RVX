#include "Slider.h"

#include <CommCtrl.h>

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
