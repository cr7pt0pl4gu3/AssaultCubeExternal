#include <iostream>
#include "proc.h"

int main()
{
	// Initialize module name
	const wchar_t* moduleName {L"ac_client.exe"};
	
    // Get process id of the target process
	const DWORD procId {getProcId(moduleName)};

	// Get module base address
	const uintptr_t moduleBase {getModuleBaseAddress(procId, moduleName)};

	// Get handle to process
	HANDLE hProcess {OpenProcess(PROCESS_ALL_ACCESS, false, procId)};
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		std::cout << "Can't open handle to the process!";
		return GetLastError();
	}

	// Resolve base address of the pointer chain
	const uintptr_t dynamicPtrBaseAddress {moduleBase + 0x109B74};

	// Resolve our health address through the pointer chain
	const std::vector<unsigned int> healthOffsets {0xF8};
	const uintptr_t healthAddr {findDynamicMemoryAddress(hProcess, dynamicPtrBaseAddress, healthOffsets)};

	// Read health value
	int healthValue {};
	ReadProcessMemory(hProcess, reinterpret_cast<BYTE*>(healthAddr), &healthValue, sizeof(healthValue), nullptr);
	
	// Write to health value
	int newHealth {1337};
	WriteProcessMemory(hProcess, reinterpret_cast<BYTE*>(healthAddr), &newHealth, sizeof(newHealth), nullptr);
}
