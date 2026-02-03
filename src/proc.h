#pragma once

#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>

DWORD GetProcId(std::u16string processName);
uintptr_t GetModuleBaseAddress(DWORD procId, std::u16string targetModName);
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
