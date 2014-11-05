// Display.cpp : implementation file
//

#include "stdafx.h"
#include "SettingsUI.h"
#include "Display.h"
#include "afxdialogex.h"


// Display dialog

IMPLEMENT_DYNAMIC(Display, CPropertyPage)

Display::Display() :
CPropertyPage(Display::IDD) {
}

Display::~Display()
{
}

void Display::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Display, CPropertyPage)
END_MESSAGE_MAP()


// Display message handlers
