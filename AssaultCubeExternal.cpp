#include <iostream>
#include "proc.h"
#include "mem.h"

int main()
{	
	// Initialize module name
	const wchar_t* moduleName {L"ac_client.exe"};
	
	// Get process id of the target process
	const DWORD procId {getProcId(moduleName)};

	// If process is running
	if (procId)
	{
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

		// Resolve our health through the pointer chain
		const std::vector<unsigned int> healthOffsets {0xF8};
		const uintptr_t healthAddr {findDynamicMemoryAddress(hProcess, dynamicPtrBaseAddress, healthOffsets)};
		
		// On and off state bools
		bool bHealth {}, bAmmo {}, bRecoil {};

		// Values to sub health and ammo
		int newHealthValue {1337};
		int newAmmoValue {1337};

		// Use trainer until process closes
		DWORD dwExit {};
		while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
		{
			if (GetAsyncKeyState(VK_NUMPAD1) & 1)
			{
				bHealth = !bHealth;
			}
			if (GetAsyncKeyState(VK_NUMPAD2) & 1)
			{
				bAmmo = !bAmmo;
				
				if (bAmmo)
				{
					Mem::patchEx(reinterpret_cast<BYTE*>(moduleBase + 0x637e9), (BYTE*)("\xFF\x06"), 2, hProcess);
				}
				else
				{
					Mem::patchEx(reinterpret_cast<BYTE*>(moduleBase + 0x637e9), (BYTE*)("\xFF\x0E"), 2, hProcess);
				}
			}
			if (GetAsyncKeyState(VK_NUMPAD3) & 1)
			{
				bRecoil = !bRecoil;

				if (bRecoil)
				{
					Mem::nopEx(reinterpret_cast<BYTE*>(moduleBase + 0x63786), 10, hProcess);
				}
				else
				{
					Mem::patchEx(reinterpret_cast<BYTE*>(moduleBase + 0x63786),
						(BYTE*)("\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2"), 10, hProcess);
				}
			}
			if (GetAsyncKeyState(VK_INSERT) & 1)
			{
				return 0;
			}

			if (bHealth)
			{
				Mem::patchEx(reinterpret_cast<BYTE*>(healthAddr), reinterpret_cast<BYTE*>(&newHealthValue), sizeof(newHealthValue), hProcess);
			}

			Sleep(10);
		}
	}
	else
	{
		std::cout << "Process not found, press Enter to exit.\n";
		std::cin.get();
		return 0;
	}

	std::cout << "Process not found, press Enter to exit.\n";
	std::cin.get();
}
