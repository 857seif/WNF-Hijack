#pragma once
#include <windows.h>

typedef struct _WNF_NAME_DESCRIPTOR {
    WNF_STATE_NAME StateName;
    ULONG MaximumStateSize;
    ULONG ChangeStamp;
    ULONG Data[4];
} WNF_NAME_DESCRIPTOR, *PWNF_NAME_DESCRIPTOR;

typedef struct _WNF_SUBSCRIPTION {
    LIST_ENTRY ListEntry;
    WNF_STATE_NAME StateName;
    PVOID Callback;
    PVOID Context;
    ULONG ChangeStamp;
    ULONG Flags;
} WNF_SUBSCRIPTION, *PWNF_SUBSCRIPTION;

typedef struct _TP_POOL {
    PVOID Lock;
    PVOID WorkerFactory;
    PVOID CompletionPort;
    ULONG PoolFlags;
    ULONG ThreadMinimum;
    ULONG ThreadMaximum;
    ULONG TaskCount;
    LIST_ENTRY WorkQueue;
    LIST_ENTRY TimerQueue;
    PVOID CleanupGroup;
    PVOID CallbackEnvironment;
} TP_POOL, *PTP_POOL;

typedef struct _TP_CALLBACK_ENVIRON_V3 {
    ULONG Version;
    PTP_POOL Pool;
    PVOID CleanupGroup;
    PVOID CleanupGroupCancelCallback;
    PVOID RaceDll;
    PVOID ActivationContext;
    PVOID FinalizationCallback;
    union {
        ULONG Flags;
        struct {
            ULONG LongFunction:1;
            ULONG Persistent:1;
            ULONG Private:30;
        } s;
    } u;
} TP_CALLBACK_ENVIRON_V3, *PTP_CALLBACK_ENVIRON_V3;
