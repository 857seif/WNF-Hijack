#pragma once
#include <windows.h>
#include "wnf_client.h"

class TpHijacker {
public:
    TpHijacker();
    ~TpHijacker();
    BOOL ExecutePayloadInTarget(PVOID payload, SIZE_T size);
    BOOL FindTargetProcess();
    BOOL LocateThreadPool();
    BOOL HijackWorkItem(PVOID payloadAddress);
    BOOL TriggerViaWNF(WnfClient* wnf);

private:
    HANDLE m_targetProcess;
    PVOID m_targetPool;
};
