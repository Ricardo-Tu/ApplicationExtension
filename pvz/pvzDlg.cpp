
// pvzDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "pvz.h"
#include "pvzDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

#include "common.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Q_HOT_KEY_ID 0x123

CpvzDlg* g_dialog;

CrackRE32* g_crack;

BOOLEAN bBulletFlag = FALSE;
LONG AllBulletFlag = FALSE;

VOID initCardSlot_comboBox();

VOID initPlants_comboBox();

VOID initPlantRow();

VOID initPlantList();

VOID initPlant2();

VOID initZombiesRow();

VOID initZombiesList();

VOID initZembies();

VOID initCornBullet();

VOID initBulletClass();

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
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpvzDlg dialog


IMPLEMENT_DYNAMIC(CpvzDlg, CDialogEx);

CpvzDlg::CpvzDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PVZ_DIALOG, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MYICON);
	m_pAutoProxy = nullptr;
}

CpvzDlg::~CpvzDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to null, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void CpvzDlg::DoDataExchange(CDataExchange* pDX)
{
	HICON hIcon = AfxGetApp()->LoadIconW(IDI_MYICON);

	SetIcon(hIcon, FALSE);

	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cardslot_combobox);
	DDX_Control(pDX, IDC_COMBO2, plants_comboBox);
	DDX_Control(pDX, IDC_COMBO3, plant_row);
	DDX_Control(pDX, IDC_COMBO4, plant_list);
	DDX_Control(pDX, IDC_COMBO5, plants2);
	DDX_Control(pDX, IDC_COMBO6, zombies_row);
	DDX_Control(pDX, IDC_COMBO7, zombies_list);
	DDX_Control(pDX, IDC_COMBO8, zombies_class);
	DDX_Control(pDX, IDC_COMBO9, corn_bullet);
	DDX_Control(pDX, IDC_COMBO10, BulletClass);
}

BEGIN_MESSAGE_MAP(CpvzDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_HOTKEY, &CpvzDlg::OnHotKey)
	ON_BN_CLICKED(IDC_BUTTON1, &CpvzDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CpvzDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK1, &CpvzDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, &CpvzDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK2, &CpvzDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CpvzDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CpvzDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CpvzDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CpvzDlg::OnBnClickedCheck6)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CpvzDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &CpvzDlg::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CpvzDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON4, &CpvzDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK7, &CpvzDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_BUTTON5, &CpvzDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK8, &CpvzDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &CpvzDlg::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_BUTTON7, &CpvzDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_CHECK10, &CpvzDlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK11, &CpvzDlg::OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_CHECK12, &CpvzDlg::OnBnClickedCheck12)
	ON_BN_CLICKED(IDC_CHECK13, &CpvzDlg::OnBnClickedCheck13)
	ON_BN_CLICKED(IDC_CHECK14, &CpvzDlg::OnBnClickedCheck14)
	ON_BN_CLICKED(IDC_BUTTON6, &CpvzDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON9, &CpvzDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_CHECK15, &CpvzDlg::OnBnClickedCheck15)
	ON_BN_CLICKED(IDC_BUTTON10, &CpvzDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_CHECK16, &CpvzDlg::OnBnClickedCheck16)
	ON_BN_CLICKED(IDC_CHECK17, &CpvzDlg::OnBnClickedCheck17)
	ON_BN_CLICKED(IDC_CHECK18, &CpvzDlg::OnBnClickedCheck18)
	ON_BN_CLICKED(IDC_CHECK19, &CpvzDlg::OnBnClickedCheck19)
	ON_BN_CLICKED(IDC_CHECK20, &CpvzDlg::OnBnClickedCheck20)
	ON_BN_CLICKED(IDC_CHECK22, &CpvzDlg::OnBnClickedCheck22)
	ON_BN_CLICKED(IDC_CHECK21, &CpvzDlg::OnBnClickedCheck21)
	ON_BN_CLICKED(IDC_CHECK23, &CpvzDlg::OnBnClickedCheck23)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CpvzDlg::OnCbnSelchangeCombo8)
	ON_CBN_SELCHANGE(IDC_COMBO10, &CpvzDlg::OnCbnSelchangeCombo10)
	ON_BN_CLICKED(IDC_BUTTON12, &CpvzDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON11, &CpvzDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON8, &CpvzDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON13, &CpvzDlg::OnBnClickedButton13)
	ON_CBN_SELCHANGE(IDC_COMBO6, &CpvzDlg::OnCbnSelchangeCombo6)
