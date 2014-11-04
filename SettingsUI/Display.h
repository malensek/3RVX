#pragma once


// Display dialog

class Display : public CPropertyPage
{
	DECLARE_DYNAMIC(Display)

public:
	Display();
	virtual ~Display();

// Dialog Data
	enum { IDD = IDD_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
