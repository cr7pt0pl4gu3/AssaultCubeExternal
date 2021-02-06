#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

DWORD getProcId(const wchar_t* procName);

uintptr_t getModuleBaseAddress(const DWORD& procId, const wchar_t* modName);

uintptr_t findDynamicMemoryAddress(HANDLE& hProc, const uintptr_t& ptr, const std::vector<unsigned int>& offsets);