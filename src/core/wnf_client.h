#pragma once
#include <windows.h>

class WnfClient {
public:
    WnfClient();
    ~WnfClient();
    BOOL Initialize();
    BOOL StorePayload(PVOID payload, SIZE_T size);
    BOOL RetrievePayload(PVOID buffer, SIZE_T bufferSize, SIZE_T* outSize);
    BOOL SubscribeToTrigger(PVOID callback, PVOID context);
    VOID EnumerateStates();

private:
    BOOL m_initialized;
};
