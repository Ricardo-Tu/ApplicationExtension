
// pvzDlg.h : header file
//

#pragma once
#include "crack.h"
class CpvzDlgAutoProxy;


// CpvzDlg dialog
class CpvzDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CpvzDlg);
    friend class CpvzDlgAutoProxy;

    // Construction
public:
    CpvzDlg(CWnd* pParent = nullptr);    // standard constructor
    virtual ~CpvzDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_PVZ_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


    // Implementation
protected:
    CpvzDlgAutoProxy* m_pAutoProxy;
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
public:
    afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedButton1();
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedCheck2();
    afx_msg void OnBnClickedCheck3();
    afx_msg void OnBnClickedCheck4();
    afx_msg void OnBnClickedCheck5();
    afx_msg void OnBnClickedCheck6();
    afx_msg void OnCbnSelchangeCombo1();
    // This is card slot combobox.
    CComboBox cardslot_combobox;
    afx_msg void OnBnClickedButton3();
    afx_msg void OnCbnSelchangeCombo2();
    CComboBox plants_comboBox;
    CComboBox plant_row;
    CComboBox plant_list;
    CComboBox plants2;
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedCheck7();
    afx_msg void OnBnClickedButton5();
    CComboBox zombies_row;
    CComboBox zombies_list;
    CComboBox zombies_class;
    afx_msg void OnBnClickedCheck8();
    afx_msg void OnBnClickedCheck9();
    afx_msg void OnBnClickedButton7();
    afx_msg void OnBnClickedCheck10();
    afx_msg void OnBnClickedCheck11();
    afx_msg void OnBnClickedCheck12();
    afx_msg void OnBnClickedCheck13();
    afx_msg void OnBnClickedCheck14();
    afx_msg void OnBnClickedButton6();
    CComboBox corn_bullet;
    afx_msg void OnBnClickedButton9();
    afx_msg void OnBnClickedCheck15();
    afx_msg void OnBnClickedButton10();
    afx_msg void OnBnClickedCheck16();
    afx_msg void OnBnClickedCheck17();
    afx_msg void OnBnClickedCheck18();
    afx_msg void OnBnClickedCheck19();
    afx_msg void OnBnClickedCheck20();
    afx_msg void OnBnClickedCheck22();
    afx_msg void OnBnClickedCheck21();
    afx_msg void OnBnClickedCheck23();
    afx_msg void OnCbnSelchangeCombo8();
    afx_msg void OnCbnSelchangeCombo10();
    CComboBox BulletClass;
    afx_msg void OnBnClickedButton12();
    afx_msg void OnBnClickedButton11();
    afx_msg void OnBnClickedButton8();
    afx_msg void OnBnClickedButton13();
    afx_msg void OnCbnSelchangeCombo6();
    afx_msg void OnBnClickedCheck24();
    afx_msg void OnBnClickedCheck25();
};

extern CpvzDlg* g_dialog;

extern CrackRE32* g_crack;

extern bool fiveCarThreadFlag, sixCarThreadFlag;
