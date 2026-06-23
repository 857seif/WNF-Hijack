# WNF Internals

## What is WNF?

Windows Notification Facility is an undocumented kernel pub/sub system.

## Key Structures

```c
typedef struct _WNF_STATE_NAME {
    ULONG Data[2];
} WNF_STATE_NAME;
```

## Key Functions

- NtUpdateWnfStateData - Publish data
- NtQueryWnfStateData - Retrieve data
- NtSubscribeWnfStateChangeNotification - Subscribe to changes

## Reverse Engineering Required

1. Dump WNF names from ntoskrnl.exe
2. Find user-mode accessible states
3. Determine maximum state sizes
4. Map state names to system components
