#pragma once


// General dialog

class General : public CPropertyPage
{
	DECLARE_DYNAMIC(General)

public:
	General();
	virtual ~General();

// Dialog Data
	enum { IDD = IDD_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
