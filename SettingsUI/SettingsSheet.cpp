#include "stdafx.h"
#include "SettingsUI.h"
#include "SettingsSheet.h"

IMPLEMENT_DYNAMIC(SettingsSheet, CPropertySheet)

SettingsSheet::SettingsSheet(
    UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) :
CPropertySheet(nIDCaption, pParentWnd, iSelectPage) {

}

SettingsSheet::SettingsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
    : CPropertySheet(pszCaption, pParentWnd, iSelectPage) {

}

SettingsSheet::~SettingsSheet() {
}

BOOL SettingsSheet::OnInitDialog() {
    CPropertySheet::OnInitDialog();
    return TRUE;
}

BEGIN_MESSAGE_MAP(SettingsSheet, CPropertySheet)
END_MESSAGE_MAP()