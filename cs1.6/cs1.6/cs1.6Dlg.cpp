
// cs1.6Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "cs1.6.h"
#include "cs1.6Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Ccs16Dlg* g_dialog;
std::unique_ptr<game::GameContext> g_pGameContext;
boolean boxperspective_thread_quit_flag;
extern uint32_t boxShowFlag = 0;
boolean bone_flag = 0;

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


// Ccs16Dlg dialog



Ccs16Dlg::Ccs16Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CS16_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_MYICON);
}

void Ccs16Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ccs16Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_RADIO1, &Ccs16Dlg::OnBnClickedRadio1)
    ON_EN_CHANGE(IDC_EDIT1, &Ccs16Dlg::OnEnChangeEdit1)
    ON_BN_CLICKED(IDC_CHECK1, &Ccs16Dlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK2, &Ccs16Dlg::OnBnClickedCheck2)
    ON_BN_CLICKED(IDC_BUTTON1, &Ccs16Dlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_RADIO2, &Ccs16Dlg::OnBnClickedRadio2)
    ON_BN_CLICKED(IDC_CHECK3, &Ccs16Dlg::OnBnClickedCheck3)
    ON_BN_CLICKED(IDC_CHECK5, &Ccs16Dlg::OnBnClickedCheck5)
    ON_BN_CLICKED(IDC_CHECK7, &Ccs16Dlg::OnBnClickedCheck7)
    ON_BN_CLICKED(IDC_BUTTON2, &Ccs16Dlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &Ccs16Dlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &Ccs16Dlg::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_CHECK4, &Ccs16Dlg::OnBnClickedCheck4)
    ON_BN_CLICKED(IDC_CHECK6, &Ccs16Dlg::OnBnClickedCheck6)
    ON_BN_CLICKED(IDC_CHECK8, &Ccs16Dlg::OnBnClickedCheck8)
    ON_BN_CLICKED(IDC_CHECK9, &Ccs16Dlg::OnBnClickedCheck9)
    ON_BN_CLICKED(IDC_BUTTON5, &Ccs16Dlg::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON6, &Ccs16Dlg::OnBnClickedButton6)
    ON_BN_CLICKED(IDC_BUTTON7, &Ccs16Dlg::OnBnClickedButton7)
    ON_BN_CLICKED(IDC_BUTTON8, &Ccs16Dlg::OnBnClickedButton8)
    ON_BN_CLICKED(IDC_BUTTON9, &Ccs16Dlg::OnBnClickedButton9)
    ON_BN_CLICKED(IDC_BUTTON10, &Ccs16Dlg::OnBnClickedButton10)
    ON_BN_CLICKED(IDC_CHECK10, &Ccs16Dlg::OnBnClickedCheck10)
    ON_BN_CLICKED(IDC_CHECK11, &Ccs16Dlg::OnBnClickedCheck11)
    ON_BN_CLICKED(IDC_CHECK12, &Ccs16Dlg::OnBnClickedCheck12)
END_MESSAGE_MAP()


// Ccs16Dlg message handlers

BOOL Ccs16Dlg::OnInitDialog()
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
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    g_pGameContext = std::make_unique<game::GameContext>(L"cstrike.exe");
    g_dialog = this;
    game::InitAllControlsContext();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ccs16Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Ccs16Dlg::OnPaint()
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
HCURSOR Ccs16Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void Ccs16Dlg::OnBnClickedRadio1()
{
    // TODO: Add your control notification handler code here
    int nSelectId = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
    if (nSelectId == IDC_RADIO1)
        boxShowFlag = 0;
    else if (nSelectId == IDC_RADIO2)
        boxShowFlag = 1;
}


void Ccs16Dlg::OnEnChangeEdit1()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


void Ccs16Dlg::OnBnClickedCheck1()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();

    if (status)
    {
        this->GetDlgItem(IDC_EDIT1)->EnableWindow(false);
        game::SetMoneyTimer();
    }
    else
    {
        this->GetDlgItem(IDC_EDIT1)->EnableWindow(true);
        game::CancelSetMoneyTimer();
    }
}

void Ccs16Dlg::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    game::SetMoney();
}

