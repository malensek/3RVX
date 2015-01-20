#pragma once
#include "afxwin.h"

#include "KeyReceiver.h"

class HotkeyPrompt : public CDialog, KeyReceiver {

	DECLARE_DYNAMIC(HotkeyPrompt)

public:
	HotkeyPrompt(CWnd* pParent = NULL);   // standard constructor
	virtual ~HotkeyPrompt();
    virtual BOOL OnClose();

    virtual void ReceiveKeys(int combination);

	enum { IDD = IDD_HOTKEYPROMPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
    CStatic _keys;
    CButton _save;
};
