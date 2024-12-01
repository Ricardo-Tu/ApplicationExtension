#include "pch.h"
#include "context.hpp"
#include "tools.hpp"



namespace game {
    /*
   Routine Description:

       This routine get module base address by module full name.

   Arguments:

       szModuleName - Supplies the module Wide character string of the whole process command line that you want get base address.

    ReturnValue:

       PVOID - Module base address that you Supplies.

*/
    void* ModuleContext::GetModuleBaseAddress(std::wstring szModuleName)
    {
        HANDLE    hModuleSnap = INVALID_HANDLE_VALUE;

        MODULEENTRY32* lpMe32 = new MODULEENTRY32;

        hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GamePid);

        lpMe32->dwSize = sizeof(MODULEENTRY32);

        if (hModuleSnap == INVALID_HANDLE_VALUE)
        {
            DebugPrint(__FUNCTIONW__, L"Get module snap error!\n");
            return NULL;
        }

        if (!Module32First(hModuleSnap, lpMe32))
        {
            DebugPrint(__FUNCTIONW__, L"Error module32first snap!\n");
            CloseHandle(hModuleSnap);
            return NULL;
        }

        do
        {
            if (wcscmp(lpMe32->szModule, szModuleName.c_str()) == 0)
            {
                CloseHandle(hModuleSnap);

                return lpMe32->modBaseAddr;
            }
        } while (Module32Next(hModuleSnap, lpMe32));

