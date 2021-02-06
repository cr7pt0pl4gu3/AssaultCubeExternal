#include "proc.h"

// Function to get process id by knowing process name, if returns 0 -> process not found
DWORD getProcId(const wchar_t* procName)
{
	DWORD procId {};
	HANDLE hSnap {CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};

	// Iterating over each process, comparing its name to procName, if successfull -> return that process id
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry {};
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (_wcsicmp(procEntry.szExeFile, procName) == 0)
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

// Function to get module base address by knowing module name and target process id, if returns 0 -> module not found
uintptr_t getModuleBaseAddress(const DWORD& procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr {};
	HANDLE hSnap {CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId)};

	// Iterating over each module, comparing its name to modName, if successfull -> return that module base address
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry {};
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (_wcsicmp(modEntry.szModule, modName) == 0)
				{
					modBaseAddr = reinterpret_cast<uintptr_t>(modEntry.modBaseAddr);
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t findDynamicMemoryAddress(HANDLE& hProc, const uintptr_t& ptr, const std::vector<unsigned int>& offsets)
{
	uintptr_t addr {ptr};
	for (unsigned int i {}; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, reinterpret_cast<BYTE*>(addr), &addr, sizeof(addr), nullptr);
		addr += offsets[i];
	}
	return addr;
}
