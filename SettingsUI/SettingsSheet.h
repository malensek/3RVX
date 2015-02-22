#pragma once

class SettingsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(SettingsSheet)

public:
    SettingsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    SettingsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    virtual ~SettingsSheet();

protected:
    virtual BOOL OnInitDialog();
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};