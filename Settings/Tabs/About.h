#pragma once

#include "Tab.h"

class About : public Tab {
public:
    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    /* Controls: */
    Label _title;
};