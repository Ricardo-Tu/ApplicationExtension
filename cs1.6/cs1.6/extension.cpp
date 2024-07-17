#include "pch.h"
#include "extension.hpp"

#define MONEYTIMER  0x100
#define BLOODTIMER  0x110
#define ARMORTIMER  0X120
#define BULLETTIMER 0x130
#define BuyEveryWhereTimer 0x140
#define MOUSELEFTBUTTONCOLDDOWNTIMER 0x150
#define NOBACKSEATTIMER 0x160
#define SUPERMANTIMER 0x170

std::vector<Postion> myPostion(3);

namespace game {

    void ModifyMoneyTimer(HWND, UINT, UINT_PTR, DWORD)
    {
        SetMoney();
    }

    boolean SetMoneyTimer()
    {
        return SetTimer(AfxGetMainWnd()->GetSafeHwnd(), MONEYTIMER, 1000, (TIMERPROC)ModifyMoneyTimer);
    }

    void SetMoney()
    {
        CString MoneyStr;
        g_dialog->GetDlgItem(IDC_EDIT1)->GetWindowTextW(MoneyStr);
        uint32_t Money = _wtol(MoneyStr);
        std::vector<uint32_t> MoneyOffsetShow = { 0x61B9FC };
        std::vector<uint32_t> MoneyOffsetSource = { 0x0182C5C4 , 0x490 , 0xB0, 0x0, 0x0, 0x380, 0x3a0, 0x1cc };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32(Money, MoneyOffsetShow);
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32(Money, MoneyOffsetSource);
    }

    boolean CancelSetMoneyTimer()
    {
        return KillTimer(AfxGetMainWnd()->GetSafeHwnd(), MONEYTIMER);
    }

    void ModifyBloodTimer(HWND, UINT, UINT_PTR, DWORD)
    {
        SetBlood();
    }

    void SetBlood()
    {
        CString BloodStr;
        g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(BloodStr);
        uint32_t blood = _wtol(BloodStr);
        std::vector<uint32_t> BloodOffset = { 0x11069BC , 0x7c , 0x4, 0x160 };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float(blood, BloodOffset);
    }

    boolean SetBloodTimer()
    {
        return SetTimer(AfxGetMainWnd()->GetSafeHwnd(), BLOODTIMER, 500, (TIMERPROC)ModifyBloodTimer);
    }

    boolean CancelSetBloodTimer()
    {
        return KillTimer(AfxGetMainWnd()->GetSafeHwnd(), BLOODTIMER);
    }

    void ModifyArmorTimer(HWND, UINT, UINT_PTR, DWORD)
    {
        SetArmor();
    }

    void SetArmor()
    {
        CString ArmorStr;
        g_dialog->GetDlgItem(IDC_EDIT3)->GetWindowTextW(ArmorStr);
        uint32_t armor = _wtol(ArmorStr);
        std::vector<uint32_t> ArmorOffset = { 0x11069BC , 0x7c , 0x4, 0x1bc };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float((float)armor, ArmorOffset);
    }

    boolean SetArmorTimer()
    {
        return SetTimer(AfxGetMainWnd()->GetSafeHwnd(), ARMORTIMER, 500, (TIMERPROC)ModifyArmorTimer);
    }

    boolean CancelSetArmorTimer()
    {
        return KillTimer(AfxGetMainWnd()->GetSafeHwnd(), ARMORTIMER);
    }

    void ModifyBulletTimer(HWND, UINT, UINT_PTR, DWORD)
    {
        SetBullet();
    }
    void SetBullet()
    {
        CString BulletStr;
        g_dialog->GetDlgItem(IDC_EDIT3)->GetWindowTextW(BulletStr);
        uint32_t bullet = _wtol(BulletStr);
        std::vector<uint32_t> BulletOffset = { 0x11069BC , 0x7c , 0x5ec, 0xcc };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32(bullet, BulletOffset);
    }

    boolean SetBulletTimer()
    {
        return SetTimer(AfxGetMainWnd()->GetSafeHwnd(), BULLETTIMER, 500, (TIMERPROC)ModifyBulletTimer);
    }

    boolean CancelSetBulletTimer()
    {
        return KillTimer(AfxGetMainWnd()->GetSafeHwnd(), BULLETTIMER);
    }


    boolean InitAllControlsContext()
    {
        InitEditControlContext();
        InitRadioControlContext();
        return true;
    }

