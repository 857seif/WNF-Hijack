#pragma once
#include <windows.h>

class ProcessUtils {
public:
    static DWORD FindProcessByName(const wchar_t* name);
    static BOOL IsProtectedProcess(DWORD pid);
};
