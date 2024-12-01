#include "pch.h"
#include "common.h"

#define CARDCOOLDOWNTIMER       111
#define RANDOMIZEDBULLETTIMER   112
#define RANDOMIZEDPLANTTIMER    113
#define FIVECARTIMER            114
#define SIXCARTIMER             115


BOOLEAN cardSlotCoolDown[10] = { 0 };

PVOID lpkillZombiesFlagAddress = NULL;

PVOID lpRandomBulletNumber = 0;

/*
    Routine Description:
        This routine lock sunshine value by modify assembly source code that decrease sunshine.

*/
VOID WINAPI LockSunShine()
{
    BOOLEAN status = FALSE;

    BYTE nop0[] = { "\x90\x90\x90\x90\x90\x90" };
    BYTE nop1[] = { "\x90\x90\x90\x90\x90\x90" };

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PVOID)((PUCHAR)g_crack->stGameInfo->lpBase + 0x30a11),
        nop0, sizeof(nop0) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"Modify lock sunshine code0 procedure failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);

    }

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PVOID)((PUCHAR)g_crack->stGameInfo->lpBase + 0x1ba76),
        nop1, sizeof(nop1) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"Modify lock sunshine code1 procedure failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);

    }

    return;
}


// This routine unlock sun shine value.
VOID WINAPI RecoverLockSunShineShineOriginalCode()
{
    BOOLEAN status = FALSE;

    BYTE szOrginalCode1[] = { "\x01\x88\x60\x55\x00\x00" };
    BYTE szOrginalCode2[] = { "\x89\xb7\x60\x55\x00\x00" };

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PVOID)((PUCHAR)g_crack->stGameInfo->lpBase + 0x30a11),
        szOrginalCode1,
        sizeof(szOrginalCode1) - 1,
        NULL);

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"Recover no crease sunshin origial code failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PVOID)((PUCHAR)g_crack->stGameInfo->lpBase + 0x1ba76),
        szOrginalCode2,
        sizeof(szOrginalCode2) - 1,
        NULL);

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"Recover no crease sunshin origial code failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}


// This routine modify sun shine value that num in edit.
VOID WINAPI ModifySunShine()
{
    BOOLEAN status = FALSE;

    CString editstr;

    g_dialog->GetDlgItem(IDC_EDIT1)->GetWindowTextW(editstr);

    DWORD num = _ttol(editstr);

    ULONG offset[] = { 0x002A9EC0 , 0x768, 0x5560 };

    g_crack->ModifyLayerMemory32(g_crack->stGameInfo->lpBase, num, offset, arraysize(offset) - 1);

    return;
}


// This routine decrease card cool down by modify assembly source code.
VOID WINAPI DecreaseCardCooldown()
{
    BOOLEAN status = FALSE;

    BYTE asmcode[] = { "\x83\x47\x24\x7f" };

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x8728c,
        asmcode,
        sizeof(asmcode) - 1,
        NULL);

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"Increase card cool down procedure implement write memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(szEditString);
    }

    return;
}


// This routine recover card cool down source code.
VOID WINAPI RecoverCardCooldownSourceCode()
{
    BOOLEAN status = FALSE;

    BYTE asmcode[] = { "\x83\x47\x24\x01" };

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x8728c,
        asmcode,
        sizeof(asmcode) - 1,
        NULL);

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"Recover card cool down source code procedure implement failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(szEditString);
    }

    return;
}


/*

    Routine Description:
        This timer routine is no card slot cool down.

*/

VOID CardCoolDownTimer(HWND, UINT, UINT_PTR, DWORD)
{
    STATUS status = STATUS::NoneError;

    LONG    dwCoolDown = 50000;

    ULONG    offset[] = { 0x2a6328,0xc8,0x4,0xe0,0x14,0x144,0x4c };

    for (ULONG i = 0; i < 10; i++)
    {
        if (cardSlotCoolDown[i])
        {
            offset[6] += 0x50;

            status = g_crack->ModifyLayerMemory32(g_crack->stGameInfo->lpBase,
                dwCoolDown,
                offset,
                arraysize(offset) - 1
            );

            if (status != STATUS::NoneError)
            {
                CString szEditStr;

                szEditStr.Format(L"Card cool down write layer momory failed!\n");

                g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(szEditStr);
            }
        }
    }
}


// This routine  set a timer that no card slot cool down.
VOID WINAPI NoCardCooldown()
{
    for (ULONG i = 0; i < 10; i++)
        cardSlotCoolDown[i] = TRUE;

    SetTimer(AfxGetMainWnd()->GetSafeHwnd(), CARDCOOLDOWNTIMER, 500u, (TIMERPROC)CardCoolDownTimer);

    return;
}


VOID WINAPI CancelNoCardCooldown()
{
    KillTimer(AfxGetMainWnd()->GetSafeHwnd(), CARDCOOLDOWNTIMER);

    return;
}