END_MESSAGE_MAP()


// CpvzDlg message handlers

BOOL CpvzDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here
	CString szEditString;

	g_dialog = this;

	g_crack = new CrackRE32(L"PlantsVsZombies.exe");

	szEditString.Format(L"8000");

	this->GetDlgItem(IDC_EDIT1)->SetWindowTextW(szEditString);

	this->GetDlgItem(IDC_EDIT3)->SetWindowTextW(szEditString);

	// Init card slot plant modify combo box.
	initCardSlot_comboBox();
	initPlants_comboBox();

	// Init plant combo box.
	initPlantRow();
	initPlantList();
	initPlant2();

	// Init zombies combo box.
	initZombiesRow();
	initZombiesList();
	initZembies();

	initCornBullet();
	initBulletClass();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CpvzDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CpvzDlg::OnPaint()
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
HCURSOR CpvzDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CpvzDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CpvzDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CpvzDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CpvzDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CpvzDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	ModifySunShine();
}


void CpvzDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CpvzDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	if (status)
		LockSunShine();
	else
		RecoverLockSunShineShineOriginalCode();
}


void CpvzDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if (!g_crack->InitGameInformation(L"PlantsVsZombies.exe"))
	{
		AfxMessageBox(L"Refresh game information failed!\n");
	}
}


void CpvzDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();

	if (status)
		DecreaseCardCooldown();
	else
		RecoverCardCooldownSourceCode();

}


void CpvzDlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();

	if (status)
		NoCardCooldown();
	else
		CancelNoCardCooldown();
}


void CpvzDlg::OnBnClickedCheck4()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();

	if (status)
		AutomaticallyCollectionSunShine();
	else
		RecoverAutomaticallyCollectionSunShine();

}




void CpvzDlg::OnBnClickedCheck5()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();

	if (status)
		Stregthenbullet();
	else
		RecoverStrengthenbullet();

}


void CpvzDlg::OnBnClickedCheck6()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();

	if (status)
		BTBullet();
	else
		RecoverBTBullet();

}

VOID initCardSlot_comboBox()
{
	g_dialog->cardslot_combobox.AddString(L"卡槽0");
	g_dialog->cardslot_combobox.AddString(L"卡槽1");
	g_dialog->cardslot_combobox.AddString(L"卡槽2");
	g_dialog->cardslot_combobox.AddString(L"卡槽3");
	g_dialog->cardslot_combobox.AddString(L"卡槽4");

	g_dialog->cardslot_combobox.AddString(L"卡槽5");
	g_dialog->cardslot_combobox.AddString(L"卡槽6");
	g_dialog->cardslot_combobox.AddString(L"卡槽7");
	g_dialog->cardslot_combobox.AddString(L"卡槽8");
	g_dialog->cardslot_combobox.AddString(L"卡槽9");
}

