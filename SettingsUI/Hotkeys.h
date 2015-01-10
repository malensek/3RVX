#pragma once
#include "afxwin.h"
#include "afxcmn.h"

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
    virtual BOOL PreTranslateMessage(PMSG msg);

	DECLARE_MESSAGE_MAP()

private:
    void SelectItem(int idx);

private:
    CButton _keys;
    CListCtrl _list;
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedRemove();
    afx_msg void OnLvnItemchangedKeys(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnBnClickedKeys();
};
