#include "Mem.h"

void Mem::patchEx(BYTE* dst, BYTE* src, const unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}	

void Mem::nopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	BYTE* nopArray {new BYTE[size]};
	memset(nopArray, 0x90, size);
	
	patchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
	nopArray = nullptr;
}
