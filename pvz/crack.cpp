#include "pch.h"
#include <Windows.h>
#include <TlHelp32.h>
#include "common.h"


/*

	Routine Description:

		This Routine init CrackRE32 class static variable.

	Arguments:

		szGameName - Supplies the game full name string that you want to modify.

	Return Value:

		Return true if all routines implement.

*/
_Use_decl_annotations_ BOOLEAN CrackRE32::InitGameInformation(
	_In_ LPWSTR szGameName
)
{
	BOOLEAN status = TRUE;

	this->stGameInfo->dwpid = GetProcessPID(szGameName);

	this->stGameInfo->hGame = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->stGameInfo->dwpid);

	this->stGameInfo->lpBase = GetModuleBaseAddress(szGameName);

	if (!this->stGameInfo->lpBase)
		return FALSE;

	return status;
}


/*

	Routine Description:

		This Routine get process ID by process name.

	Arguments:

		szGaName - Supplies the process that you want to get PID full name wide string .

	Return Value:

		Return target process ID that you want to get to if all routines implement successfully.

*/
_Use_decl_annotations_ DWORD CrackRE32::GetProcessPID(
	_In_ LPWSTR szGaName
)
{
	DWORD status = TRUE;

	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	PROCESSENTRY32* lpInfo = new PROCESSENTRY32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	lpInfo->dwSize = sizeof(PROCESSENTRY32);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		CString szEditString;

		//AfxMessageBox(L"Get process snap shot failed!\n");

		g_dialog->GetDlgItem(IDC_EDIT2)->SetWindowTextW(szEditString);

		return NULL;
	}

	if (!Process32First(hProcessSnap, lpInfo))
	{
		//AfxMessageBox(L"Get process first snap shot failed!\n");

		return NULL;
	}

	do
	{
		if (wcscmp(lpInfo->szExeFile, szGaName) == 0)
		{
			CloseHandle(hProcessSnap);

			return lpInfo->th32ProcessID;
		}
	} while (Process32Next(hProcessSnap, lpInfo));

	return status;
}


