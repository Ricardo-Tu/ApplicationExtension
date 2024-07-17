#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <TlHelp32.h>
#include <memory>
#include <vector>

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

        boolean ModifyMultiLevelLayerMemory32(uint32_t num, std::vector<uint32_t> layerOffset);
        boolean ModifyMultiLevelLayerMemory32float(float num, std::vector<uint32_t> layerOffset);
        boolean ReadMultiLevelMemory32float(float* pOutputNum, std::vector<uint32_t> layerOffset);

        boolean ReadMultiLevelMemory32(uint32_t* pOutputNum, std::vector<uint32_t> layerOffset);

        boolean AssemblyInject32(
            void* lpTargetInjuctAddress,
            std::vector<byte> InjuctCode,
            boolean InjuctCodeWriteOrNot,
            std::vector<byte> SourceCode,
            boolean SourceCodeWriteOrNot
        );

        boolean RemoteThreadExecuteShellCode(std::vector<byte> shellcode);

    private:
    };

    class GameContext
    {
    public:
        uint32_t GamePid;
        HANDLE hGame;
        void* lpGameBaseAddress;
        std::wstring GameName;
        std::map<std::wstring, std::unique_ptr<ModuleContext>> modules;
        GameContext(std::wstring GameName);
        ~GameContext();
        boolean elevateprivileges();
        uint32_t GetProcessPID(std::wstring szGaName);
        boolean AddModule(std::wstring ModuleName, HANDLE hGame);

    private:

    };

}
