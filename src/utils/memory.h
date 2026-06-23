#pragma once
#include <windows.h>

class MemoryUtils {
public:
    static PVOID AllocateExecutableMemory(SIZE_T size);
    static BOOL ProtectMemory(PVOID address, SIZE_T size, DWORD protection);
    static VOID FreeMemory(PVOID address);
    static PVOID FindPattern(PVOID base, SIZE_T size, const BYTE* pattern, const char* mask);
};
