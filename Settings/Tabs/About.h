#pragma once

#include "SettingsTab.h"

class About : public SettingsTab {
public:
    About(HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title = L"") :
    SettingsTab(hInstance, tabTemplate, title) {

    }

    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    /* Controls: */
    Label *_title;
};