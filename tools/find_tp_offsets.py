#!/usr/bin/env python3
import sys
import struct
import pefile

def find_tp_pool_patterns(ntdll_path):
    pe = pefile.PE(ntdll_path)
    data = pe.get_data()
    patterns = [
        (b'\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xF9\x33\xF6', "TP_POOL::Initialize"),
        (b'\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xE9', "TP_WORK::Submit"),
        (b'\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\x48\x8B\x40\x00', "TP_POOL global"),
    ]
    results = {}
    for pattern, name in patterns:
        idx = data.find(pattern)
        if idx != -1:
            rva = pe.get_rva_from_offset(idx)
            results[name] = hex(rva)
    return results

def dump_tp_structs(ntdll_path):
    pe = pefile.PE(ntdll_path)
    print(f"[*] Analyzing: {ntdll_path}")
    print(f"[*] ImageBase: {hex(pe.OPTIONAL_HEADER.ImageBase)}")
    print(f"[*] NumberOfSections: {pe.FILE_HEADER.NumberOfSections}")
    patterns = find_tp_pool_patterns(ntdll_path)
    print("
[+] Found patterns:")
    for name, rva in patterns.items():
        print(f"    {name}: {rva}")
    print("
[!] NOTE: These are heuristic patterns. Verify with WinDbg/IDA Pro.")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <path to ntdll.dll>")
        print(f"Example: {sys.argv[0]} C:\\Windows\\System32\\ntdll.dll")
        sys.exit(1)
    dump_tp_structs(sys.argv[1])
