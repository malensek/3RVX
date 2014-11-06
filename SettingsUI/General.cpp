#include "stdafx.h"
#include "SettingsUI.h"
#include "General.h"
#include "afxdialogex.h"

#define KEY_NAME L"3RVXv3"
#define STARTUP_KEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

IMPLEMENT_DYNAMIC(General, CPropertyPage)

General::General(Settings *settings) :
CPropertyPage(General::IDD),
_settings(settings) {
}

General::~General()
{
}

void General::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, CHK_STARTUP, _startup);
    DDX_Control(pDX, CHK_NOTIFY, _notify);
    DDX_Control(pDX, CHK_SOUNDS, _sounds);
    DDX_Control(pDX, CMB_LANG, _lang);
    DDX_Control(pDX, GRP_SKIN, _skinGrp);
    DDX_Control(pDX, GRP_BEHAVIOR, _behaviorGrp);
    DDX_Control(pDX, GRP_LANGUAGE, _languageGrp);
    DDX_Control(pDX, LBL_AUTHOR, _author);
    DDX_Control(pDX, CMB_SKIN, _skins);
}

BOOL General::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    LoadSettings();

    std::list<CString> skins = FindSkins(L"../3RVX/Skins");
    for each (CString skin in skins) {
        _skins.AddString(skin);
    }

    std::wstring current = _settings->GetText("skin");
    int idx = _skins.SelectString(0, current.c_str());
    if (idx == CB_ERR) {
        _skins.SelectString(0, L"Default");
    }

    return TRUE;
}

std::list<CString> General::FindSkins(CString dir) {
    std::list<CString> skins;

    CFileFind ff;
    dir += L"\\*";
    BOOL result = ff.FindFile(dir);
    while (result) {
        result = ff.FindNextFile();
        if (ff.IsDots()) {
            continue;
        }

        if (ff.IsDirectory()) {
            CFileFind inDir;
            CString dirPath = ff.GetFilePath();
            dirPath += L"\\skin.xml";
            if (inDir.FindFile(dirPath)) {
                /* We found a skin XML file; add the skin dir to our list. */
                skins.push_back(ff.GetFileName());
            }
        }
    }

    return skins;
}

void General::LoadSettings() {
    _notify.SetCheck(_settings->IsEnabled("notifyIcon"));
    _sounds.SetCheck(_settings->IsEnabled("soundEffects"));
}

void General::EnableRunOnStartup() {
    wchar_t path[1024];
    GetModuleFileName(NULL, path, 1024);
    PathRemoveFileSpec(path);
    CString exePath(path);
    exePath.Append(L"\\3RVX.exe");

    CRegKey rk;
    int result = rk.Open(HKEY_CURRENT_USER, STARTUP_KEY, KEY_WRITE);
    if (result == ERROR_SUCCESS) {
        rk.SetStringValue(KEY_NAME, exePath, REG_SZ);
    }
}

BEGIN_MESSAGE_MAP(General, CPropertyPage)
END_MESSAGE_MAP()