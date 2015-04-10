#pragma once

#include "Tab.h"

class Hotkeys : public Tab {
public:
    virtual void SaveSettings();

private:
    virtual DLGPROC Command(unsigned short nCode, unsigned short ctrlId);
    virtual DLGPROC Notification(NMHDR *nHdr);

    virtual void Initialize();
    virtual void LoadSettings();
};