VOID initPlants_comboBox()
{
	// 0
	g_dialog->plants_comboBox.AddString(L"豌豆射手");
	g_dialog->plants_comboBox.AddString(L"向日葵");
	g_dialog->plants_comboBox.AddString(L"樱桃炸弹");
	g_dialog->plants_comboBox.AddString(L"坚果墙");
	g_dialog->plants_comboBox.AddString(L"土豆雷");
	g_dialog->plants_comboBox.AddString(L"寒冰射手");
	g_dialog->plants_comboBox.AddString(L"大嘴花");
	g_dialog->plants_comboBox.AddString(L"双发射手");

	// 8 
	g_dialog->plants_comboBox.AddString(L"小喷菇");
	g_dialog->plants_comboBox.AddString(L"阳光菇");
	g_dialog->plants_comboBox.AddString(L"大喷菇");
	g_dialog->plants_comboBox.AddString(L"墓碑吞噬者");
	g_dialog->plants_comboBox.AddString(L"魅惑菇");
	g_dialog->plants_comboBox.AddString(L"胆小菇");
	g_dialog->plants_comboBox.AddString(L"寒冰菇");
	g_dialog->plants_comboBox.AddString(L"毁灭菇");

	// 16
	g_dialog->plants_comboBox.AddString(L"睡莲");
	g_dialog->plants_comboBox.AddString(L"窝瓜");
	g_dialog->plants_comboBox.AddString(L"三线射手");
	g_dialog->plants_comboBox.AddString(L"缠绕海草");
	g_dialog->plants_comboBox.AddString(L"火爆辣椒");
	g_dialog->plants_comboBox.AddString(L"地刺");
	g_dialog->plants_comboBox.AddString(L"火炬树桩");
	g_dialog->plants_comboBox.AddString(L"高坚果");

	// 24
	g_dialog->plants_comboBox.AddString(L"海蘑菇");
	g_dialog->plants_comboBox.AddString(L"路灯花");
	g_dialog->plants_comboBox.AddString(L"仙人掌");
	g_dialog->plants_comboBox.AddString(L"三叶草");
	g_dialog->plants_comboBox.AddString(L"裂荚射手");
	g_dialog->plants_comboBox.AddString(L"杨桃");
	g_dialog->plants_comboBox.AddString(L"南瓜头");
	g_dialog->plants_comboBox.AddString(L"磁力菇");

	// 32
	g_dialog->plants_comboBox.AddString(L"卷心菜投手");
	g_dialog->plants_comboBox.AddString(L"花盆");
	g_dialog->plants_comboBox.AddString(L"玉米投手");
	g_dialog->plants_comboBox.AddString(L"咖啡豆");
	g_dialog->plants_comboBox.AddString(L"大蒜");
	g_dialog->plants_comboBox.AddString(L"叶子保护伞");
	g_dialog->plants_comboBox.AddString(L"金盏花");
	g_dialog->plants_comboBox.AddString(L"西瓜投手");

	// 40
	g_dialog->plants_comboBox.AddString(L"机枪射手");
	g_dialog->plants_comboBox.AddString(L"双子向日葵");
	g_dialog->plants_comboBox.AddString(L"忧郁蘑菇");
	g_dialog->plants_comboBox.AddString(L"香蒲");
	g_dialog->plants_comboBox.AddString(L"冰瓜");
	g_dialog->plants_comboBox.AddString(L"吸金磁");
	g_dialog->plants_comboBox.AddString(L"地刺王");
	g_dialog->plants_comboBox.AddString(L"玉米加农炮");

	// 48
	g_dialog->plants_comboBox.AddString(L"模仿者");
	g_dialog->plants_comboBox.AddString(L"爆炸坚果");
	g_dialog->plants_comboBox.AddString(L"大坚果保龄球");
	g_dialog->plants_comboBox.AddString(L"种子芽");
	g_dialog->plants_comboBox.AddString(L"反向双发射手");
}


VOID initPlantRow()
{
	g_dialog->plant_row.AddString(L"第0行");
	g_dialog->plant_row.AddString(L"第1行");
	g_dialog->plant_row.AddString(L"第2行");
	g_dialog->plant_row.AddString(L"第3行");
	g_dialog->plant_row.AddString(L"第4行");
	g_dialog->plant_row.AddString(L"第5行");

	g_dialog->plant_row.SetCurSel(0);
}

VOID initPlantList()
{
	g_dialog->plant_list.AddString(L"第0列");
	g_dialog->plant_list.AddString(L"第1列");
	g_dialog->plant_list.AddString(L"第2列");
	g_dialog->plant_list.AddString(L"第3列");
	g_dialog->plant_list.AddString(L"第4列");

	g_dialog->plant_list.AddString(L"第5列");
	g_dialog->plant_list.AddString(L"第6列");
	g_dialog->plant_list.AddString(L"第7列");
	g_dialog->plant_list.AddString(L"第8列");

	g_dialog->plant_list.SetCurSel(0);
}

