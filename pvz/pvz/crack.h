#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include <stdexcept>
#define arraysize(arr) (sizeof(arr)/sizeof(arr[0]))

enum class STATUS
{
    NoneError = 0,
    WriteProcessMemoryError = 1,
    ReadProcessMemoryError,
    AllocateRemoteVirtualMemoryError,
    ProcessOperationError,
    CreateRetmoteThreadError,
};

typedef struct _GAME_INFORMATION_
{
    LPWSTR  szGameName;
    DWORD   dwpid;
    HANDLE  hGame;
    PVOID   lpBase;
}GAME_INFORMATION, * HGAME_INFORMATION;


class CrackRE32
{
public:
    HGAME_INFORMATION stGameInfo = { 0 };

    CrackRE32(_In_ LPWSTR szGameStr);

    ~CrackRE32();

    BOOLEAN InitGameInformation(
        _In_ LPWSTR szGameName
    );

    DWORD    GetProcessPID(
        _In_ LPWSTR szGaName
    );

    PVOID    GetModuleBaseAddress(
        _In_ LPWSTR szModuleName
    );

    STATUS    elevateprivileges();

    STATUS ModifyLayerMemory32(
        _In_ PVOID lpBase,
        _In_ ULONG num,
        _In_ PULONG offset,
        _In_ ULONG offsetMaxIndex
    );

    STATUS ReadLayerMemory32(
        _In_ PVOID lpBase,
        _Out_ PULONG pOutputNum,
        _In_ PULONG offset,
        _In_ ULONG offsetMaxIndex
    );

    STATUS AssemblyInject32(
        _In_ PVOID lpModifiedAddress,
        _In_ BYTE* injuctcode,
        _In_ ULONG injuctcodelength,
        _In_ BOOLEAN injuctcodeWriteOrNot,
        _In_ BYTE* sourcecode,
        _In_ ULONG sourcecodelength,
        _In_ BOOLEAN sourcecodeWriteOrNot
    );

    STATUS RemoteThreadExecuteShellCode(
        _In_ BYTE* shellcode,
        _In_ ULONG shellcodelength
    );
};

