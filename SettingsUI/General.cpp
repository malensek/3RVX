// General.cpp : implementation file
//

#include "stdafx.h"
#include "SettingsUI.h"
#include "General.h"
#include "afxdialogex.h"


// General dialog

IMPLEMENT_DYNAMIC(General, CPropertyPage)

General::General()
	: CPropertyPage(General::IDD)
{

}

General::~General()
{
}

void General::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(General, CPropertyPage)
END_MESSAGE_MAP()


// General message handlers
