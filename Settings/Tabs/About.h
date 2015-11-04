// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "SettingsTab.h"

class About : public SettingsTab {
public:
    About() :
    SettingsTab(NULL, MAKEINTRESOURCE(IDD_ABOUT), L"About") {

    }

    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    /* Controls: */
    Label *_title;
};