
// DlgProxy.h: header file
//

#pragma once

class CApplicationExtensionDlg;


// CApplicationExtensionDlgAutoProxy command target

class CApplicationExtensionDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CApplicationExtensionDlgAutoProxy)

	CApplicationExtensionDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CApplicationExtensionDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CApplicationExtensionDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CApplicationExtensionDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

