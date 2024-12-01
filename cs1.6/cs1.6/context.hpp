#pragma once
#include <afx.h>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <memoryapi.h>
#include <rpcndr.h>
#include <string>
#include <TlHelp32.h>
#include <vector>
#include <Windows.h>

namespace game {

    class ModuleContext
    {
    public:
        uint32_t GamePid;
        HANDLE hGame;
        void* lpModuleAddress;
        ModuleContext(std::wstring ModuleName, uint32_t pid, HANDLE hGame);
        ~ModuleContext();
        void* GetModuleBaseAddress(std::wstring szModuleName);

        /*

            Routine Description:
                This routine write data to multi-level offset pointers

            Arguments:
                num - Data that will be written in.
                layerOffset - The offset array address of each level pointer relative to the base address of the module.
                targetProcBitWidth - Target Process register bit width. x86(Win32) is 32bit,x86_64 is 64 bit.

            ReturnValue:
                bool - Status of operation.

        */
        template <typename T>
        bool ModifyMultiLevelLayerMemory(
            T num, std::vector<uint32_t> layerOffset, uint32_t targetProcBitWidth = 32
        )
        {
            bool status = FALSE;

            char* pTemp = (char*)lpModuleAddress;

            for (uint32_t i = 0; i < layerOffset.size() - 1; i++)
            {
                pTemp = (char*)pTemp + (uint32_t)(layerOffset[i]);

                ReadProcessMemory(hGame, (void*)pTemp, &pTemp, targetProcBitWidth / 8, NULL);
            }

            if (!pTemp)
                return false;

            pTemp = (char*)pTemp + (uint32_t)(layerOffset.back());

            status = WriteProcessMemory(hGame, (T*)pTemp, &num, sizeof(T), NULL);

            if (!status)
                return false;

            return true;
        }

        /*

        Routine Description:
            This is routine read data from multi-level offset pointers

        Arguments:
            pOutputNum - Data stored after reading.
            layerOffset - The offset array address of each level pointer relative to the base address of the module.
            targetProcBitWidth -  Target Process register bit width. x86(Win32) is 32bit,x86_64 is 64 bit.

        ReturnValue:
            bool - Status of operation.

        */

        template <typename T>
        bool ReadMultiLevelMemory(T* pOutputNum, std::vector<uint32_t> layerOffset, uint32_t targetProcBitWidth = 32)
        {
            char* pTemp = (char*)lpModuleAddress;

            for (ULONG i = 0; i < layerOffset.size(); i++)
            {
                pTemp = (char*)pTemp + (uint32_t)(layerOffset[i]);
                ReadProcessMemory(hGame, (void*)pTemp, &pTemp, targetProcBitWidth / 8, NULL);
            }

            *pOutputNum = *(T*)&pTemp;

            if (!pTemp)
                return false;
            return true;
        }

        bool AssemblyInject32(
            void* lpTargetInjuctAddress,
            std::vector<byte> InjuctCode,
            bool InjuctCodeWriteOrNot,
            std::vector<byte> SourceCode,
            bool SourceCodeWriteOrNot
        );

        bool RemoteThreadExecuteShellCode(std::vector<byte> shellcode);

    private:
    };

    class GameContext
    {
    public:
        uint32_t gamePid;
        HANDLE hGame;
        void* lpGameBaseAddress;
        std::wstring GameName;
        std::map<std::wstring, std::unique_ptr<ModuleContext>> modules;
        GameContext(std::wstring GameName);
        ~GameContext();
        bool elevateprivileges();
        uint32_t GetProcessPID(std::wstring szGaName);
        bool AddModule(std::wstring ModuleName, HANDLE hGame);

    private:

    };

}
