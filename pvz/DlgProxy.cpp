
// DlgProxy.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "pvz.h"
#include "DlgProxy.h"
#include "pvzDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpvzDlgAutoProxy

IMPLEMENT_DYNCREATE(CpvzDlgAutoProxy, CCmdTarget)

CpvzDlgAutoProxy::CpvzDlgAutoProxy()
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
		ASSERT_KINDOF(CpvzDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CpvzDlg)))
		{
			m_pDialog = reinterpret_cast<CpvzDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CpvzDlgAutoProxy::~CpvzDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CpvzDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CpvzDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CpvzDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_Ipvz to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {e49e3495-9343-466f-ba14-f2d29aefc0cf}
static const IID IID_Ipvz =
{0xe49e3495,0x9343,0x466f,{0xba,0x14,0xf2,0xd2,0x9a,0xef,0xc0,0xcf}};

BEGIN_INTERFACE_MAP(CpvzDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CpvzDlgAutoProxy, IID_Ipvz, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in pch.h of this project
// {2c259e82-0471-4749-b7ca-e7a0595938ae}
IMPLEMENT_OLECREATE2(CpvzDlgAutoProxy, "pvz.Application", 0x2c259e82,0x0471,0x4749,0xb7,0xca,0xe7,0xa0,0x59,0x59,0x38,0xae)


// CpvzDlgAutoProxy message handlers