void Ccs16Dlg::OnBnClickedCheck2()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
    if (status)
    {
        ((CButton*)g_dialog->GetDlgItem(IDC_RADIO1))->EnableWindow(true);
        ((CButton*)g_dialog->GetDlgItem(IDC_RADIO2))->EnableWindow(true);
        ((CButton*)g_dialog->GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
        boxperspective_thread_quit_flag = false;
        std::thread bp_thread(game::boxperspective_thread);
        bp_thread.detach();
    }
    else
    {
        ((CButton*)g_dialog->GetDlgItem(IDC_RADIO1))->SetCheck(BST_UNCHECKED);
        ((CButton*)g_dialog->GetDlgItem(IDC_RADIO1))->EnableWindow(false);
        ((CButton*)g_dialog->GetDlgItem(IDC_RADIO2))->EnableWindow(false);
        boxperspective_thread_quit_flag = true;
    }
}

void Ccs16Dlg::OnBnClickedRadio2()
{
    // TODO: Add your control notification handler code here
        // TODO: Add your control notification handler code here
    int nSelectId = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
    if (nSelectId == IDC_RADIO1)
        boxShowFlag = 0;
    else if (nSelectId == IDC_RADIO2)
        boxShowFlag = 1;
}


void Ccs16Dlg::OnBnClickedCheck3()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();

    if (status)
    {
        this->GetDlgItem(IDC_EDIT2)->EnableWindow(false);
        game::SetBloodTimer();
    }
    else
    {
        this->GetDlgItem(IDC_EDIT2)->EnableWindow(true);
        game::CancelSetBloodTimer();
    }
}


void Ccs16Dlg::OnBnClickedCheck5()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();

    if (status)
    {
        this->GetDlgItem(IDC_EDIT3)->EnableWindow(false);
        game::SetArmorTimer();
    }
    else
    {
        this->GetDlgItem(IDC_EDIT3)->EnableWindow(true);
        game::CancelSetArmorTimer();
    }
}


void Ccs16Dlg::OnBnClickedCheck7()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();

    if (status)
    {
        this->GetDlgItem(IDC_EDIT4)->EnableWindow(false);
        game::SetBulletTimer();
    }
    else
    {
        this->GetDlgItem(IDC_EDIT4)->EnableWindow(true);
        game::CancelSetBulletTimer();
    }
}


void Ccs16Dlg::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    game::SetBlood();
}


void Ccs16Dlg::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
    game::SetArmor();
}


void Ccs16Dlg::OnBnClickedButton4()
{
    // TODO: Add your control notification handler code here
    game::SetBullet();
}


void Ccs16Dlg::OnBnClickedCheck4()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();

    if (status)
    {
        game::SetInvincibility();
    }
    else
    {
        game::CancelInvincibility();
    }
}


void Ccs16Dlg::OnBnClickedCheck6()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();

    if (status)
    {
        game::SetBuyEveryWhere();
    }
    else
    {
        game::CancelBuyEveryWhere();
    }
}



void Ccs16Dlg::OnBnClickedCheck8()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();

    if (status)
    {
        game::SetMouseLeftButtonColdDown();
    }
    else
    {
        game::CancelMouseLeftButtonColdDown();
    }
}


void Ccs16Dlg::OnBnClickedCheck9()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();

    if (status)
    {
        game::SetNoBackSeat();
    }
    else
    {
        game::CancelNoBackSeat();
    }
}


void Ccs16Dlg::OnBnClickedButton5()
{
    // TODO: Add your control notification handler code here
    game::SavePostion(myPostion[0]);
}



void Ccs16Dlg::OnBnClickedButton6()
{
    // TODO: Add your control notification handler code here
    game::LoadPostion(myPostion[0]);
}




void Ccs16Dlg::OnBnClickedButton7()
{
    // TODO: Add your control notification handler code here
    game::SavePostion(myPostion[1]);
}


void Ccs16Dlg::OnBnClickedButton8()
{
    // TODO: Add your control notification handler code here
    game::LoadPostion(myPostion[1]);
}


void Ccs16Dlg::OnBnClickedButton9()
{
    // TODO: Add your control notification handler code here
    game::SavePostion(myPostion[2]);
}


void Ccs16Dlg::OnBnClickedButton10()
{
    // TODO: Add your control notification handler code here
    game::LoadPostion(myPostion[2]);
}


void Ccs16Dlg::OnBnClickedCheck10()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();

    if (status)
    {
        game::SetSuperMan();
    }
    else
    {
        game::CancelSuperMan();
    }
}


void Ccs16Dlg::OnBnClickedCheck11()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck();

    if (status)
    {
        game::SetNightVision();
    }
    else
    {
        game::CancelNightVision();
    }
}


void Ccs16Dlg::OnBnClickedCheck12()
{
    // TODO: Add your control notification handler code here
    int status = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck();

    if (status)
        bone_flag = 1;
    else
        bone_flag = 0;
}
