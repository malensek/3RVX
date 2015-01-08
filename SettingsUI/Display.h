#pragma once
#include "afxwin.h"
#include "afxcmn.h"

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
    CButton _fullscreen;
    CSpinButtonCtrl _spinDelay;
    CSpinButtonCtrl _spinSpeed;
    CComboBox _position;
    CStatic _lbX;
    CStatic _lbY;
    CEdit _customX;
    CEdit _customY;
    CEdit _delay;
    CEdit _speed;
    CComboBox _animation;

    afx_msg void OnDeltaposDelay(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpeed(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCbnSelchangePosition();
    CComboBox _monitor;
    CSpinButtonCtrl _customEdge;
    CButton _useCustomEdge;
    CEdit _edgeEdit;
    afx_msg void OnBnClickedEdge();
};
