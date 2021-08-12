#pragma once

class COptionsDialog : public CDialog
{
public:
	COptionsDialog(bool bRowColumn, CWnd* pParent = nullptr);
	virtual ~COptionsDialog();
	DECLARE_DYNAMIC(COptionsDialog)
	DECLARE_MESSAGE_MAP()

// Override
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Implementation
public:
	CStatic m_ctrlStaticTextUpside;
	CStatic m_ctrlStaticTextDownside;

	int m_nValueUpside;
	int m_nValueDownside;

	bool m_bRowColumnDialog;

	afx_msg void OnBnClickedButtonDefaults();
};
