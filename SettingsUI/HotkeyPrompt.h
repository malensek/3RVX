#pragma once


// HotkeyPrompt dialog

class HotkeyPrompt : public CDialog
{
	DECLARE_DYNAMIC(HotkeyPrompt)

public:
	HotkeyPrompt(CWnd* pParent = NULL);   // standard constructor
	virtual ~HotkeyPrompt();

// Dialog Data
	enum { IDD = IDD_HOTKEYPROMPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
    CStatic _keys;
    CButton _save;
};
