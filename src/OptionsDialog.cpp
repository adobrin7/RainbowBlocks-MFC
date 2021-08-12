#include "pch.h"
#include "OptionsDialog.h"
#include "afxdialogex.h"

// COptionsDialog

IMPLEMENT_DYNAMIC(COptionsDialog, CDialog)

COptionsDialog::COptionsDialog(bool bRowColumn, CWnd* pParent)
	:
	CDialog(IDD_DIALOG_OPTIONS, pParent),
	m_nValueUpside(0),
	m_nValueDownside(0),
	m_bRowColumnDialog(bRowColumn)
{
}

COptionsDialog::~COptionsDialog()
{
}

void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT_UPSIDE, m_ctrlStaticTextUpside);
	DDX_Control(pDX, IDC_STATIC_TEXT_DOWNSIDE, m_ctrlStaticTextDownside);
	DDX_Text(pDX, IDC_EDIT_VALUE_UPSIDE, m_nValueUpside);
	DDX_Text(pDX, IDC_EDIT_VALUE_DOWNSIDE, m_nValueDownside);
}

BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULTS, &COptionsDialog::OnBnClickedButtonDefaults)
END_MESSAGE_MAP()

// Implementation

void COptionsDialog::OnBnClickedButtonDefaults()
{
	if (m_bRowColumnDialog)
		m_nValueUpside = m_nValueDownside = 15; 
	else
		m_nValueUpside = m_nValueDownside = 35;

	UpdateData(false);
}

BOOL COptionsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString upsideMessage, downsideMessage;
	CString windowName;

	if (m_bRowColumnDialog)
	{
		windowName.Format(_T("Update Board Size"));
		upsideMessage.Format(_T("Rows"));
		downsideMessage.Format(_T("Columns"));
	}
	else
	{
		windowName.Format(_T("Update Block Size"));
		upsideMessage.Format(_T("Width"));
		downsideMessage.Format(_T("Height"));
	}
	SetWindowText(windowName);
	m_ctrlStaticTextUpside.SetWindowText(upsideMessage);
	m_ctrlStaticTextDownside.SetWindowText(downsideMessage);

	return TRUE;  
}