VOID initPlant2()
{
	g_dialog->plants2.AddString(L"豌豆射手");
	g_dialog->plants2.AddString(L"向日葵");
	g_dialog->plants2.AddString(L"樱桃炸弹");
	g_dialog->plants2.AddString(L"坚果墙");
	g_dialog->plants2.AddString(L"土豆雷");
	g_dialog->plants2.AddString(L"寒冰射手");
	g_dialog->plants2.AddString(L"大嘴花");
	g_dialog->plants2.AddString(L"双发射手");

	g_dialog->plants2.AddString(L"小喷菇");
	g_dialog->plants2.AddString(L"阳光菇");
	g_dialog->plants2.AddString(L"大喷菇");
	g_dialog->plants2.AddString(L"墓碑吞噬者");
	g_dialog->plants2.AddString(L"魅惑菇");
	g_dialog->plants2.AddString(L"胆小菇");
	g_dialog->plants2.AddString(L"寒冰菇");
	g_dialog->plants2.AddString(L"毁灭菇");

	g_dialog->plants2.AddString(L"睡莲");
	g_dialog->plants2.AddString(L"窝瓜");
	g_dialog->plants2.AddString(L"三线射手");
	g_dialog->plants2.AddString(L"缠绕海草");
	g_dialog->plants2.AddString(L"火爆辣椒");
	g_dialog->plants2.AddString(L"地刺");
	g_dialog->plants2.AddString(L"火炬树桩");
	g_dialog->plants2.AddString(L"高坚果");

	g_dialog->plants2.AddString(L"海蘑菇");
	g_dialog->plants2.AddString(L"路灯花");
	g_dialog->plants2.AddString(L"仙人掌");
	g_dialog->plants2.AddString(L"三叶草");
	g_dialog->plants2.AddString(L"裂荚射手");
	g_dialog->plants2.AddString(L"杨桃");
	g_dialog->plants2.AddString(L"南瓜头");
	g_dialog->plants2.AddString(L"磁力菇");

	g_dialog->plants2.AddString(L"卷心菜投手");
	g_dialog->plants2.AddString(L"花盆");
	g_dialog->plants2.AddString(L"玉米投手");
	g_dialog->plants2.AddString(L"咖啡豆");
	g_dialog->plants2.AddString(L"大蒜");
	g_dialog->plants2.AddString(L"叶子保护伞");
	g_dialog->plants2.AddString(L"金盏花");
	g_dialog->plants2.AddString(L"西瓜投手");

	g_dialog->plants2.AddString(L"机枪射手");
	g_dialog->plants2.AddString(L"双子向日葵");
	g_dialog->plants2.AddString(L"忧郁蘑菇");
	g_dialog->plants2.AddString(L"香蒲");
	g_dialog->plants2.AddString(L"冰瓜");
	g_dialog->plants2.AddString(L"吸金磁");
	g_dialog->plants2.AddString(L"地刺王");
	g_dialog->plants2.AddString(L"玉米加农炮");

	g_dialog->plants2.AddString(L"模仿者");
	g_dialog->plants2.AddString(L"爆炸坚果");
	g_dialog->plants2.AddString(L"大坚果保龄球");
	g_dialog->plants2.AddString(L"种子芽");
	g_dialog->plants2.AddString(L"反向双发射手");

	g_dialog->plants2.SetCurSel(0);
}

VOID initZombiesRow()
{
	g_dialog->zombies_row.AddString(L"第0行");
	g_dialog->zombies_row.AddString(L"第1行");
	g_dialog->zombies_row.AddString(L"第2行");
	g_dialog->zombies_row.AddString(L"第3行");
	g_dialog->zombies_row.AddString(L"第4行");
	g_dialog->zombies_row.AddString(L"第5行");

	g_dialog->zombies_row.SetCurSel(0);
}

VOID initZombiesList()
{
	g_dialog->zombies_list.AddString(L"第0列");
	g_dialog->zombies_list.AddString(L"第1列");
	g_dialog->zombies_list.AddString(L"第2列");
	g_dialog->zombies_list.AddString(L"第3列");
	g_dialog->zombies_list.AddString(L"第4列");

	g_dialog->zombies_list.AddString(L"第5列");
	g_dialog->zombies_list.AddString(L"第6列");
	g_dialog->zombies_list.AddString(L"第7列");
	g_dialog->zombies_list.AddString(L"第8列");

	g_dialog->zombies_list.SetCurSel(8);
}