VOID WINAPI AutomaticallyCollectionSunShine()
{
    BOOLEAN status = FALSE;

    BYTE code1[] = { "\x90\x90" },
        code2[] = { "\x90\x90\x90\x90\x90\x90" },
        code3[] = { "\x90\x90\x90\x90\x90" };

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x30ad0,
        code1,
        sizeof(code1) - 1,
        NULL);

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x30afe,
        code2,
        sizeof(code2) - 1,
        NULL);

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x30b0b,
        code3,
        sizeof(code3) - 1,
        NULL);

    if (!status)
    {
        CString    szEditStr;

        szEditStr.Format(L"Automatically collection sunshine routine write memory failed!\n");
    }

    return;
}


VOID WINAPI RecoverAutomaticallyCollectionSunShine()
{
    BOOLEAN status = FALSE;

    BYTE code1[] = { "\x75\x3e" }, code2[] = { "\x0F\x8C\xE7\x02\x00\x00" }, code3[] = { "\xE9\xDB\x02\x00\x00" };

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x30ad0,
        code1,
        sizeof(code1) - 1,
        NULL);

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x30afe,
        code2,
        sizeof(code2) - 1,
        NULL);

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x30b0b,
        code3,
        sizeof(code3) - 1,
        NULL);

    if (!status)
    {
        CString    szEditStr;

        szEditStr.Format(L"Automatically collection sunshine routine write memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(szEditStr);
    }

    return;
}

