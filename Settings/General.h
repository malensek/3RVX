#pragma once
#include "Tab.h"

#include <list>
#include <string>

class General : public Tab {
public:

private:
    std::wstring _url;

    virtual void Command(unsigned short nCode, unsigned short ctrlId);
    virtual void LoadSettings();
    virtual void SaveSettings();

    std::list<std::wstring> FindLanguages(std::wstring dir);
    std::list<std::wstring> FindSkins(std::wstring dir);
    void LoadSkinInfo(std::wstring skinName);
    bool RunOnStartup();
    bool RunOnStartup(bool enable);
};