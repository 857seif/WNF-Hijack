#pragma once
#include <windows.h>

typedef struct _WNF_STATE_NAME {
    ULONG Data[2];
} WNF_STATE_NAME, *PWNF_STATE_NAME;

typedef struct _WNF_TYPE_ID {
    GUID TypeId;
} WNF_TYPE_ID, *PWNF_TYPE_ID;

typedef struct _TP_WORK {
    PVOID VTable;
    PVOID Callback;
    PVOID Context;
    PVOID CancelCallback;
    PVOID WorkUnion;
    PVOID Pool;
    PVOID Reserved[2];
    LONG CancelPending;
    PVOID CleanupGroup;
    PVOID CleanupGroupCancelCallback;
    PVOID FinalizationCallback;
    PVOID RaceDll;
    PVOID CallbackDll;
    ULONG Flags;
} TP_WORK, *PTP_WORK;

typedef NTSTATUS (NTAPI *NtUpdateWnfStateData_t)(
    PWNF_STATE_NAME StateName,
    PVOID Buffer,
    ULONG Length,
    PWNF_TYPE_ID TypeId,
    PVOID ExplicitScope,
    ULONG MatchingChangeStamp,
    ULONG CheckStamp
);

typedef NTSTATUS (NTAPI *NtQueryWnfStateData_t)(
    PWNF_STATE_NAME StateName,
    PWNF_TYPE_ID TypeId,
    PVOID ExplicitScope,
    ULONG *ChangeStamp,
    PVOID Buffer,
    ULONG *BufferSize
);

typedef NTSTATUS (NTAPI *NtSubscribeWnfStateChangeNotification_t)(
    PVOID Subscription,
    PWNF_STATE_NAME StateName,
    ULONG ChangeStamp,
    PVOID Callback,
    PVOID CallbackContext,
    PVOID TypeId,
    PVOID SerializationGroup,
    ULONG Unknown
);

typedef PVOID (NTAPI *RtlCreateThreadPoolWork_t)(
    PVOID Pool,
    PVOID Callback,
    PVOID Context,
    PVOID CleanupGroup,
    PVOID CleanupGroupCancelCallback,
    PVOID FinalizationCallback,
    PVOID CallbackDll,
    ULONG Flags
);

typedef NTSTATUS (NTAPI *RtlSubmitThreadPoolWork_t)(
    PVOID Work
);

extern NtUpdateWnfStateData_t g_NtUpdateWnfStateData;
extern NtQueryWnfStateData_t g_NtQueryWnfStateData;
extern NtSubscribeWnfStateChangeNotification_t g_NtSubscribeWnfStateChangeNotification;

BOOL ResolveUndocumentedAPIs();
PVOID GetProcAddressUndoc(HMODULE hModule, const char* funcName);