        CloseHandle(hModuleSnap);
        return NULL;
    }

    /*

    Routine Description:

        This is routine injuct shellcode.

    Arguments:

        lpTargetInjuctAddress - Address that will be injuct shellcode.
        inJuctCode - Data address of shellcode.
        inJuctCodeWriteOrNot - Whether the shellcode is written in the head of the application address applied to the target process.
        sourcecode - Data address of sourcecode that will be overwrite.
        sourcecodeWriteOrNot - Whether the sourcecode is written in the after shellcode.

    ReturnValue:

        NTSTATUS - Status of operation.

*/
    bool ModuleContext::AssemblyInject32(
        void* lpTargetInjuctAddress,
        std::vector<byte> inJuctCode,
        bool inJuctCodeWriteOrNot,
        std::vector<byte> sourceCode,
        bool sourceCodeWriteOrNot
    )
    {
        bool status = TRUE;

        char* pRemoteBuffer = NULL;

        BYTE JmpCode[] = "\x68\x44\x33\x22\x11\xC3";

        if (sourceCode.size() < 0x100)
            pRemoteBuffer = (char*)VirtualAllocEx(hGame,
                NULL,
                0x100,
                MEM_COMMIT,
                PAGE_EXECUTE_READWRITE
            );
        else
            pRemoteBuffer = (char*)VirtualAllocEx(hGame,
                NULL,
                0x1000,
                MEM_COMMIT,
                PAGE_EXECUTE_READWRITE
            );

        if (!pRemoteBuffer)
            return false;

        if (inJuctCodeWriteOrNot)
        {
            status = WriteProcessMemory(
                hGame,
                pRemoteBuffer,
                byteVectorToString(inJuctCode).c_str(),
                inJuctCode.size(),
                NULL);

            if (!status)
                return false;
        }

        if (sourceCodeWriteOrNot)
        {
            status = WriteProcessMemory(
                hGame,
                (PUCHAR)pRemoteBuffer + (inJuctCodeWriteOrNot ? inJuctCode.size() : 0),
                byteVectorToString(sourceCode).c_str(),
                sourceCode.size(),
                NULL);

            if (!status)
                return false;
        }

        *(PDWORD)(JmpCode + 1) = (DWORD)lpTargetInjuctAddress + sourceCode.size();

        status = WriteProcessMemory(hGame,
            (PUCHAR)pRemoteBuffer + (inJuctCodeWriteOrNot ? inJuctCode.size() : 0) + (sourceCodeWriteOrNot ? sourceCode.size() : 0),
            JmpCode,
            sizeof(JmpCode) - 1,
            NULL
        );

        if (!status)
            return false;

        *(PDWORD)(JmpCode + 1) = (DWORD)pRemoteBuffer;

        status = WriteProcessMemory(hGame,
            lpTargetInjuctAddress,
            JmpCode,
            sizeof(JmpCode) - 1,
            NULL
        );

        if (!status)
            return false;
        return true;
    }


    /*

    Routine Description:
        This is routine start up remote thread in target process.

    Arguments:
        shellcode - The shellocde that will execute in remote thread.

    ReturnValue:
        bool - Status of operation.

*/
    bool ModuleContext::RemoteThreadExecuteShellCode(std::vector<byte> shellCode)
    {
        bool status = TRUE;
        char* pRemoteBuffer = NULL, * pRemoteThreadParam = NULL;

        byte initparam[0x100] = { 0 };

        pRemoteThreadParam = (char*)VirtualAllocEx(
            hGame,
            NULL,
            0x100,
            MEM_COMMIT,
            PAGE_EXECUTE_READWRITE);

        if (!pRemoteThreadParam)
            return false;

        status = WriteProcessMemory(
            hGame,
            (void*)pRemoteThreadParam,
            initparam,
            0x100,
            NULL);

        if (!status)
            return status;

        pRemoteBuffer = (char*)VirtualAllocEx(
            hGame,
            NULL,
            0x100,
            MEM_COMMIT,
            PAGE_EXECUTE_READWRITE);

        if (!pRemoteBuffer)
            return false;

        status = WriteProcessMemory(
            hGame,
            (void*)pRemoteBuffer,
            byteVectorToString(shellCode).c_str(),
            shellCode.size(),
            NULL);

        if (!status)
            return status;

        HANDLE hRemoteThread = CreateRemoteThread(
            hGame,
            NULL,
            0,
            (LPTHREAD_START_ROUTINE)pRemoteBuffer,
            pRemoteThreadParam,
            0,
            NULL);

        if (hRemoteThread == INVALID_HANDLE_VALUE)
            return false;

        return true;
    }


    ModuleContext::ModuleContext(std::wstring moduleName, uint32_t pid, HANDLE hGame) : GamePid(pid), hGame(hGame)
    {
        lpModuleAddress = GetModuleBaseAddress(moduleName);
        if (!lpModuleAddress)
        {
            DebugPrint(__FUNCTIONW__, L"Get module base address failed!\n");
        }
    }

    ModuleContext::~ModuleContext()
    {

    }


    /*

        Routine Description:
            Evevate process privileges before modifying other process.

        Arguments:
            None

        ReturnValue:
            Status - Status of operation

    */
    bool GameContext::elevateprivileges()
    {

        bool status = TRUE;
        HANDLE    hToken = NULL;
        LUID    luid = { 0 };
        TOKEN_PRIVILEGES    tp = { 0 };
        status = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);

        if (!status)
            return false;

        status = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
        if (!status)
            return false;

        if (!status)
        {
            CloseHandle(hToken);
            return false;
        }

        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        status = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
        if (!status)
        {
            DebugPrint(__FUNCTIONW__, L"Elevate privilege failed!\n");
            CloseHandle(hToken);
            return false;
        }

        CloseHandle(hToken);
        return true;
    }

    /*

    Routine Description:
        This Routine get process ID by process name.

    Arguments:
        GameName - Supplies the process that you want to get PID full name wide string .

    Return Value:
        Return target process ID that you want to get to if all routines implement successfully.

    */
    uint32_t GameContext::GetProcessPID(std::wstring gameName)
    {
        DWORD status = TRUE;
        HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
        PROCESSENTRY32* lpInfo = new PROCESSENTRY32;
        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        lpInfo->dwSize = sizeof(PROCESSENTRY32);

        if (hProcessSnap == INVALID_HANDLE_VALUE)
        {
            DebugPrint(__FUNCTIONW__, L"Getprocess snap shot failed!\n");
            return NULL;
        }

        if (!Process32First(hProcessSnap, lpInfo))
        {
            DebugPrint(__FUNCTIONW__, L"Error process32first snap!\n");
            return NULL;
        }

        do
        {
            if (wcscmp(lpInfo->szExeFile, gameName.c_str()) == 0)
            {
                CloseHandle(hProcessSnap);

                return lpInfo->th32ProcessID;
            }
        } while (Process32Next(hProcessSnap, lpInfo));

        return status;
    }

    bool GameContext::AddModule(std::wstring moduleName, HANDLE hGame)
    {
        std::unique_ptr<ModuleContext> module = std::make_unique<ModuleContext>(
            moduleName,
            gamePid,
            hGame);
        modules[moduleName] = std::move(module);
        return true;
    }

    GameContext::GameContext(std::wstring GameName)
    {
        gamePid = GetProcessPID(GameName);
        if (!gamePid)
        {
            DebugPrint(__FUNCTIONW__, L"Get process pid failed!\n");
        }
        hGame = OpenProcess(PROCESS_ALL_ACCESS, FALSE, gamePid);
        AddModule(GameName, hGame);
        lpGameBaseAddress = modules[GameName]->lpModuleAddress;
    }

    GameContext::~GameContext()
    {
        for (auto& module : modules)
            module.second.reset();
    }
}