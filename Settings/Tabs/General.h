// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "SettingsTab.h"

#include <list>
#include <string>

class General : public SettingsTab {
public:
    General() :
    SettingsTab(NULL, MAKEINTRESOURCE(IDD_GENERAL), L"General") {

    }

    virtual void SaveSettings();

private:
    std::wstring _url;

    virtual void Initialize();
    virtual void LoadSettings();

    std::list<std::wstring> FindLanguages(std::wstring dir);
    std::list<std::wstring> FindSkins(std::wstring dir);
    void LoadSkinInfo(std::wstring skinName);
    bool RunOnStartup();
    bool RunOnStartup(bool enable);
    bool CheckForUpdates();

private:
    /* Controls: */
    GroupBox *_behaviorGroup;
    Checkbox *_startup;
    Checkbox *_showStartup;
    Checkbox *_sounds;
    Checkbox *_autoUpdate;
    Button *_checkNow;

    GroupBox *_skinGroup;
    ComboBox *_skin;
    Label *_author;
    Button *_website;

    GroupBox *_languageGroup;
    ComboBox *_language;

private:
    static const wchar_t REGKEY_NAME[];
    static const wchar_t REGKEY_RUN[];
};