#include "stdafx.h"
#include "SettingsUI.h"
#include "About.h"
#include "afxdialogex.h"

#include "Settings.h"

IMPLEMENT_DYNAMIC(About, CPropertyPage)

About::About() : CPropertyPage(About::IDD) {

}

About::~About() {

}

BOOL About::OnApply() {
    OutputDebugString(L"-> About\n");
    UIUtils::SaveSettings(*this);
    return CPropertyPage::OnApply();
}

void About::DoDataExchange(CDataExchange* pDX) {
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(About, CPropertyPage)
END_MESSAGE_MAP()
