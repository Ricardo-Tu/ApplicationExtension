
// DlgProxy.h: header file
//

#pragma once

class CpvzDlg;


// CpvzDlgAutoProxy command target

class CpvzDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CpvzDlgAutoProxy)

	CpvzDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CpvzDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CpvzDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CpvzDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

