#include "memory.h"
#include <stdio.h>
#include <string.h>

PVOID MemoryUtils::AllocateExecutableMemory(SIZE_T size) {
    PVOID mem = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) printf("[-] VirtualAlloc failed: %lu\n", GetLastError());
    return mem;
}

BOOL MemoryUtils::ProtectMemory(PVOID address, SIZE_T size, DWORD protection) {
    DWORD oldProtect;
    return VirtualProtect(address, size, protection, &oldProtect);
}

VOID MemoryUtils::FreeMemory(PVOID address) {
    VirtualFree(address, 0, MEM_RELEASE);
}

PVOID MemoryUtils::FindPattern(PVOID base, SIZE_T size, const BYTE* pattern, const char* mask) {
    SIZE_T patternLen = strlen(mask);
    for (SIZE_T i = 0; i < size - patternLen; i++) {
        BOOL found = TRUE;
        for (SIZE_T j = 0; j < patternLen; j++) {
            if (mask[j] == 'x' && ((BYTE*)base)[i + j] != pattern[j]) {
                found = FALSE;
                break;
            }
        }
        if (found) return (PVOID)((BYTE*)base + i);
    }
    return nullptr;
}
