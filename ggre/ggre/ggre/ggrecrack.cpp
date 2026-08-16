#include "pch.h"
#include "common.hpp"

#define ARMOR_LEVEL_TIMER 100
#define LOCK_ARM_TYPE_TIMER 101

VOID lock_armor_level_timer_routine(HWND, UINT, UINT_PTR, DWORD)
{
    STATUS status = STATUS::NoneError;

    ULONG offset[] = { 0x05e1b728, 0xc8, 0x20,0x80,0x98,0x360 };

    ULONG armor_level = 0;

    int sel = g_dialog->armor_level.GetCurSel();
    if (sel != CB_ERR)
        armor_level = sel + 1;

    status = g_crack->ModifyLayerMemory32((PVOID)((uint64_t)g_crack->stGameInfo->lpBase),
        armor_level,
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        CString szEditStr;

        szEditStr.Format(L"lock armor level write layer memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT1)->SetWindowTextW(szEditStr);
    }
}

void lock_armor_level()
{
    SetTimer(AfxGetMainWnd()->GetSafeHwnd(), ARMOR_LEVEL_TIMER, 500u, (TIMERPROC)lock_armor_level_timer_routine);
    return;
}

void cancel_lock_armor_level()
{
    KillTimer(AfxGetMainWnd()->GetSafeHwnd(), ARMOR_LEVEL_TIMER);
    return;
}

void lock_invincible()
{
    /*
    * src
    *  F3:0F1043 78             | movss xmm0,dword ptr ds:[rbx+78]        |
    *  0F 29 74 24 30           | movaps [rsp+30],xmm6
    *  0F 57 F6                 | xorps xmm6,xmm6
    *  0F 2F F0                 | comiss xmm6,xmm0
    *
    *  injuct code
    *  80B9 10010000 01         | cmp byte ptr ds:[rcx+110],1             |
    *  0F84 0D000000            | je  (C743 78 0000803F)                  |
    *  80B9 10010000 01         | cmp byte ptr ds:[rcx+110],1             |
    *  0F85 0E000000            | jne ()                       |
    *  C743 78 0000803F         | mov dword ptr ds:[rbx+78],3F800000      |
    *  F3:0F1043 78             | movss xmm0,dword ptr ds:[rbx+78]        |
    *  EB 05                    | jmp (return code)                       |
    *  F3:0F1043 78             | movss xmm0,dword ptr ds:[rbx+78]        |
    *
    */
    std::vector<uint8_t> injuctcode_vec = {
        0x80, 0xB9, 0x10, 0x01, 0x00, 0x00, 0x01,
        0x0F, 0x84, 0x0D, 0x00, 0x00, 0x00,
        0x80, 0xB9, 0x10, 0x01, 0x00, 0x00, 0x01,
        0x0F, 0x85, 0x0E, 0x00, 0x00, 0x00,
        0xC7, 0x43, 0x78, 0x00, 0x00, 0x80, 0x3F,
        0xF3, 0x0F, 0x10, 0x43, 0x78,
        0xEB, 0x05,
        0xF3, 0x0F, 0x10, 0x43, 0x78
    };

    std::vector<uint8_t> sourcecode_vec = {
        0xF3, 0x0F, 0x10, 0x43, 0x78,
        0x0F, 0x29, 0x74, 0x24, 0x30,
        0x0F, 0x57, 0xF6,
        0x0F, 0x2F, 0xF0
    };

    g_crack->AssemblyInject64((PVOID)((uint64_t)g_crack->stGameInfo->lpBase + 0xef86e8),
        injuctcode_vec,
        sourcecode_vec
    );


}

void cancel_lock_invincible()
{
    std::vector<uint8_t> sourcecode_vec = {
    0xF3, 0x0F, 0x10, 0x43, 0x78,
    0x0F, 0x29, 0x74, 0x24, 0x30,
    0x0F, 0x57, 0xF6,
    0x0F, 0x2F, 0xF0
    };

    if (!WriteProcessMemory((HANDLE)g_crack->stGameInfo->hGame, (LPVOID)((uint64_t)g_crack->stGameInfo->lpBase + 0xef86e8), sourcecode_vec.data(), sourcecode_vec.size(), NULL))
    {
        CString szEditStr;
        szEditStr.Format(L"cancel lock invincible write layer memory failed!\n");
        g_dialog->GetDlgItem(IDC_EDIT1)->SetWindowTextW(szEditStr);
    }
}

VOID lock_arm_type_timer_routine(HWND, UINT, UINT_PTR, DWORD)
{
    modify_arm_type();
}

void lock_arm_type()
{
    SetTimer(AfxGetMainWnd()->GetSafeHwnd(), LOCK_ARM_TYPE_TIMER, 500u, (TIMERPROC)lock_arm_type_timer_routine);
    return;
}

void cancel_lock_arm_type()
{
    KillTimer(AfxGetMainWnd()->GetSafeHwnd(), LOCK_ARM_TYPE_TIMER);
    return;
}

void modify_arm_type()
{
    STATUS status = STATUS::NoneError;

    ULONG offset[] = { 0x05e19390, 0x80, 0x40 };

    ULONG arm_type = 0;

    int sel = g_dialog->arm_type.GetCurSel();
    if (sel != CB_ERR)
        arm_type = sel;

    status = g_crack->ModifyLayerMemory8(g_crack->stGameInfo->lpBase,
        (BYTE)arm_type,
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        CString szEditStr;

        szEditStr.Format(L"lock arm type write layer memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT1)->SetWindowTextW(szEditStr);
    }
}

void modify_score()
{
    STATUS status = STATUS::NoneError;

    ULONG offset[] = { 0x05e1bd48, 0xe8 };

    ULONG score = g_dialog->GetDlgItemInt(IDC_EDIT2, NULL, FALSE);

    status = g_crack->ModifyLayerMemory32(g_crack->stGameInfo->lpBase,
        score,
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        CString szEditStr;

        szEditStr.Format(L"modify score write layer memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT1)->SetWindowTextW(szEditStr);
    }
}

void modify_time()
{
    STATUS status = STATUS::NoneError;

    ULONG offset[] = { 0x05e1bd48, 0x150 };

    ULONG time_num = g_dialog->GetDlgItemInt(IDC_EDIT3, NULL, FALSE);

    float time_float = (float)time_num;

    status = g_crack->ModifyLayerMemory32(g_crack->stGameInfo->lpBase,
        *((ULONG*)&time_float),
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        CString szEditStr;

        szEditStr.Format(L"modify time write layer memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT1)->SetWindowTextW(szEditStr);
    }
}


