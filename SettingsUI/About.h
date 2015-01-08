#pragma once

class About : public CPropertyPage {

	DECLARE_DYNAMIC(About)

public:
	About();
	virtual ~About();
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnApply();
	DECLARE_MESSAGE_MAP()
};
