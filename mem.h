#pragma once
#include <Windows.h>

namespace Mem
{
	void patchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void nopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
}
