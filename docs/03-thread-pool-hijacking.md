# Thread Pool Hijacking

## What is Thread Pool?

Windows Thread Pool is a system-managed thread pool used by services and applications.

## Key Structures

```c
typedef struct _TP_POOL {
    PVOID Lock;
    PVOID WorkerFactory;
    PVOID CompletionPort;
    ULONG PoolFlags;
    LIST_ENTRY WorkQueue;
    LIST_ENTRY TimerQueue;
} TP_POOL;
```

## Attack Steps

1. Find target process with active Thread Pool
2. Locate TP_POOL structure in memory
3. Find pending TP_WORK item
4. Modify Callback pointer
5. Wait for pool to process

## Why Hard to Detect

- No new threads created
- No remote memory allocation
- Uses legitimate system mechanism
- Callback runs in normal pool context