/*
	Routine Description:

		This routine get module base address by module full name.

	Arguments:

		szModuleName - Supplies the module Wide character string of the whole process command line that you want get base address.

	 ReturnValue:

		PVOID - Module base address that you Supplies.

*/
_Use_decl_annotations_ PVOID CrackRE32::GetModuleBaseAddress(
	_In_ LPWSTR szModuleName
)
{
	HANDLE	hModuleSnap = INVALID_HANDLE_VALUE;

	MODULEENTRY32* lpMe32 = new MODULEENTRY32;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->stGameInfo->dwpid);

	lpMe32->dwSize = sizeof(MODULEENTRY32);

	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(L"Get module snap error!\n");

		return NULL;
	}

	if (!Module32First(hModuleSnap, lpMe32))
	{

		//AfxMessageBox(L"Error module32first snap!\n");

		CloseHandle(hModuleSnap);

		return NULL;
	}

	do
	{
		if (wcscmp(lpMe32->szModule, szModuleName) == 0)
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

		Evevate process privileges before modifying other process.

	Arguments:

		None

	ReturnValue:

		NTSTATUS - Status of operation

*/
NTSTATUS CrackRE32::elevateprivileges()
{

	BOOLEAN status = TRUE;

	HANDLE	hToken = NULL;

	LUID	luid = { 0 };

	TOKEN_PRIVILEGES	tp = { 0 };

	status = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);

	if (!status)
		return NTSTATUS::ProcessOperationError;

	status = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
	if (!status)
		return NTSTATUS::ProcessOperationError;

	if (!status)
	{
		CloseHandle(hToken);
		return NTSTATUS::ProcessOperationError;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	status = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	if (!status)
	{

		//AfxMessageBox(L"Elevate privilege failed!\n");

		return NTSTATUS::ProcessOperationError;
	}

	CloseHandle(hToken);

	return NTSTATUS::ProcessOperationError;
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
_Use_decl_annotations_ NTSTATUS CrackRE32::ModifyLayerMemory32(
	_In_ PVOID lpBase,
	_In_ ULONG num,
	_In_ PULONG offset,
	_In_ ULONG offsetMaxIndex
)
{
	BOOLEAN status = FALSE;

	PUCHAR pTemp = (PUCHAR)lpBase;

	for (ULONG i = 0; i < offsetMaxIndex; i++)
	{
		pTemp = (PUCHAR)pTemp + (ULONG)(offset[i]);

		ReadProcessMemory(this->stGameInfo->hGame, (PVOID)pTemp, &pTemp, sizeof(DWORD32), NULL);
	}

	if (!pTemp)
		return NTSTATUS::ReadProcessMemoryError;

	pTemp = (PUCHAR)pTemp + (ULONG)(offset[offsetMaxIndex]);

	status = WriteProcessMemory(this->stGameInfo->hGame, (PVOID)pTemp, &num, sizeof(DWORD32), NULL);

	if (!status)
		return NTSTATUS::WriteProcessMemoryError;

	return NTSTATUS::NoneError;
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
_Use_decl_annotations_ NTSTATUS CrackRE32::ReadLayerMemory32(
	_In_ PVOID lpBase,
	_Out_ PULONG pOutputNum,
	_In_ PULONG offset,
	_In_ ULONG offsetMaxIndex
)
{
	BOOLEAN status = FALSE;

	PUCHAR pTemp = (PUCHAR)lpBase;

	for (ULONG i = 0; i < offsetMaxIndex + 1; i++)
	{
		pTemp = (PUCHAR)pTemp + (ULONG)(offset[i]);

		ReadProcessMemory(this->stGameInfo->hGame, (PVOID)pTemp, &pTemp, sizeof(DWORD32), NULL);
	}

	*pOutputNum = (ULONG)pTemp;

	if (!pTemp)
		return NTSTATUS::ReadProcessMemoryError;

	return NTSTATUS::NoneError;
}

/*

	Routine Description:

		This is routine injuct shellcode.

	Arguments:

		lpModifiedAddress - Address that will be injuct shellcode.
		injuctcode - Data address of shellcode.
		injuctcodelength - Sizeof shellcode that will be injuct.
		injuctcodeWriteOrNot - Whether the shellcode is written in the head of the application address applied to the target process.
		sourcecode - Data address of sourcecode that will be overwrite.
		sourcecodelength - Sizeof sourcecode that will be write.
		sourcecodeWriteOrNot - Whether the sourcecode is written in the after shellcode. 

	ReturnValue:

		NTSTATUS - Status of operation.

*/
_Use_decl_annotations_ NTSTATUS CrackRE32::AssemblyInject32(
	_In_ PVOID lpModifiedAddress,
	_In_ BYTE* injuctcode,
	_In_ ULONG injuctcodelength,
	_In_ BOOLEAN injuctcodeWriteOrNot,
	_In_ BYTE* sourcecode,
	_In_ ULONG sourcecodelength,
	_In_ BOOLEAN sourcecodeWriteOrNot
)
{
	BOOLEAN status = TRUE;

	PUCHAR pRemoteBuffer = NULL;

	BYTE JmpCode[] = "\x68\x44\x33\x22\x11\xC3";

	if (sourcecodelength < 0x100)
		pRemoteBuffer = (PUCHAR)VirtualAllocEx(this->stGameInfo->hGame,
			NULL,
			0x100,
			MEM_COMMIT,
			PAGE_EXECUTE_READWRITE
		);
	else
		pRemoteBuffer = (PUCHAR)VirtualAllocEx(this->stGameInfo->hGame,
			NULL,
			0x1000,
			MEM_COMMIT,
			PAGE_EXECUTE_READWRITE
		);

	if (!pRemoteBuffer)
		return NTSTATUS::ReadProcessMemoryError;

	if (injuctcodeWriteOrNot)
	{
		status = WriteProcessMemory(this->stGameInfo->hGame,
			(PUCHAR)pRemoteBuffer,
			injuctcode,
			injuctcodelength,
			NULL
		);

		if (!status)
			return NTSTATUS::WriteProcessMemoryError;
	}

	if (sourcecodeWriteOrNot)
	{
		status = WriteProcessMemory(this->stGameInfo->hGame,
			(PUCHAR)pRemoteBuffer + (injuctcodeWriteOrNot ? injuctcodelength : 0),
			sourcecode,
			sourcecodelength,
			NULL
		);

		if (!status)
			return NTSTATUS::WriteProcessMemoryError;
	}

	*(PDWORD)(JmpCode + 1) = (DWORD)lpModifiedAddress + sourcecodelength;

	status = WriteProcessMemory(this->stGameInfo->hGame,
		(PUCHAR)pRemoteBuffer + (injuctcodeWriteOrNot ? injuctcodelength : 0) + (sourcecodeWriteOrNot ? sourcecodelength : 0),
		JmpCode,
		sizeof(JmpCode) - 1,
		NULL
	);

	if (!status)
		return NTSTATUS::WriteProcessMemoryError;

	*(PDWORD)(JmpCode + 1) = (DWORD)pRemoteBuffer;

	status = WriteProcessMemory(this->stGameInfo->hGame,
		lpModifiedAddress,
		JmpCode,
		sizeof(JmpCode) - 1,
		NULL
	);

	if (!status)
		return NTSTATUS::WriteProcessMemoryError;

	return NTSTATUS::NoneError;
}


_Use_decl_annotations_ CrackRE32::CrackRE32(
	_In_ LPWSTR szGameStr
)
{
	this->stGameInfo = new(GAME_INFORMATION);

	this->stGameInfo->szGameName = szGameStr;

	elevateprivileges();

	if (!InitGameInformation(szGameStr))
		AfxMessageBox(L"Init game information failed!\n");
}


CrackRE32::~CrackRE32()
{
}


/*

	Routine Description:

		This is routine start up remote thread in target process.

	Arguments:

		shellcode - The shellocde that will execute in remote thread.
		shellcodelength - Size of shellcode.

	ReturnValue:

		NTSTATUS - Status of operation.

*/
_Use_decl_annotations_ NTSTATUS CrackRE32::RemoteThreadExecuteShellCode(
	BYTE* shellcode,
	ULONG shellcodelength
)
{
	BOOLEAN status = TRUE;

	PUCHAR pRemoteBuffer = NULL, pRemoteThreadParam = NULL;

	BYTE initparam[0x100] = { 0 };

	pRemoteThreadParam = (PUCHAR)VirtualAllocEx(g_crack->stGameInfo->hGame,
		NULL,
		0x100,
		MEM_COMMIT,
		PAGE_EXECUTE_READWRITE
	);

	if (!pRemoteThreadParam)
		return NTSTATUS::AllocateRemoteVirtualMemoryError;

	status = WriteProcessMemory(g_crack->stGameInfo->hGame,
		(PVOID)pRemoteThreadParam,
		initparam,
		0x100,
		NULL
	);

	if (!status)
		return NTSTATUS::WriteProcessMemoryError;

	pRemoteBuffer = (PUCHAR)VirtualAllocEx(g_crack->stGameInfo->hGame,
		NULL,
		0x100,
		MEM_COMMIT,
		PAGE_EXECUTE_READWRITE
	);

	if (!pRemoteBuffer)
		return NTSTATUS::AllocateRemoteVirtualMemoryError;

	status = WriteProcessMemory(g_crack->stGameInfo->hGame,
		(PVOID)pRemoteBuffer,
		shellcode,
		shellcodelength,
		NULL
	);

	if (!status)
		return NTSTATUS::WriteProcessMemoryError;

	HANDLE hRemoteThread = CreateRemoteThread(g_crack->stGameInfo->hGame,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)pRemoteBuffer,
		pRemoteThreadParam,
		0,
		NULL
	);

	if (hRemoteThread == INVALID_HANDLE_VALUE)
		return NTSTATUS::CreateRetmoteThreadError;

	return NTSTATUS::NoneError;
}
