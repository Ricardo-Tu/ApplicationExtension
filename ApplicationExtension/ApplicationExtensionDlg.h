
// ApplicationExtensionDlg.h : header file
//

#pragma once

class CApplicationExtensionDlgAutoProxy;


// CApplicationExtensionDlg dialog
class CApplicationExtensionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CApplicationExtensionDlg);
	friend class CApplicationExtensionDlgAutoProxy;

// Construction
public:
	CApplicationExtensionDlg(CWnd* pParent = nullptr);	// standard constructor
	virtual ~CApplicationExtensionDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPLICATIONEXTENSION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CApplicationExtensionDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
};
