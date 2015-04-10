#pragma once

#include "Tab.h"

class Hotkeys : public Tab {
public:

private:
    virtual DLGPROC Command(unsigned short nCode, unsigned short ctrlId);
    virtual DLGPROC Notification(NMHDR *nHdr);

    virtual void Initialize();
    virtual void LoadSettings();
    virtual void SaveSettings();
};