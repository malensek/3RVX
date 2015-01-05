#pragma once
#include "afxwin.h"

class Hotkeys : public CPropertyPage
{
	DECLARE_DYNAMIC(Hotkeys)

public:
	Hotkeys();
	virtual ~Hotkeys();

// Dialog Data
	enum { IDD = IDD_HOTKEYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
