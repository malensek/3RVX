#pragma once
#include "afxwin.h"
#include "../3RVX/Settings.h"

class General : public CPropertyPage
{
	DECLARE_DYNAMIC(General)

public:
    General(Settings *settings);
	virtual ~General();

	enum { IDD = IDD_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
    Settings *_settings;
    std::wstring _url;

    void LoadSettings();
    std::list<CString> FindSkins(CString dir);
    std::list<CString> FindLanguages(CString dir);
    void EnableRunOnStartup();

/* Auto-generated members follow*/
public:
    afx_msg void OnBnClickedWebsite();
private:
    CButton _startup;
    CButton _notify;
    CButton _sounds;
    CComboBox _lang;
    CStatic _skinGrp;
    CStatic _behaviorGrp;
    CStatic _languageGrp;
    CStatic _author;
    CButton _website;
    CComboBox _skins;
};
