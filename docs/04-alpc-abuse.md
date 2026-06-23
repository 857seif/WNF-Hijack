# ALPC Abuse

## What is ALPC?

Advanced Local Procedure Call - the backbone of Windows IPC.

## Attack Vector

Instead of injection, abuse legitimate ALPC communication:

1. Find ALPC port with vulnerable handler
2. Craft malicious message
3. Send via legitimate ALPC connection
4. Handler executes payload in service context

## Advantages

- No memory injection
- Legitimate communication channel
- Hard to distinguish from normal traffic
- Service context execution

## Challenges

- Finding vulnerable ports
- Understanding message formats
- Bypassing port security checks