VOID WINAPI Stregthenbullet()
{
    BYTE purposecode[] = { "\x0F\x84\x98\xFE\xFF\xFF" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x64a96,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditStr;

        szEditStr.Format(L"Stengthen bullet routine implement failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(szEditStr);
    }

    return;
}



VOID WINAPI RecoverStrengthenbullet()
{
    BYTE sourcecode[] = { "\x0F\x85\x98\xFE\xFF\xFF" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x64a96,
        sourcecode,
        sizeof(sourcecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditStr;

        szEditStr.Format(L"Recover stengthen bullet routine implement failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(szEditStr);
    }

    return;
}


VOID WINAPI BTBullet()
{

    BYTE sourcecode[] = { "\x89\x87\x90\x00\x00\x00" };

    BYTE injuctcode[] = { "\xC7\x87\x90\x00\x00\x00\x01\x00\x00\x00" };

    STATUS status = STATUS::NoneError;

    status = g_crack->AssemblyInject32(
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6488f,
        injuctcode,
        sizeof(injuctcode) - 1,
        TRUE,
        sourcecode,
        sizeof(sourcecode) - 1,
        FALSE
    );

    if (status != STATUS::NoneError)
    {
        CString szEditStr;

        szEditStr.Format(L"Recover BT bullet routine implement failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(szEditStr);
    }

    return;
}


VOID WINAPI RecoverBTBullet()
{
    BYTE sourcecode[] = { "\x89\x87\x90\x00\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6488f,
        sourcecode,
        sizeof(sourcecode) - 1,
        NULL
    );

    if (!status)
    {
        CString    szEditStr;

        szEditStr.Format(L"Recover BT bullet routine implement failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->GetWindowTextW(szEditStr);
    }

    return;
}

VOID WINAPI ModifyCardSlotPlant(ULONG cardslot, ULONG plant)
{

    ULONG offset[] = { 0x2a6340,0x1d8,0x98,0x1c,0x15c,0x4,0x144,0x5c };

    offset[7] += 0x50 * cardslot;

    g_crack->ModifyLayerMemory32(g_crack->stGameInfo->lpBase, plant, offset, arraysize(offset) - 1);
}


VOID WINAPI Plant(ULONG row, ULONG column, ULONG plantType)
{
    /*
        60                       | pushad                                  |
        6A FF                    | push FFFFFFFF                           |  固定值-1
        6A 00                    | push 0                                  |  压入植物序号
        B8 01000000              | mov eax,1                               |  y坐标保存在eax中
        6A 02                    | push 2                                  |  把x坐标压入栈中
        8B1D C09E6A00            | mov ebx,dword ptr ds:[6A9EC0]           |  压入可变ebp
        8B9B 68070000            | mov ebx,dword ptr ds:[ebx+768]          |
        53                       | push ebx                                |
        BB 20D14000              | mov ebx,40D120                          |
        FFD3                     | call ebx                                |
        61                       | popad                                   |
        C3                       | ret                                     |

    */
    BYTE shellcode[] = { "\x60\x6A\xFF\x6A\x00\xB8\x01\x00\x00\x00\x6A\x02\x8B\x1D\xC0\x9E\x6A\x00\x8B\x9B\x68\x07\x00\x00\x53\xBB\x20\xD1\x40\x00\xFF\xD3\x61\xC3" };

    BOOLEAN status = FALSE;
    PUCHAR pRemoteBuffer = NULL, pRemoteThreadParam = NULL;
    DWORD dwColumn = column;
    BYTE bPlantType = (BYTE)plantType, bRow = (BYTE)row;

    *(PBYTE)(shellcode + 4) = bPlantType;

    *(PDWORD)(shellcode + 6) = bRow;

    *(PBYTE)(shellcode + 11) = dwColumn;

    g_crack->RemoteThreadExecuteShellCode(shellcode, sizeof(shellcode) - 1);
    return;
}

VOID WINAPI PlantOverlapping()
{
    BYTE purposecode[] = { "\xE9\x20\x09\x00\x00\x90" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0xfe2f,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;
        szEditString.Format(L"PlantOverlapping cover process memory failed!\n");
        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);

    }

    return;
}

VOID WINAPI RecoverPlantOverlappint()
{
    BYTE sourcecode[] = { "\x0F\x84\x1F\x09\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0xfe2f,
        sourcecode,
        sizeof(sourcecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"PlantOverlapping cover process memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);

    }

    return;
}

VOID WINAPI ZombiesPlant(ULONG row, ULONG column, ULONG zombies)
{

    /*
            60                       | pushad                                  |
            6A 00                    | push 0                                  |  row
            6A 01                    | push 1                                  |  zombies type
            B8 02000000              | mov eax,2                               |  column
            8B15 C09E6A00            | mov edx,dword ptr ds:[edx+6A9EC0]       |
            8B92 68070000            | mov edx,dword ptr ds:[edx+768]          |
            8B92 60010000            | mov edx,dword ptr ds:[edx+160]          |
            89D1                     | mov ecx,edx                             |
            BA F0A04200              | mov edx,42A0F0                          |
            FFD2                     | call edx                                |
            61                       | popad                                   |
            C3                       | ret                                     |
    */

    BYTE shellcode[] = { "\x60\x6A\x00\x6A\x01\xB8\x02\x00\x00\x00\x8B\x15\xC0\x9E\x6A\x00\x8B\x92\x68\x07\x00\x00\x8B\x92\x60\x01\x00\x00\x89\xD1\xBA\xF0\xA0\x42\x00\xFF\xD2\x61\xC3" };

    BOOLEAN status = FALSE;

    PUCHAR pRemoteBuffer = NULL, pRemoteThreadParam = NULL;

    BYTE bColumn = (BYTE)column, bZombies = (BYTE)zombies;

    DWORD dwRow = row;

    *(PBYTE)(shellcode + 2) = bColumn;

    *(PBYTE)(shellcode + 4) = bZombies;

    *(PDWORD)(shellcode + 6) = dwRow;

    g_crack->RemoteThreadExecuteShellCode(shellcode, sizeof(shellcode) - 1);

    return;
}


VOID WINAPI PurpleDirectPlant()
{
    BYTE lightcard[] = { "\xc3" }, directplantcode[] = { "\x74\x1a" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1d7d0,
        lightcard,
        sizeof(lightcard) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"PurpleDirectPlant cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0xe46c,
        directplantcode,
        sizeof(directplantcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"PurpleDirectPlant cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}



VOID WINAPI    RecoverPurpleDirectPlant()
{
    BYTE source[] = { "\x51" }, sourcedirectplantcode[] = { "\xeb\x1a" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1d7d0,
        source,
        sizeof(source) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RocoverPurpleDirectPlant cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0xe46c,
        sourcedirectplantcode,
        sizeof(sourcedirectplantcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RocoverPurpleDirectPlant cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

}


VOID WINAPI BackgroundRunGame()
{
    BYTE targetcode[] = { "\x90\x90\x90\x90\x90\x90" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x146310,
        targetcode,
        sizeof(targetcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"BackgroundRunGame cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}


VOID WINAPI RecoverBackgroundRunGame()
{
    BYTE targetcode[] = { "\x8b\x81\x2c\x03\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x146310,
        targetcode,
        sizeof(targetcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverBackgroundRunGame cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI KillAllZombies()
{
    BYTE purposecode[] = { "\x90\x90" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x12aef6,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"KillAllZombies cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI RecoverKillAllZombies()
{
    BYTE originalcode[] = { "\x75\x0c" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x12aef6,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"KillAllZombies cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI KillOneZombies()
{
    /*
        53                   | push ebx
        BB 44332211          | mov ebx,11223344
        8B03                 | mov eax,dword ptr ds:[ebx]
        C703 00000000        | mov dword ptr ds:[ebx],0
        83F8 00              | cmp eax,0
        74 07                | je first_test.4C3273
        C746 28 03000000     | mov dword ptr ds:[esi+28],3
        5B                   | pop ebx
    */

    BYTE originalcode[] = { "\x8b\x46\x28\x83\xf8\x02" }, injuctcode[] = { "\x53\xBB\x44\x33\x22\x11\x8B\x03\xC7\x03\x00\x00\x00\x00\x83\xF8\x00\x74\x07\xC7\x46\x28\x03\x00\x00\x00\x5B" };

    STATUS status = STATUS::NoneError;

    CString szEditString;

    lpkillZombiesFlagAddress = NULL;

    lpkillZombiesFlagAddress = VirtualAllocEx(
        g_crack->stGameInfo->hGame,
        NULL,
        sizeof(DWORD),
        MEM_COMMIT,
        PAGE_EXECUTE_READWRITE
    );

    if (!lpkillZombiesFlagAddress)
    {
        szEditString.Format(L"KillOneZombies allocate flag memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);

        return;
    }

    *(PDWORD32)(injuctcode + 2) = (DWORD32)lpkillZombiesFlagAddress;

    status = g_crack->AssemblyInject32(
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x12aece,
        injuctcode,
        sizeof(injuctcode) - 1,
        TRUE,
        originalcode,
        sizeof(originalcode) - 1,
        TRUE
    );

    if (status != STATUS::NoneError)
    {
        szEditString.Format(L"KillOneZombies injuct code failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI RecoverKillOneZombies()
{
    BYTE originalcode[] = { "\x8b\x46\x28\x83\xf8\x02" };

    BOOLEAN status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x12aece,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverKillOneZombies cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

/*

    Routine Description:
        This timer routine is generate random number.

*/

VOID RandomBulletNumberWriteTimer(HWND, UINT, UINT_PTR, DWORD)
{
    DWORD32 randomNumber = rand() % 13;

    BOOLEAN status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PVOID)lpRandomBulletNumber,
        &randomNumber,
        sizeof(DWORD32),
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RandomBulletNumberWriteTimer write bullet serial number failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}



VOID WINAPI RomdomizedBullet()
{
    /*

        B8 44332211 |   mov eax,11223344
        8B00        |   mov eax,dword ptr ds:[eax]
        3E:8945 5C  |   mov dword ptr ds:[ebp+5C],eax

    */
    STATUS status = STATUS::NoneError;

    BYTE injuctcode[] = { "\xB8\x44\x33\x22\x11\x8B\x00\x3E\x89\x45\x5C" },
        originalcode[] = { "\x89\x45\x5c\x8b\xc6\xd9\x5c\x24\x18" };

    CString szEditString;

    lpRandomBulletNumber = NULL;

    lpRandomBulletNumber = VirtualAllocEx(
        g_crack->stGameInfo->hGame,
        NULL,
        sizeof(DWORD32),
        MEM_COMMIT,
        PAGE_EXECUTE_READWRITE
    );

    if (!lpRandomBulletNumber)
    {

        szEditString.Format(L"Allocate remote memory that storage bullet serial number failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    SetTimer(AfxGetMainWnd()->GetSafeHwnd(), RANDOMIZEDBULLETTIMER, 10u, (TIMERPROC)RandomBulletNumberWriteTimer);

    *(PDWORD)(injuctcode + 1) = (DWORD32)lpRandomBulletNumber;

    status = g_crack->AssemblyInject32(
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6c769,
        injuctcode,
        sizeof(injuctcode) - 1,
        TRUE,
        originalcode,
        sizeof(originalcode) - 1,
        TRUE
    );

    if (status != STATUS::NoneError)
    {
        szEditString.Format(L"RomdomizedBullet routine injuct code failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI RecoverRomdomizedBullet()
{
    BYTE  originalcode[] = { "\x89\x45\x5c\x8b\xc6\xd9\x5c\x24\x18" };

    BOOLEAN status = NULL;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6c769,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverRomdomizedBullet cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}



VOID WINAPI ClayPotPerspective()
{
    BYTE originalcode[] = { "\x83\xc4\x14\x83\x7d\x4c\x00" }, injuctcode[] = { "\xC7\x45\x4C\x50\x00\x00\x00" };

    STATUS status = STATUS::NoneError;

    status = g_crack->AssemblyInject32(
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x4dbf1,
        injuctcode,
        sizeof(injuctcode) - 1,
        TRUE,
        originalcode,
        sizeof(originalcode) - 1,
        TRUE
    );

    if (status != STATUS::NoneError)
    {
        CString szEditString;

        szEditString.Format(L"ClayPotPerspective cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI RecoverClayPotPerspective()
{
    BYTE originalcode[] = { "\x83\xc4\x14\x83\x7d\x4c\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x4dbf1,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverClayPotPerspective cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}


VOID WINAPI QuickConveyorBelt()
{
    BYTE originalcode[] = { "\x83\x43\x5c\xff\x83\x7b\x5c\x00" },
        injuctcode[] = { "\xC7\x43\x5C\x00\x00\x00\x00" },
        covercode[] = { "\x31\xc0" };

    CString szEditString;

    STATUS status = g_crack->AssemblyInject32(
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x22d17,
        injuctcode,
        sizeof(injuctcode) - 1,
        TRUE,
        originalcode,
        sizeof(originalcode) - 1,
        TRUE
    );

    if (status != STATUS::NoneError)
    {
        szEditString.Format(L"QuickConveyorBelt injuct code failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    if (!WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x89ca1,
        covercode,
        sizeof(covercode) - 1,
        NULL
    ))
    {
        szEditString.Format(L"QuickConveyorBelt routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}


VOID WINAPI RecoverQucikConveyorBelt()
{
    BYTE originalcode[] = { "\x83\x43\x5c\xff\x83\x7b\x5c\x00" },
        sourcecovercode[] = { "\x85\xc0" };

    CString szEditString;

    BOOLEAN status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x22d17,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        szEditString.Format(L"QuickConveyorBelt injuct code failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    if (!WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x89ca1,
        sourcecovercode,
        sizeof(sourcecovercode) - 1,
        NULL
    ))
    {
        szEditString.Format(L"QuickConveyorBelt routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
    return;
}

VOID WINAPI ListPlant()
{
    BYTE purposecode[] = { "\x90\x90\x90\x90\x90\x90" };

    BOOLEAN status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x10ae6,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"ListPlant cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI RecoverListPlant()
{
    BYTE originalcode[] = { "\x0f\x85\xe5\x00\x00\x00" };

    BOOLEAN status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x10ae6,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverListPlant cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}



VOID WINAPI ChangeCornBulletToButter()
{
    /*
        83F8 0A        | cmp eax,A
        75 05          | jne first_test.4C3284
        B8 0C000000    | mov eax,C
    */
    STATUS status = STATUS::NoneError;

    BYTE originalcode[] = { "\x89\x45\x5c\x8b\xc6\xd9\x5c\x24\x18" },
        injuctcode[] = { "\x83\xF8\x0A\x75\x05\xB8\x0C\x00\x00\x00" };

    status = g_crack->AssemblyInject32(
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6c769,
        injuctcode,
        sizeof(injuctcode) - 1,
        TRUE,
        originalcode,
        sizeof(originalcode) - 1,
        TRUE
    );

    if (status != STATUS::NoneError)
    {
        CString szEditString;

        szEditString.Format(L"RecoverChangeCornBullet routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI CleanTombstone()
{
    BOOLEAN status = FALSE;

    BYTE originalcode[] = { "\x74\x96" },
        purposecode[] = { "\x90\x90" };

    CString szEditString;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1be28,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        szEditString.Format(L"CleanTombstone routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    Sleep(5000);

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1be28,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        szEditString.Format(L"CleanTombstone routine recover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI CleanPlants()
{
    BOOLEAN status = FALSE;

    BYTE originalcode[] = { "\x74\xb0" },
        purposecode[] = { "\x90\x90" };

    CString szEditString;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1bb2e,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        szEditString.Format(L"CleanPlants routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    Sleep(1000);

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1bb2e,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        szEditString.Format(L"CleanPlants routine recover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}


VOID RandomPlantTimer(HWND, UINT, UINT_PTR, DWORD)
{
    DWORD32 randomNumber = rand() % 53;

    DWORD offset[] = { 0x96bc8,0x8,0x138,0x28 };

    STATUS status = g_crack->ModifyLayerMemory32(
        g_crack->stGameInfo->lpBase,
        randomNumber,
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        CString szEditString;

        szEditString.Format(L"RandomPlantTimer write plant serial number failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}

VOID WINAPI RomdomPlant()
{
    SetTimer(AfxGetMainWnd()->GetSafeHwnd(), RANDOMIZEDPLANTTIMER, 10u, (TIMERPROC)RandomPlantTimer);

    return;
}

VOID WINAPI CancelRomdomPlant()
{
    KillTimer(AfxGetMainWnd()->GetSafeHwnd(), RANDOMIZEDPLANTTIMER);

    return;
}

VOID WINAPI ChangeCornBulletToBomb()
{
    /*
        83F8 0A          | cmp eax,A
        75 05            | jne first_test.4C3266
        B8 0B000000      | mov eax,B
        83F8 0C          | cmp eax,C
        75 05            | jne first_test.4C3270
        B8 0B000000      | mov eax,B
    */

    STATUS status = STATUS::NoneError;

    BYTE originalcode[] = { "\x89\x45\x5c\x8b\xc6\xd9\x5c\x24\x18" },
        injuctcode[] = { "\x83\xF8\x0A\x75\x05\xB8\x0B\x00\x00\x00\x83\xF8\x0C\x75\x05\xB8\x0B\x00\x00\x00" };

    status = g_crack->AssemblyInject32(
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6c769,
        injuctcode,
        sizeof(injuctcode) - 1,
        TRUE,
        originalcode,
        sizeof(originalcode) - 1,
        TRUE
    );

    if (status != STATUS::NoneError)
    {
        CString szEditString;

        szEditString.Format(L"RecoverChangeCornBullet routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}


VOID WINAPI RecoverChangeCornBullet()
{
    BYTE originalcode[] = { "\x89\x45\x5c\x8b\xc6\xd9\x5c\x24\x18" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6c769,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverChangeCornBullet routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    return;
}

VOID WINAPI BigMouthFlowerColdDown()
{
    BYTE purposecode[] = { "\xc7\x47\x54\x00\x00\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6154e,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"BigMouthFlowerColdDown routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}

VOID WINAPI    RecoverBigMouthFlowerColdDown()
{
    BYTE originalcode[] = { "\xc7\x47\x54\xa0\x0f\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6154e,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverBigMouthFlowerColdDown routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI PotatoMineColdDown()
{
    BYTE purposecode[] = { "\xc7\x40\x54\x00\x00\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x5e34b,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"PotatoMineColdDown routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI    RecoverPotatoMineColdDown()
{
    BYTE originalcode[] = { "\xc7\x40\x54\xdc\x05\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x5e34b,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverPotatoMineColdDown routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI CornCannonColdDown()
{
    BYTE purposecode[] = { "\xC7\x47\x54\x00\x00\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x64d4a,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"CornCannonColdDown routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI    RecoverCornCannonColdDown()
{
    BYTE originalcode[] = { "\xC7\x47\x54\xB8\x0B\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x61637,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverCornCannonColdDown routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI MagneticMushRoomColdDown()
{
    BYTE purposecode[] = { "\xC7\x45\x54\x00\x00\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x61637,
        purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"MagneticMushRoomColdDown routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI    RecoverMagneticMushRoomColdDown()
{
    BYTE originalcode[] = { "\xC7\x45\x54\xDC\x05\x00\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6154e,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverMagneticMushRoomColdDown routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}



VOID WINAPI ClearFog()
{
    BYTE originalcode[] = { "\x8B\x02\x85\xC0\x89\x44\x24\x3C" },
        injuctcode[] = { "\xC7\x02\x00\x00\x00\x00" };

    STATUS status = STATUS::NoneError;

    CString szEditString;

    status = g_crack->AssemblyInject32(
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1a78f,
        injuctcode,
        sizeof(injuctcode) - 1,
        TRUE,
        originalcode,
        sizeof(originalcode) - 1,
        TRUE
    );

    if (status != STATUS::NoneError)
    {
        szEditString.Format(L"ClearFog routine injuct code failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}

VOID WINAPI RecoverClearFog()
{
    BYTE originalcode[] = { "\x8B\x02\x85\xC0\x89\x44\x24\x3C" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1a78f,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverClearFog routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI MushRoomDonotSleep()
{
    BYTE originalcode[] = { "\xeb\x1d" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1a78f,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"MushRoomDonotSleep routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI RecoverMushRoomDonotSleep()
{
    BYTE originalcode[] = { "\x74\x1d" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x1a78f,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverMushRoomDonotSleep routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}



VOID WINAPI SpikeZombies()
{
    BYTE purpose[] = { "\x90\x90" };

    BOOLEAN status = FALSE;

    CString szEditString;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x13178a,
        purpose,
        sizeof(purpose) - 1,
        NULL
    );

    if (!status)
    {
        szEditString.Format(L"RecoverSpikeZombies routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x131066,
        purpose,
        sizeof(purpose) - 1,
        NULL
    );

    if (!status)
    {
        szEditString.Format(L"RecoverSpikeZombies routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

}


VOID WINAPI RecoverSpikeZombies()
{
    BYTE originalcode1[] = { "\x7f\x1d" },
        originalcode2[] = { "\x75\x11" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x13178a,
        originalcode1,
        sizeof(originalcode1) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverSpikeZombies routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x131066,
        originalcode2,
        sizeof(originalcode2) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverSpikeZombies routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}

// 8B 46 04  56  50  E8 E580EEFF  83 C7 46

VOID WINAPI UndefeatedForever()
{
    BYTE originalcode[] = { "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x12b311,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverUndefeatedForever routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}

VOID WINAPI RecoverUndefeatedForever()
{
    BYTE originalcode[] = { "\x8B\x46\x04\x56\x50\xE8\xE5\x80\xEE\xFF\x83\xC7\x46" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x12b311,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverUndefeatedForever routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}

VOID WINAPI PenetrateBullet()
{
    BYTE purpssecode[] = { "\xc6\x46\x50\x00" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6eb2b,
        purpssecode,
        sizeof(purpssecode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverUndefeatedForever routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}

VOID WINAPI RecoverPenetrateBullet()
{
    BYTE originalcode[] = { "\xc6\x46\x50\x01" };

    BOOLEAN status = FALSE;

    status = WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6eb2b,
        originalcode,
        sizeof(originalcode) - 1,
        NULL
    );

    if (!status)
    {
        CString szEditString;

        szEditString.Format(L"RecoverUndefeatedForever routine cover memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }
}


VOID WINAPI ModifyMoney()
{
    BOOLEAN status = FALSE;

    CString editstr;

    g_dialog->GetDlgItem(IDC_EDIT3)->GetWindowTextW(editstr);

    DWORD num = _ttol(editstr);

    ULONG offset[] = { 0x0009beec ,0x60, 0x8 , 0x4, 0x58 };

    g_crack->ModifyLayerMemory32(g_crack->stGameInfo->lpBase, num, offset, arraysize(offset) - 1);

    return;
}


VOID WINAPI BulletTracking()
{
    BYTE purpose0[] = { "\xeb\x43" },
        purpose1[] = { "\x90\x90" },
        purpose2[] = { "\x90\x90" },
        purpose3[] = { "\x90\x90\x90\x90\x90\x90" },
        purpose4[] = { "\x90\x90\x90\x90\x90\x90" },
        purpose5[] = { "\x90\x90\x90\x90\x90\x90" },
        purpose6[] = { "\x90\x90\x90\x90\x90\x90" },
        purpose7[] = { "\x90\x90\x90\x90\x90\x90" },
        purpose8[] = { "\x90\x90\x90\x90\x90\x90" },
        purpose9[] = { "\xeb\x59" },
        purpose10[] = { "\xeb\x10" };

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x67683,
        purpose0,
        sizeof(purpose0) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x64a60,
        purpose1,
        sizeof(purpose1) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x67384,
        purpose2,
        sizeof(purpose2) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x67352,
        purpose3,
        sizeof(purpose3) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6735b,
        purpose4,
        sizeof(purpose4) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672d0,
        purpose5,
        sizeof(purpose5) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672d9,
        purpose6,
        sizeof(purpose6) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672e2,
        purpose7,
        sizeof(purpose7) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672eb,
        purpose8,
        sizeof(purpose8) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672f4,
        purpose9,
        sizeof(purpose9) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x5eb16,
        purpose10,
        sizeof(purpose10) - 1,
        NULL
    );
}


VOID WINAPI RecoverBulletTracking()
{
    BYTE originalcode0[] = { "\x74\x43" },
        originalcode1[] = { "\x75\x2d" },
        originalcode2[] = { "\x75\x2a" },
        originalcode3[] = { "\x0f\x84\xcd\x00\x00\x00" },
        originalcode4[] = { "\x0f\x84\xc4\x00\x00\x00" },
        originalcode5[] = { "\x0f\x84\x60\x01\x00\x00" },
        originalcode6[] = { "\x0f\x84\x57\x01\x00\x00" },
        originalcode7[] = { "\x0f\x84\x4e\x01\x00\x00" },
        originalcode8[] = { "\x0f\x84\x45\x01\x00\x00" },
        originalcode9[] = { "\x75\x59" },
        originalcode10[] = { "\x75\x10" };


    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x67683,
        originalcode0,
        sizeof(originalcode1) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x64a60,
        originalcode1,
        sizeof(originalcode1) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x67384,
        originalcode2,
        sizeof(originalcode2) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x67352,
        originalcode3,
        sizeof(originalcode3) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x6735b,
        originalcode4,
        sizeof(originalcode4) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672d0,
        originalcode5,
        sizeof(originalcode5) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672d9,
        originalcode6,
        sizeof(originalcode6) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672e2,
        originalcode7,
        sizeof(originalcode7) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672eb,
        originalcode8,
        sizeof(originalcode8) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x672f4,
        originalcode9,
        sizeof(originalcode9) - 1,
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x5eb16,
        originalcode10,
        sizeof(originalcode10) - 1,
        NULL
    );
}
/*
         0x458555

         60                       | pushad                                  |
         BE 44332211              | mov esi,11223344                        |
         BA A08D4500              | mov edx,458DA0                          |
         FFD2                     | call edx                                |
         61                       | popad                                   |
         C3                       | ret                                     |
*/

VOID WINAPI StartupFiveCar()
{
    STATUS status = STATUS::NoneError;

    BYTE code[] = { "\x60\xBE\x44\x33\x22\x11\xBA\xA0\x8D\x45\x00\xFF\xD2\x61\xC3" },
        purposecode[] = { "\xc6\x47\x30\x00" };

    ULONG dwCarEsi = 0, offset[] = { 0x2a9ec0, 0x768, 0x100 };

    float carlocation = -15;

    DWORD dwOldPageAttribute = 0;

    CString szEditString;

    VirtualProtectEx(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x279bf8,
        sizeof(float),
        PAGE_EXECUTE_READWRITE,
        &dwOldPageAttribute
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x279bf8,
        &carlocation,
        sizeof(float),
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x58d1c,
        &purposecode,
        sizeof(purposecode) - 1,
        NULL
    );


    status = g_crack->ReadLayerMemory32(
        g_crack->stGameInfo->lpBase,
        &dwCarEsi,
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        szEditString.Format(L"StartupAllCar routine read memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    for (ULONG i = 0; i < 5; i++)
    {
        *(PDWORD)(code + 2) = dwCarEsi + i * 0x48;

        status = g_crack->RemoteThreadExecuteShellCode(
            code,
            sizeof(code) - 1
        );

        if (status != STATUS::NoneError)
        {
            szEditString.Format(L"StartupAllCar start up remote thread failed!\n");

            g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
        }
    }
}

/*
        0x40bd0f

        60                       | pushad                                  |
        68 44332211              | push 11223344                           |
        B8 00000000              | mov eax,00000000                        |
        BA 00804500              | mov edx,458000                          |
        FFD2                     | call edx                                |
        61                       | popad                                   |
        C3                       | ret                                     |
*/
VOID WINAPI RecoverFiveCar()
{
    STATUS status = STATUS::NoneError;

    BYTE code[] = { "\x60\x68\x44\x33\x22\x11\xB8\x00\x00\x00\x00\xBA\x00\x80\x45\x00\xFF\xD2\x61\xC3" };

    ULONG dwCarEsi = 0, offset[] = { 0x2a9ec0, 0x768, 0x100 };

    CString szEditString;

    status = g_crack->ReadLayerMemory32(
        g_crack->stGameInfo->lpBase,
        &dwCarEsi,
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        szEditString.Format(L"StartupAllCar routine read memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    for (ULONG i = 0; i < 5; i++)
    {
        *(PDWORD)(code + 2) = dwCarEsi + i * 0x48;

        *(PDWORD)(code + 7) = i;

        status = g_crack->RemoteThreadExecuteShellCode(
            code,
            sizeof(code) - 1
        );

        if (status != STATUS::NoneError)
        {
            szEditString.Format(L"StartupAllCar start up remote thread failed!\n");

            g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
        }
    }
}



VOID WINAPI StartupSixCar()
{
    STATUS status = STATUS::NoneError;

    BYTE code[] = { "\x60\xBE\x44\x33\x22\x11\xBA\xA0\x8D\x45\x00\xFF\xD2\x61\xC3" },
        purposecode[] = { "\xc6\x47\x30\x00" };

    ULONG dwCarEsi = 0, offset[] = { 0x2a9ec0, 0x768, 0x100 };

    float carlocation = -15;

    DWORD dwOldPageAttribute = 0;

    CString szEditString;

    VirtualProtectEx(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x279bf8,
        sizeof(float),
        PAGE_EXECUTE_READWRITE,
        &dwOldPageAttribute
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x279bf8,
        &carlocation,
        sizeof(float),
        NULL
    );

    WriteProcessMemory(
        g_crack->stGameInfo->hGame,
        (PUCHAR)g_crack->stGameInfo->lpBase + 0x58d1c,
        &purposecode,
        sizeof(purposecode) - 1,
        NULL
    );

    status = g_crack->ReadLayerMemory32(
        g_crack->stGameInfo->lpBase,
        &dwCarEsi,
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        szEditString.Format(L"StartupAllCar routine read memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    for (ULONG i = 0; i < 6; i++)
    {
        *(PDWORD)(code + 2) = dwCarEsi + i * 0x48;

        status = g_crack->RemoteThreadExecuteShellCode(
            code,
            sizeof(code) - 1
        );

        if (status != STATUS::NoneError)
        {
            szEditString.Format(L"StartupAllCar start up remote thread failed!\n");

            g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
        }
    }
}

VOID WINAPI RecoverSixCar()
{
    STATUS status = STATUS::NoneError;

    BYTE code[] = { "\x60\x68\x44\x33\x22\x11\xB8\x00\x00\x00\x00\xBA\x00\x80\x45\x00\xFF\xD2\x61\xC3" };

    ULONG dwCarEsi = 0, offset[] = { 0x2a9ec0, 0x768, 0x100 };

    CString szEditString;

    status = g_crack->ReadLayerMemory32(
        g_crack->stGameInfo->lpBase,
        &dwCarEsi,
        offset,
        arraysize(offset) - 1
    );

    if (status != STATUS::NoneError)
    {
        szEditString.Format(L"StartupAllCar routine read memory failed!\n");

        g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
    }

    for (ULONG i = 0; i < 6; i++)
    {
        *(PDWORD)(code + 2) = dwCarEsi + i * 0x48;

        *(PDWORD)(code + 7) = i;

        status = g_crack->RemoteThreadExecuteShellCode(
            code,
            sizeof(code) - 1
        );

        if (status != STATUS::NoneError)
        {
            szEditString.Format(L"StartupAllCar start up remote thread failed!\n");

            g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);
        }
    }
}

VOID FiveCarThread() {
    StartupFiveCar();
    Sleep(6000);
    RecoverFiveCar();
}

VOID FiveCar(HWND, UINT, UINT_PTR, DWORD)
{
    std::thread bp_thread(FiveCarThread);
    bp_thread.detach();
}

VOID WINAPI SetFiveCarTimer()
{
    SetTimer(AfxGetMainWnd()->GetSafeHwnd(), FIVECARTIMER, 10000u, (TIMERPROC)FiveCar);

    return;
}

VOID WINAPI CancelFiveCarTimer()
{
    KillTimer(AfxGetMainWnd()->GetSafeHwnd(), FIVECARTIMER);

    return;
}

VOID SixCarThread() {
    StartupSixCar();
    Sleep(6000);
    RecoverSixCar();
}

VOID SixCar(HWND, UINT, UINT_PTR, DWORD)
{
    std::thread bp_thread(SixCarThread);
    bp_thread.detach();
}

VOID WINAPI SetSixCarTimer()
{
    SetTimer(AfxGetMainWnd()->GetSafeHwnd(), SIXCARTIMER, 10000u, (TIMERPROC)SixCar);
    return;
}

VOID WINAPI CancelSixCarTimer()
{
    KillTimer(AfxGetMainWnd()->GetSafeHwnd(), SIXCARTIMER);
    return;
}
