
// cs1.6Dlg.h : header file
//

#pragma once

#include <thread>

#include "context.hpp"
#include "extension.hpp"
#include "boxperspective.hpp"


// Ccs16Dlg dialog
class Ccs16Dlg : public CDialogEx
{
// Construction
public:
    Ccs16Dlg(CWnd* pParent = nullptr);    // standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CS16_DIALOG };
#endif

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedRadio1();
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedCheck2();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedRadio2();
    afx_msg void OnBnClickedCheck3();
    afx_msg void OnBnClickedCheck5();
    afx_msg void OnBnClickedCheck7();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedCheck4();
    afx_msg void OnBnClickedCheck6();
    afx_msg void OnBnClickedCheck8();
    afx_msg void OnBnClickedCheck9();
    afx_msg void OnBnClickedButton5();
    afx_msg void OnBnClickedButton6();
    afx_msg void OnBnClickedButton7();
    afx_msg void OnBnClickedButton8();
    afx_msg void OnBnClickedButton9();
    afx_msg void OnBnClickedButton10();
    afx_msg void OnBnClickedCheck10();
    afx_msg void OnBnClickedCheck11();
    afx_msg void OnBnClickedCheck12();
};

extern Ccs16Dlg* g_dialog;
extern std::unique_ptr<game::GameContext> g_pGameContext;
extern boolean boxperspective_thread_quit_flag;
extern boolean bone_flag;
