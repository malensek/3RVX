#pragma once
#include "afxwin.h"

class Display : public CPropertyPage
{
	DECLARE_DYNAMIC(Display)

public:
	Display();
	virtual ~Display();

	enum { IDD = IDD_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnApply();
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
    CButton _onTop;
};
