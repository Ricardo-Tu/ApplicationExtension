
// DlgProxy.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ApplicationExtension.h"
#include "DlgProxy.h"
#include "ApplicationExtensionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CApplicationExtensionDlgAutoProxy

IMPLEMENT_DYNCREATE(CApplicationExtensionDlgAutoProxy, CCmdTarget)

CApplicationExtensionDlgAutoProxy::CApplicationExtensionDlgAutoProxy()
{
	EnableAutomation();

	// To keep the application running as long as an automation
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CApplicationExtensionDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CApplicationExtensionDlg)))
		{
			m_pDialog = reinterpret_cast<CApplicationExtensionDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CApplicationExtensionDlgAutoProxy::~CApplicationExtensionDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CApplicationExtensionDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CApplicationExtensionDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CApplicationExtensionDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_IApplicationExtension to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {53928aa9-efb3-4c89-b974-123cfa208245}
static const IID IID_IApplicationExtension =
{0x53928aa9,0xefb3,0x4c89,{0xb9,0x74,0x12,0x3c,0xfa,0x20,0x82,0x45}};

BEGIN_INTERFACE_MAP(CApplicationExtensionDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CApplicationExtensionDlgAutoProxy, IID_IApplicationExtension, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in pch.h of this project
// {079c9fdc-aa0f-4ccf-82a7-62e3490867ab}
IMPLEMENT_OLECREATE2(CApplicationExtensionDlgAutoProxy, "ApplicationExtension.Application", 0x079c9fdc,0xaa0f,0x4ccf,0x82,0xa7,0x62,0xe3,0x49,0x08,0x67,0xab)


// CApplicationExtensionDlgAutoProxy message handlers