VOID initZembies()
{
	// 0
	g_dialog->zombies_class.AddString(L"普通僵尸");
	g_dialog->zombies_class.AddString(L"摇旗僵尸");
	g_dialog->zombies_class.AddString(L"路障僵尸");
	g_dialog->zombies_class.AddString(L"撑杆僵尸");
	g_dialog->zombies_class.AddString(L"铁桶僵尸");

	// 5
	g_dialog->zombies_class.AddString(L"读报僵尸");
	g_dialog->zombies_class.AddString(L"铁栅门僵尸");
	g_dialog->zombies_class.AddString(L"橄榄球僵尸");
	g_dialog->zombies_class.AddString(L"舞王僵尸");
	g_dialog->zombies_class.AddString(L"伴舞僵尸");

	// 10
	g_dialog->zombies_class.AddString(L"鸭子救生圈僵尸");
	g_dialog->zombies_class.AddString(L"潜水僵尸");
	g_dialog->zombies_class.AddString(L"雪撬车僵尸");
	g_dialog->zombies_class.AddString(L"雪橇僵尸小队");
	g_dialog->zombies_class.AddString(L"海豚骑士僵尸");

	// 15
	g_dialog->zombies_class.AddString(L"玩偶匣僵尸");
	g_dialog->zombies_class.AddString(L"气球僵尸");
	g_dialog->zombies_class.AddString(L"矿工僵尸");
	g_dialog->zombies_class.AddString(L"跳跳僵尸");
	g_dialog->zombies_class.AddString(L"僵尸雪人");

	// 20
	g_dialog->zombies_class.AddString(L"蹦极僵尸");
	g_dialog->zombies_class.AddString(L"扶梯僵尸");
	g_dialog->zombies_class.AddString(L"投石车僵尸");
	g_dialog->zombies_class.AddString(L"巨人僵尸");
	g_dialog->zombies_class.AddString(L"小鬼僵尸");

	// 25
	g_dialog->zombies_class.AddString(L"僵王博士");
	g_dialog->zombies_class.AddString(L"豌豆射手僵尸");
	g_dialog->zombies_class.AddString(L"坚果墙僵尸");
	g_dialog->zombies_class.AddString(L"火爆辣椒僵尸");
	g_dialog->zombies_class.AddString(L"机枪射手僵尸");

	// 30
	g_dialog->zombies_class.AddString(L"窝瓜僵尸");
	g_dialog->zombies_class.AddString(L"高坚果僵尸");
	g_dialog->zombies_class.AddString(L"无");
	g_dialog->zombies_class.AddString(L"隐身僵尸");
	g_dialog->zombies_class.AddString(L"无");

	// 30
	g_dialog->zombies_class.AddString(L"无");
	g_dialog->zombies_class.AddString(L"无扶梯的扶梯僵尸");

	g_dialog->zombies_class.SetCurSel(0);
}


VOID initCornBullet()
{
	g_dialog->corn_bullet.AddString(L"黄油");

	g_dialog->corn_bullet.AddString(L"加农炮");

	g_dialog->corn_bullet.SetCurSel(1);
}


VOID initBulletClass()
{
	g_dialog->BulletClass.AddString(L"子弹穿透");

	g_dialog->BulletClass.AddString(L"自动追踪");

	g_dialog->BulletClass.AddString(L"全屏子弹");

	g_dialog->BulletClass.SetCurSel(0);
}



void CpvzDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}


void CpvzDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here3
	DWORD dwcard = g_dialog->cardslot_combobox.GetCurSel();

	DWORD dwPlant = g_dialog->plants_comboBox.GetCurSel();

	ModifyCardSlotPlant(dwcard, dwPlant);

}


void CpvzDlg::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
}


void CpvzDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	DWORD dwrow = g_dialog->plant_row.GetCurSel();

	DWORD dwlist = g_dialog->plant_list.GetCurSel();

	DWORD dwplant = g_dialog->plants2.GetCurSel();

	Plant(dwrow, dwlist, dwplant);

}


