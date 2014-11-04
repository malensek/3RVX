// SettingsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SettingsUI.h"
#include "SettingsSheet.h"


// SettingsSheet

IMPLEMENT_DYNAMIC(SettingsSheet, CPropertySheet)

SettingsSheet::SettingsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

SettingsSheet::SettingsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

SettingsSheet::~SettingsSheet()
{
}

BEGIN_MESSAGE_MAP(SettingsSheet, CPropertySheet)
END_MESSAGE_MAP()


// SettingsSheet message handlers
