# 🚀 WNF‑Hijack Framework

```
██╗    ██╗███╗   ██╗███████╗    ██╗  ██╗██╗   ██╗ █████╗  ██████╗██╗  ██╗
██║    ██║████╗  ██║██╔════╝    ██║  ██║╚██╗ ██╔╝██╔══██╗██╔════╝██║ ██╔╝
██║ █╗ ██║██╔██╗ ██║█████╗      ███████║ ╚████╔╝ ███████║██║     █████╔╝ 
██║███╗██║██║╚██╗██║██╔══╝      ██╔══██║  ╚██╔╝  ██╔══██║██║     ██╔═██╗ 
╚███╔███╔╝██║ ╚████║██║         ██║  ██║   ██║   ██║  ██║╚██████╗██║  ██╗
 ╚══╝╚══╝ ╚═╝  ╚═══╝╚═╝         ╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝
```

**A low‑level Windows payload execution framework that abuses WNF & Thread Pools**  
*Bypass traditional EDR/AV monitoring with kernel‑backed triggers and thread reuse.*

---

## 🔥 What Makes This Different?

| Traditional Injection | This Framework |
|----------------------|----------------|
| `CreateRemoteThread` | WNF State Change Trigger |
| `VirtualAllocEx` | Kernel WNF Storage |
| `WriteProcessMemory` | Thread Pool Work Item Hijacking |
| New thread creation | Reuses existing thread pool threads |
| DLL injection | No DLLs loaded |

⚡ **Result:** Significantly lower detection footprint against modern EDR/AV.

---

## 🧠 Architecture

```
[Payload (sa.cpp)] ——> [WNF Kernel State] ——> [Target Process]
                                              |
                                              v
                                    [Thread Pool Work Queue]
                                              |
                                              v
                                    [Hijacked Work Item]
                                              |
                                              v
                                    [Payload Execution]
```

### Execution Flow

1. **Store** — Payload is stored in a WNF state (kernel‑managed pub/sub)
2. **Locate** — Thread Pool structures are located in a target process (e.g. `svchost.exe`)
3. **Hijack** — A pending `TP_WORK` item's callback pointer is modified
4. **Trigger** — WNF state change causes the target to process the hijacked work item
5. **Execute** — Payload runs in the context of a legitimate Windows service

---

## 📁 Project Structure

```
wnf-hijack-framework/
├── src/
│   ├── sa.cpp                  # Payload module (shellcode + fallback)
│   ├── sa.h                    # Payload header
│   ├── main.cpp                # Entry point / orchestrator
│   ├── core/
│   │   ├── wnf_client.cpp/h    # WNF state operations
│   │   ├── tp_hijacker.cpp/h   # Thread Pool hijacking logic
│   │   └── alpc_client.cpp/h   # ALPC alternative approach
│   └── utils/
│       ├── memory.cpp/h        # Memory helpers
│       └── process.cpp/h       # Process enumeration
├── include/
│   └── undoc/
│       ├── ntwrapper.h         # Undocumented NT API declarations
│       └── wnf_structs.h       # WNF/TP structures (RE'd)
├── tools/
│   ├── re_wnf.ps1              # PowerShell WNF analysis helper
│   └── find_tp_offsets.py     # Python Thread Pool offset finder
├── docs/
│   ├── 01-architecture.md
│   ├── 02-wnf-internals.md
│   ├── 03-thread-pool-hijacking.md
│   └── 04-alpc-abuse.md
├── tests/
│   └── test_wnf.cpp
├── .devcontainer/
│   └── devcontainer.json       # Auto‑installs MinGW in Codespaces
├── .github/
│   └── workflows/
│       └── build.yml           # Auto‑builds Windows .exe on push
├── CMakeLists.txt
├── build.py                    # Python build script (Linux → Windows .exe)
├── build.sh                    # Bash build script
├── build.bat                   # Windows build script

```

---

## 🛠️ Building

### Option 1: GitHub Actions (Recommended — No Local Setup)

Push to `main` and Actions automatically builds the Windows `.exe`:

```bash
git add .
git commit -m "Initial commit"
git push origin main
```

Then go to **GitHub → Actions → Build Windows EXE → Download artifact**.


### Option 2: Local Linux (Cross‑Compilation)

```bash
sudo apt-get update && sudo apt-get install -y mingw-w64
cd wnf-hijack-framework
python build.py
```

### Option 3: Windows Native

```cmd
cd wnf-hijack-framework
build.bat
```

Requires: Visual Studio 2022 or CMake 3.16+

---

## 🧩 Customizing the Payload

The payload lives in `src/sa.cpp`. By default it includes:

- `g_payload[]` — calc.exe shellcode (default)
- `g_payload_notepad[]` — notepad.exe shellcode (fallback)

### Converting Your Own EXE to Shellcode

You can embed any Windows executable using **pe_to_shellcode**:

#### Step 1: Download pe_to_shellcode  
Get `pe2shc.exe` and `runshc64.exe` from:  
[https://github.com/hasherezade/pe_to_shellcode](https://github.com/hasherezade/pe_to_shellcode)

#### Step 2: Convert Your EXE
```cmd
pe2shc.exe your.exe
```
Produces: `your.shc.exe` (raw shellcode)

#### Step 3: Test the Shellcode
```cmd
runshc64.exe your.shc.exe
```

#### Step 4: Convert to C Array

**Using xxd (Linux/WSL):**
```bash
xxd -i your.shc.exe > payload.txt
```

**Using Python:**
```python
import sys
with open(sys.argv[1], 'rb') as f:
    data = f.read()
    print('unsigned char g_payload[] = {')
    print(', '.join('0x{:02x}'.format(b) for b in data))
    print('};')
    print('// size =', len(data))
```
Run:
```bash
python bin2c.py your.shc.exe
```

#### Step 5: Embed in sa.cpp  
Replace the `g_payload[]` array in `src/sa.cpp` with the output. Then rebuild:

```bash
python build.py
```

---

## 📚 API Reference

### `WnfClient`
```cpp
class WnfClient {
    BOOL Initialize();
    BOOL StorePayload(PVOID payload, SIZE_T size);
    BOOL RetrievePayload(PVOID buffer, SIZE_T size, SIZE_T* outSize);
    BOOL SubscribeToTrigger(PVOID callback, PVOID context);
    VOID EnumerateStates();
};
```

### `TpHijacker`
```cpp
class TpHijacker {
    BOOL ExecutePayloadInTarget(PVOID payload, SIZE_T size);
    BOOL FindTargetProcess();
    BOOL LocateThreadPool();
    BOOL HijackWorkItem(PVOID payloadAddress);
    BOOL TriggerViaWNF(WnfClient* wnf);
};
```

### Payload Helpers (`sa.cpp`)
```cpp
unsigned char* GetPayload(SIZE_T* outSize, BOOL fallbackNotepad);
VOID ExecuteLocalPayload();
VOID SpawnFallbackProcess();
```

---

## ✅ Implemented vs. Needs RE

| Component | Status | Notes |
|-----------|--------|-------|
| WNF API resolution | ✅ Working | Runtime resolution from `ntdll.dll` |
| Process enumeration | ✅ Working | `tlhelp32` based |
| Memory utilities | ✅ Working | `VirtualAlloc`, pattern scanning |
| Local payload execution | ✅ Working | `ExecuteLocalPayload()` |
| Fallback process spawn | ✅ Working | `SpawnFallbackProcess()` |
| WNF state enumeration | ⚠️ Needs RE | Requires `ntoskrnl.exe` analysis |
| WNF state name discovery | ⚠️ Needs RE | Per‑Windows version |
| Thread Pool offsets | ⚠️ Needs RE | Requires `ntdll.dll` analysis |
| Work item queue manipulation | ⚠️ Needs RE | Undocumented structures |
| Trigger mechanism | ⚠️ Needs RE | WNF subscription internals |

---

## 📖 Documentation

Detailed technical docs are in the `docs/` directory:

| Document | Topic |
|----------|-------|
| [01-architecture.md](https://github.com/857seif/WNF-Hijack/blob/main/docs/01-architecture.md) | High‑level architecture & data flow |
| [02-wnf-internals.md](https://github.com/857seif/WNF-Hijack/blob/main/docs/02-wnf-internals.md) | Windows Notification Facility internals |
| [03-thread-pool-hijacking.md](https://github.com/857seif/WNF-Hijack/blob/main/docs/03-thread-pool-hijacking.md) | Thread Pool hijacking technique |
| [04-alpc-abuse.md](https://github.com/857seif/WNF-Hijack/blob/main/docs/04-alpc-abuse.md) | ALPC port abuse alternative |

---

## 🔧 Research Tools

### `tools/re_wnf.ps1`  
PowerShell script to dump WNF info from `ntdll.dll`:
```powershell
#Requires -RunAsAdministrator
.\tools\re_wnf.ps1
```

### `tools/find_tp_offsets.py`  
Python script to find Thread Pool offsets in `ntdll.dll`:
```bash
python tools/find_tp_offsets.py /path/to/ntdll.dll
```

---

## 📦 Requirements

| Component | Minimum Version |
|-----------|-----------------|
| Windows (target) | Windows 10/11 |
| CMake | 3.16+ |
| MinGW (cross‑compile) | Latest |
| Python | 3.8+ (for build.py) |
| Visual Studio | 2022 (optional, Windows native) |

---

## ⚠️ Disclaimer

This project is **for educational and research purposes only**. The techniques demonstrated are used by advanced malware and should **only** be studied in isolated, authorised environments.

**Do not use on systems you do not own or have explicit permission to test.**

---

## 📄 License

MIT — Educational Use Only

---