void CpvzDlg::OnBnClickedCheck7()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();

	if (status)
		PlantOverlapping();
	else
		RecoverPlantOverlappint();
}


void CpvzDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	DWORD dwrow = g_dialog->zombies_row.GetCurSel();

	DWORD dwlist = g_dialog->zombies_list.GetCurSel();

	DWORD dwZombies = g_dialog->zombies_class.GetCurSel();

	ZombiesPlant(dwrow, dwlist, dwZombies);
}


void CpvzDlg::OnBnClickedCheck8()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();

	if (status)
		PurpleDirectPlant();
	else
		RecoverPurpleDirectPlant();
}


void CpvzDlg::OnBnClickedCheck9()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();

	if (status)
		BackgroundRunGame();
	else
		RecoverBackgroundRunGame();
}


void CpvzDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here


	KillAllZombies();

	Sleep(5000);

	RecoverKillAllZombies();

}

// 全局快捷键
LRESULT CpvzDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	UINT Mod = (UINT)LOWORD(lParam);		// key-modifier flags 
	UINT uVirtKey = (UINT)HIWORD(lParam);   // virtual-key code 
	DWORD32 flag = 1;
	
	if (uVirtKey == 0x51)
	{
		//AfxMessageBox(L"q accelerator!\n");

		WriteProcessMemory(g_crack->stGameInfo->hGame, (PVOID)lpkillZombiesFlagAddress, &flag, sizeof(DWORD32), NULL);
	}

	return 0;
}

void CpvzDlg::OnBnClickedCheck10()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();

	if (status)
	{
		RegisterHotKey(AfxGetMainWnd()->GetSafeHwnd(), Q_HOT_KEY_ID, NULL, 0x51);

		KillOneZombies();
	}
	else
	{
		UnregisterHotKey(AfxGetMainWnd()->GetSafeHwnd(), Q_HOT_KEY_ID);

		RecoverKillOneZombies();
	}

}


void CpvzDlg::OnBnClickedCheck11()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck();

	if (status)
	{
		this->GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	
		RomdomizedBullet();
	}
	else
	{
		this->GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);

		RecoverRomdomizedBullet();
	}
}


void CpvzDlg::OnBnClickedCheck12()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck();

	if (status)
		ClayPotPerspective();
	else
		RecoverClayPotPerspective();
}


void CpvzDlg::OnBnClickedCheck13()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK13))->GetCheck();

	if (status)
		QuickConveyorBelt();
	else
		RecoverQucikConveyorBelt();
}


void CpvzDlg::OnBnClickedCheck14()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck();

	if (status)
		ListPlant();
	else
		RecoverListPlant();
}


void CpvzDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	DWORD serial = g_dialog->corn_bullet.GetCurSel();

	if (!bBulletFlag)
	{
		bBulletFlag = TRUE;

		this->GetDlgItem(IDC_BUTTON6)->SetWindowTextW(L"取消锁定");

		this->GetDlgItem(IDC_COMBO9)->EnableWindow(FALSE);

		this->GetDlgItem(IDC_CHECK11)->EnableWindow(FALSE);

		if (serial == 0)
		{
			ChangeCornBulletToButter();

		}
		else if (serial == 1)
		{
			ChangeCornBulletToBomb();
		}
	}
	else
	{
		this->GetDlgItem(IDC_BUTTON6)->SetWindowTextW(L"锁定");

		this->GetDlgItem(IDC_COMBO9)->EnableWindow(TRUE);

		this->GetDlgItem(IDC_CHECK11)->EnableWindow(TRUE);

		RecoverChangeCornBullet();

		bBulletFlag = FALSE;
	}
}


void CpvzDlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	this->GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);

	CleanTombstone();

	this->GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
}


void CpvzDlg::OnBnClickedCheck15()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK15))->GetCheck();

	if (status)
		RomdomPlant();
	else
		CancelRomdomPlant();
}


void CpvzDlg::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	this->GetDlgItem(IDC_BUTTON10)->EnableWindow(FALSE);

	CleanPlants();

	this->GetDlgItem(IDC_BUTTON10)->EnableWindow(TRUE);
}


