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
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
    CButton _keys;
    CListCtrl _list;
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedRemove();
    afx_msg void OnLvnItemchangedKeys(NMHDR *pNMHDR, LRESULT *pResult);
};
