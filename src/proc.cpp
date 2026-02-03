#include "proc.h"

DWORD GetProcId(std::u16string processName)
{
        HANDLE hProcessSnap;
        HANDLE hProcess;
        PROCESSENTRY32W pe32;
        DWORD dwPriorityClass;
        DWORD pid = 0;

        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hProcessSnap != INVALID_HANDLE_VALUE)
        {
                pe32.dwSize = sizeof(pe32);

                if(Process32FirstW(hProcessSnap, &pe32))
                {
                        do
                        {
                                std::u16string exeName(reinterpret_cast<char16_t*>(pe32.szExeFile)); //sz prefix - string, zero-terminated (null-terminated)
                                if(exeName == processName)
                                {
                                        pid = pe32.th32ProcessID;

                                        break;
                                }
                        } while (Process32NextW(hProcessSnap, &pe32));
                }
        }

        CloseHandle(hProcessSnap);

        return pid;
}

uintptr_t GetModuleBaseAddress(DWORD procId, std::u16string targetModName)
{
        uintptr_t modBaseAddr = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

        if (hSnap != INVALID_HANDLE_VALUE)
        {
                MODULEENTRY32W modEntry;

                modEntry.dwSize = sizeof(modEntry);

                if(Module32FirstW(hSnap, &modEntry))
                {
                        do
                        {
                                std::u16string modName(reinterpret_cast<char16_t*>(modEntry.szModule));
                                if(modName == targetModName)
                                {
                                        modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                                        break;
                                }
                        } while (Module32NextW(hSnap, &modEntry));
                }
        }

        CloseHandle(hSnap);
        return modBaseAddr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{

        uintptr_t addr = ptr;
        for (unsigned int i = 0; i < offsets.size(); ++i)
        {
                ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
                addr += offsets[i];
        }
        return addr;
}