void CpvzDlg::OnBnClickedCheck16()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK16))->GetCheck();

	if (status)
		BigMouthFlowerColdDown();
	else
		RecoverBigMouthFlowerColdDown();
}


void CpvzDlg::OnBnClickedCheck17()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK17))->GetCheck();

	if (status)
		PotatoMineColdDown();
	else
		RecoverPotatoMineColdDown();
}


void CpvzDlg::OnBnClickedCheck18()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK18))->GetCheck();

	if (status)
		CornCannonColdDown();
	else
		RecoverCornCannonColdDown();
}


void CpvzDlg::OnBnClickedCheck19()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK19))->GetCheck();

	if (status)
		MagneticMushRoomColdDown();
	else
		RecoverMagneticMushRoomColdDown();
}


void CpvzDlg::OnBnClickedCheck20()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK20))->GetCheck();

	if (status)
		ClearFog();
	else
		RecoverClearFog();
}


void CpvzDlg::OnBnClickedCheck21()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck();

	if (status)
		MushRoomDonotSleep();
	else
		RecoverMushRoomDonotSleep();
}


void CpvzDlg::OnBnClickedCheck22()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK22))->GetCheck();

	if (status)
		SpikeZombies();
	else
		RecoverSpikeZombies();
}


void CpvzDlg::OnBnClickedCheck23()
{
	// TODO: Add your control notification handler code here
	int status = ((CButton*)GetDlgItem(IDC_CHECK23))->GetCheck();

	if (status)
		UndefeatedForever();
	else
		RecoverUndefeatedForever();
}


void CpvzDlg::OnCbnSelchangeCombo8()
{
	// TODO: Add your control notification handler code here
}


void CpvzDlg::OnCbnSelchangeCombo10()
{
	// TODO: Add your control notification handler code here

}


void CpvzDlg::OnBnClickedButton12()
{
	// TODO: Add your control notification handler code here
	DWORD serial = g_dialog->BulletClass.GetCurSel();

	switch (AllBulletFlag)
	{
	case 0:
		RecoverPenetrateBullet();

		this->GetDlgItem(IDC_BUTTON12)->SetWindowTextW(L"锁定");

		this->GetDlgItem(IDC_COMBO10)->EnableWindow(TRUE);

		AllBulletFlag = 3;
		break;
	case 1:
		RecoverBulletTracking();

		this->GetDlgItem(IDC_BUTTON12)->SetWindowTextW(L"锁定");

		this->GetDlgItem(IDC_COMBO10)->EnableWindow(TRUE);

		AllBulletFlag = 3;
		break;
	case 2:
		this->GetDlgItem(IDC_BUTTON12)->SetWindowTextW(L"锁定");

		this->GetDlgItem(IDC_COMBO10)->EnableWindow(TRUE);

		AllBulletFlag = 3;
		break;
	case 3:
		this->GetDlgItem(IDC_BUTTON12)->SetWindowTextW(L"取消锁定");

		this->GetDlgItem(IDC_COMBO10)->EnableWindow(FALSE);

		switch (serial)
		{
		case 0:
			PenetrateBullet();

			AllBulletFlag = 0;
			break;

		case 1:
			BulletTracking();

			AllBulletFlag = 1;
			break;

		case 2:
			AllBulletFlag = 2;
			break;
		}
		break;
	}
}


void CpvzDlg::OnBnClickedButton11()
{
	// TODO: Add your control notification handler code here
	ModifyMoney();
}


void CpvzDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	this->GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);

	StartupFiveCar();

	Sleep(5000);

	RecoverFiveCar();

	this->GetDlgItem(IDC_BUTTON8)->EnableWindow(TRUE);
}


void CpvzDlg::OnBnClickedButton13()
{
	// TODO: Add your control notification handler code here
	this->GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);

	StartupSixCar();

	Sleep(5000);

	RecoverSixCar();

	this->GetDlgItem(IDC_BUTTON13)->EnableWindow(TRUE);
}


void CpvzDlg::OnCbnSelchangeCombo6()
{
	// TODO: Add your control notification handler code here
}
