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

            This routine write data to multi-level offset pointers

        Arguments:

            lpBase - Modify module base address.
            num - Data that will be written in.
            offset - The offset array address of each level pointer relative to the base address of the module.
            offsetMaxIndex - The offset array max index.

        ReturnValue:

            NTSTATUS - Status of operation.

    */
    boolean ModuleContext::ModifyMultiLevelLayerMemory32(uint32_t num, std::vector<uint32_t> layerOffset)
    {
        boolean status = FALSE;

        char* pTemp = (char*)lpModuleAddress;

        for (uint32_t i = 0; i < layerOffset.size() - 1; i++)
        {
            pTemp = (char*)pTemp + (uint32_t)(layerOffset[i]);

            ReadProcessMemory(hGame, (void*)pTemp, &pTemp, sizeof(uint32_t), NULL);
        }

        if (!pTemp)
            return false;

        pTemp = (char*)pTemp + (uint32_t)(layerOffset.back());

        status = WriteProcessMemory(hGame, (void*)pTemp, &num, sizeof(uint32_t), NULL);

        if (!status)
            return false;

        return true;
    }

    boolean ModuleContext::ModifyMultiLevelLayerMemory32float(float num, std::vector<uint32_t> layerOffset)
    {
        boolean status = FALSE;

        char* pTemp = (char*)lpModuleAddress;

        for (uint32_t i = 0; i < layerOffset.size() - 1; i++)
        {
            pTemp = (char*)pTemp + (uint32_t)(layerOffset[i]);

            ReadProcessMemory(hGame, (void*)pTemp, &pTemp, sizeof(uint32_t), NULL);
        }

        if (!pTemp)
            return false;

        pTemp = (char*)pTemp + (uint32_t)(layerOffset.back());

        status = WriteProcessMemory(hGame, (void*)pTemp, &num, sizeof(uint32_t), NULL);

        if (!status)
            return false;

        return true;
    }

    /*

    Routine Description:

        This is routine read data from multi-level offset pointers

    Arguments:

        lpBase - Modify module base address.
        num - Data stored after reading.
        offset - The offset array address of each level pointer relative to the base address of the module.
        offsetMaxIndex - The offset array max index.

    ReturnValue:

        NTSTATUS - Status of operation.

*/
    boolean ModuleContext::ReadMultiLevelMemory32(uint32_t* pOutputNum, std::vector<uint32_t> layerOffset)
    {
        char* pTemp = (char*)lpModuleAddress;

        for (ULONG i = 0; i < layerOffset.size(); i++)
        {
            pTemp = (char*)pTemp + (uint32_t)(layerOffset[i]);
            ReadProcessMemory(hGame, (void*)pTemp, &pTemp, sizeof(uint32_t), NULL);
        }

        *pOutputNum = (uint32_t)pTemp;

        if (!pTemp)
            return false;
        return true;
    }

    boolean ModuleContext::ReadMultiLevelMemory32float(float* pOutputNum, std::vector<uint32_t> layerOffset)
    {
        char* pTemp = (char*)lpModuleAddress;

        for (ULONG i = 0; i < layerOffset.size(); i++)
        {
            pTemp = (char*)pTemp + (uint32_t)(layerOffset[i]);
            ReadProcessMemory(hGame, (void*)pTemp, &pTemp, sizeof(uint32_t), NULL);
        }

        *pOutputNum = *(float*)&pTemp;

        if (!pTemp)
            return false;
        return true;
    }

    /*

    Routine Description:

        This is routine injuct shellcode.

    Arguments:

        lpTargetInjuctAddress - Address that will be injuct shellcode.
        injuctcode - Data address of shellcode.
        injuctcodelength - Sizeof shellcode that will be injuct.
        injuctcodeWriteOrNot - Whether the shellcode is written in the head of the application address applied to the target process.
        sourcecode - Data address of sourcecode that will be overwrite.
        sourcecodelength - Sizeof sourcecode that will be write.
        sourcecodeWriteOrNot - Whether the sourcecode is written in the after shellcode.

    ReturnValue:

        NTSTATUS - Status of operation.

*/
    boolean ModuleContext::AssemblyInject32(
        void* lpTargetInjuctAddress,
        std::vector<byte> InjuctCode,
        boolean InjuctCodeWriteOrNot,
        std::vector<byte> SourceCode,
        boolean SourceCodeWriteOrNot
    )
    {
        boolean status = TRUE;

        char* pRemoteBuffer = NULL;

        BYTE JmpCode[] = "\x68\x44\x33\x22\x11\xC3";

        if (SourceCode.size() < 0x100)
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

        if (InjuctCodeWriteOrNot)
        {
            status = WriteProcessMemory(
                hGame,
                pRemoteBuffer,
                byteVectorToString(InjuctCode).c_str(),
                InjuctCode.size(),
                NULL);

            if (!status)
                return false;
        }

        if (SourceCodeWriteOrNot)
        {
            status = WriteProcessMemory(
                hGame,
                (PUCHAR)pRemoteBuffer + (InjuctCodeWriteOrNot ? InjuctCode.size() : 0),
                byteVectorToString(SourceCode).c_str(),
                SourceCode.size(),
                NULL);

            if (!status)
                return false;
        }

        *(PDWORD)(JmpCode + 1) = (DWORD)lpTargetInjuctAddress + SourceCode.size();

        status = WriteProcessMemory(hGame,
            (PUCHAR)pRemoteBuffer + (InjuctCodeWriteOrNot ? InjuctCode.size() : 0) + (SourceCodeWriteOrNot ? SourceCode.size() : 0),
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
        shellcodelength - Size of shellcode.

    ReturnValue:

        boolean - Status of operation.

*/
    boolean ModuleContext::RemoteThreadExecuteShellCode(std::vector<byte> shellcode)
    {
        boolean status = TRUE;
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
            byteVectorToString(shellcode).c_str(),
            shellcode.size(),
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


    ModuleContext::ModuleContext(std::wstring ModuleName, uint32_t pid, HANDLE hGame) : GamePid(pid), hGame(hGame)
    {
        lpModuleAddress = GetModuleBaseAddress(ModuleName);
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
    boolean GameContext::elevateprivileges()
    {

        BOOLEAN status = TRUE;

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

        szGaName - Supplies the process that you want to get PID full name wide string .

    Return Value:

        Return target process ID that you want to get to if all routines implement successfully.

*/
    uint32_t GameContext::GetProcessPID(std::wstring GameName)
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
            if (wcscmp(lpInfo->szExeFile, GameName.c_str()) == 0)
            {
                CloseHandle(hProcessSnap);

                return lpInfo->th32ProcessID;
            }
        } while (Process32Next(hProcessSnap, lpInfo));

        return status;
    }

    boolean GameContext::AddModule(std::wstring ModuleName, HANDLE hGame)
    {
        std::unique_ptr<ModuleContext> module = std::make_unique<ModuleContext>(ModuleName, GamePid, hGame);
        modules[ModuleName] = std::move(module);
        return true;
    }

    GameContext::GameContext(std::wstring GameName)
    {
        GamePid = GetProcessPID(GameName);
        if (!GamePid)
        {
            DebugPrint(__FUNCTIONW__, L"Get process pid failed!\n");
        }
        hGame = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GamePid);
        AddModule(GameName, hGame);
        lpGameBaseAddress = modules[GameName]->lpModuleAddress;
    }

    GameContext::~GameContext()
    {
        for (auto& module : modules)
        {
            module.second.reset();
        }
    }
}