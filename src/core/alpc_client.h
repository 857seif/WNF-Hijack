#pragma once
#include <windows.h>

class AlpcClient {
public:
    AlpcClient();
    ~AlpcClient();
    BOOL ConnectToPort(const wchar_t* portName);
    BOOL SendMessage(PVOID message, SIZE_T size);
    BOOL ReceiveMessage(PVOID buffer, SIZE_T bufferSize, SIZE_T* received);
    VOID EnumeratePorts();

private:
    HANDLE m_portHandle;
};
