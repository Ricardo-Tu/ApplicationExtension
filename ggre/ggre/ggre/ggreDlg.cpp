
// ggreDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ggre.h"
#include "ggreDlg.h"
#include "afxdialogex.h"

#include "common.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CggreDlg* g_dialog;
CrackRE32* g_crack;

void init_edit();
void init_armor_level();
void init_arm_type();


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CggreDlg dialog



CggreDlg::CggreDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_GGRE_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_MYICON);
}

void CggreDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    HICON hIcon = AfxGetApp()->LoadIconW(IDI_MYICON);

    DDX_Control(pDX, IDC_COMBO1, armor_level);
    DDX_Control(pDX, IDC_COMBO2, arm_type);
}

BEGIN_MESSAGE_MAP(CggreDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_CHECK1, &CggreDlg::OnBnClickedCheck1)
    ON_EN_CHANGE(IDC_EDIT2, &CggreDlg::OnEnChangeEdit2)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CggreDlg::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_CHECK2, &CggreDlg::OnBnClickedCheck2)
    ON_BN_CLICKED(IDC_CHECK3, &CggreDlg::OnBnClickedCheck3)
    ON_CBN_SELCHANGE(IDC_COMBO2, &CggreDlg::OnCbnSelchangeCombo2)
    ON_BN_CLICKED(IDC_BUTTON2, &CggreDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON1, &CggreDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON3, &CggreDlg::OnBnClickedButton3)
    ON_EN_CHANGE(IDC_EDIT3, &CggreDlg::OnEnChangeEdit3)
    ON_EN_CHANGE(IDC_EDIT1, &CggreDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CggreDlg message handlers

BOOL CggreDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    //SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    g_dialog = this;
    g_crack = new CrackRE32(L"makaimura_GG_RE.exe");

    init_edit();
    init_armor_level();
    init_arm_type();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CggreDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CggreDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CggreDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CggreDlg::OnBnClickedCheck1()
{
    // TODO: Add your control notification handler code here
    uint32_t sel = g_dialog->armor_level.GetCurSel();
    if (sel == CB_ERR)
    {
        CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
        return;
    }
    int status = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
    if(status)
    {
        g_dialog->GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
        lock_armor_level();
    }
    else
    {
        cancel_lock_armor_level();
        g_dialog->GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
    }
}

void CggreDlg::OnEnChangeEdit2()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void CggreDlg::OnCbnSelchangeCombo1()
{
    // TODO: Add your control notification handler code here
}

void CggreDlg::OnBnClickedCheck2()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
    if (status)
        lock_invincible();
    else
        cancel_lock_invincible();
}

void CggreDlg::OnBnClickedCheck3()
{
    // TODO: Add your control notification handler code here
    uint32_t sel = g_dialog->arm_type.GetCurSel();
    if (sel == CB_ERR)
    {
        CheckDlgButton(IDC_CHECK3, BST_UNCHECKED);
        return;
    }
    int status = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
    if (status)
    {
        g_dialog->GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
        g_dialog->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
        lock_arm_type();
    }
    else
    {
        cancel_lock_arm_type();
        g_dialog->GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
        g_dialog->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
    }

}

void CggreDlg::OnCbnSelchangeCombo2()
{
    // TODO: Add your control notification handler code here
}

void CggreDlg::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    modify_arm_type();
}

void CggreDlg::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    modify_score();
}

void CggreDlg::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
    modify_time();
}

void CggreDlg::OnEnChangeEdit3()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void CggreDlg::OnEnChangeEdit1()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void init_edit()
{
    g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(L"0");
    g_dialog->GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"0");
}

void init_armor_level()
{
    g_dialog->armor_level.AddString(L"1");
    g_dialog->armor_level.AddString(L"2");
    g_dialog->armor_level.AddString(L"3");
    g_dialog->armor_level.AddString(L"4");
}


void init_arm_type()
{
    g_dialog->arm_type.AddString(L"³¤Ã¬");
    g_dialog->arm_type.AddString(L"Ø°Ê×");
    g_dialog->arm_type.AddString(L"Ê¥Ë®");
    g_dialog->arm_type.AddString(L"·ÉïÚ");
    g_dialog->arm_type.AddString(L"´¸×Ó");

    g_dialog->arm_type.AddString(L"¶ÜÅÆ");
    g_dialog->arm_type.AddString(L"¹­åó");
    g_dialog->arm_type.AddString(L"´ÌÇò");
    g_dialog->arm_type.AddString(L"Òþ²ØÎäÆ÷1");
    g_dialog->arm_type.AddString(L"Òþ²ØÎäÆ÷2");

    g_dialog->arm_type.AddString(L"Òþ²ØÎäÆ÷3");
}
