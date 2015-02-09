#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <vector>

#include "HotkeyInfo.h"

class Hotkeys : public CPropertyPage {
	DECLARE_DYNAMIC(Hotkeys)

public:
	Hotkeys();
	virtual ~Hotkeys();

	enum { IDD = IDD_HOTKEYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
    int _selIdx;
    std::vector<HotkeyInfo> _keyInfo;
    void SelectItem(int idx);
    void LoadSelection(int idx);

private:
    CComboBox _action;
    CButton _keys;
    CListCtrl _list;
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedKeys();
    afx_msg void OnBnClickedRemove();
    afx_msg void OnCbnSelchangeAction();
    afx_msg void OnLvnItemchangedKeys(NMHDR *pNMHDR, LRESULT *pResult);
};