    boolean InitEditControlContext()
    {
        g_dialog->GetDlgItem(IDC_EDIT1)->SetWindowTextW(L"16000");
        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(L"255");
        g_dialog->GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"999");
        g_dialog->GetDlgItem(IDC_EDIT4)->SetWindowTextW(L"128");
        return true;
    }

    boolean InitRadioControlContext()
    {
        ((CButton*)g_dialog->GetDlgItem(IDC_RADIO1))->EnableWindow(false);
        ((CButton*)g_dialog->GetDlgItem(IDC_RADIO2))->EnableWindow(false);
        return true;
    }

    void SetInvincibility()
    {
        uint32_t flag = 0;
        std::vector<uint32_t> InvincibilityOffset = { 0x11069BC , 0x7c , 0x4, 0x16c };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float((float)flag, InvincibilityOffset);
    }

    void CancelInvincibility()
    {
        uint32_t flag = 2;
        std::vector<uint32_t> InvincibilityOffset = { 0x11069BC , 0x7c , 0x4, 0x16c };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float((float)flag, InvincibilityOffset);
    }

    void ModifyBuyEveryWhereTimer(HWND, UINT, UINT_PTR, DWORD)
    {
        uint32_t flag = 1;
        std::vector<uint32_t> BuyEveryWhereOffset = { 0x11069BC , 0x7c , 0x3c0 };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32(flag, BuyEveryWhereOffset);
    }

    void SetBuyEveryWhere()
    {
        SetTimer(AfxGetMainWnd()->GetSafeHwnd(), BuyEveryWhereTimer, 500, (TIMERPROC)ModifyBuyEveryWhereTimer);
    }

    void CancelBuyEveryWhere()
    {
        KillTimer(AfxGetMainWnd()->GetSafeHwnd(), BuyEveryWhereTimer);
    }

    void ModifyMouseLeftButtonColdDownTimer(HWND, UINT, UINT_PTR, DWORD)
    {
        float num = 0.01;
        std::vector<uint32_t> ColdDownOffset = { 0x11069BC , 0x7c , 0x5ec, 0xb8 };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float(num, ColdDownOffset);
    }

    void SetMouseLeftButtonColdDown()
    {
        SetTimer(AfxGetMainWnd()->GetSafeHwnd(), MOUSELEFTBUTTONCOLDDOWNTIMER, 100, (TIMERPROC)ModifyMouseLeftButtonColdDownTimer);
    }

    void CancelMouseLeftButtonColdDown()
    {
        KillTimer(AfxGetMainWnd()->GetSafeHwnd(), MOUSELEFTBUTTONCOLDDOWNTIMER);
    }

    void ModifyNoBackSeatTimer(HWND, UINT, UINT_PTR, DWORD)
    {
        uint32_t flag = 0;
        std::vector<uint32_t> NoBackSeatOffset = { 0x11069BC , 0x7c , 0x5ec, 0x100 };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32(flag, NoBackSeatOffset);
    }

    void SetNoBackSeat()
    {
        SetTimer(AfxGetMainWnd()->GetSafeHwnd(), NOBACKSEATTIMER, 50, (TIMERPROC)ModifyNoBackSeatTimer);
    }

    void CancelNoBackSeat()
    {
        KillTimer(AfxGetMainWnd()->GetSafeHwnd(), NOBACKSEATTIMER);
    }

    void SavePostion(Postion& position)
    {
        std::vector<uint32_t> coordinationOffset = { 0x11069BC , 0x7c, 0x4, 0x8 };
        g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory32float(&position.x, coordinationOffset);
        coordinationOffset[3] += 0x4;
        g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory32float(&position.y, coordinationOffset);
        coordinationOffset[3] += 0x4;
        g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory32float(&position.z, coordinationOffset);
    }

    void LoadPostion(Postion position)
    {
        std::vector<uint32_t> coordinationOffset = { 0x11069BC , 0x7c, 0x4, 0x8 };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float(position.x, coordinationOffset);
        coordinationOffset[3] += 0x4;
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float(position.y, coordinationOffset);
        coordinationOffset[3] += 0x4;
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float(position.z, coordinationOffset);
    }

    void SuperManTimer(HWND, UINT, UINT_PTR, DWORD)
    {
        float height = 0.0f;
        uint32_t flag = 0;
        std::vector<uint32_t> StatusOffset = { 0x11069BC , 0x7c , 0x4, 0x12c };
        std::vector<uint32_t> SuperManOffset = { 0x11069BC , 0x7c , 0x4, 0x28 };
        g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory32(&flag, StatusOffset);
        if (flag == 6)
        {
            g_pGameContext->modules[L"cstrike.exe"]->ReadMultiLevelMemory32float(&height, SuperManOffset);
            if (height > 0)
            {
                height += height * 1.3;
            }
            else
            {
                height -= height * 0.4;
            }
            g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32float(height, SuperManOffset);
        }
    }

    void SetSuperMan()
    {
        SetTimer(AfxGetMainWnd()->GetSafeHwnd(), SUPERMANTIMER, 100, (TIMERPROC)SuperManTimer);
    }

    void CancelSuperMan()
    {
        KillTimer(AfxGetMainWnd()->GetSafeHwnd(), SUPERMANTIMER);
    }

    void SetNightVision()
    {
        uint32_t flag = 1;
        std::vector<uint32_t> NightVisionOffset = { 0x61baa4 };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32(flag, NightVisionOffset);
    }

    void CancelNightVision()
    {
        uint32_t flag = 0;
        std::vector<uint32_t> NightVisionOffset = { 0x61baa4 };
        g_pGameContext->modules[L"cstrike.exe"]->ModifyMultiLevelLayerMemory32(flag, NightVisionOffset);
    }

}