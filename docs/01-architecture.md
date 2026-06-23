# Architecture Overview

## Components

```
[Payload] -> [WNF State] -> [Target Process] -> [Thread Pool] -> [Execution]
                ^                                    v
         [Kernel Storage]                    [Hijacked Work Item]
```

## Flow

1. WNF Client stores payload in kernel WNF state
2. TP Hijacker locates target process Thread Pool
3. TP Hijacker modifies Work Item callback pointer
4. WNF Trigger causes target to process work item
5. Payload executes in target process context

## Why This Works

- No CreateRemoteThread (monitored)
- No VirtualAllocEx (logged)
- No WriteProcessMemory (detected)
- Uses existing kernel mechanisms
