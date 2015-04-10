#pragma once
#include "Tab.h"

#include <list>
#include <string>

class General : public Tab {
public:
    virtual void SaveSettings();

private:
    std::wstring _url;

    virtual DLGPROC Command(unsigned short nCode, unsigned short ctrlId);
    virtual DLGPROC Notification(NMHDR *nHdr);

    virtual void Initialize();
    virtual void LoadSettings();

    std::list<std::wstring> FindLanguages(std::wstring dir);
    std::list<std::wstring> FindSkins(std::wstring dir);
    void LoadSkinInfo(std::wstring skinName);
    bool RunOnStartup();
    bool RunOnStartup(bool enable);

private:
    /* Controls: */
    Control _author;

private:
    static const wchar_t REGKEY_NAME[];
    static const wchar_t REGKEY_RUN[];
};