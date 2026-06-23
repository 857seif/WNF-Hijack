#Requires -RunAsAdministrator

function Get-WnfStateNames {
    $ntdll = [System.IO.File]::ReadAllBytes("$env:SystemRoot\System32\ntdll.dll")
    $pattern = [byte[]]@(0x4C, 0x8D, 0x05)
    $results = @()
    for ($i = 0; $i -lt $ntdll.Length - 3; $i++) {
        if ($ntdll[$i] -eq 0x4C -and $ntdll[$i+1] -eq 0x8D -and $ntdll[$i+2] -eq 0x05) {
            $results += $i
        }
    }
    Write-Host "[+] Found $($results.Count) potential WNF references in ntdll.dll"
    return $results
}

function Dump-WnfStructures {
    Write-Host "[*] Dumping WNF structures from kernel..."
    Write-Host "[!] Requires WinDbg or kernel debugger"
    $commands = @(
        "x /D ntoskrnl!*Wnf*",
        "dt ntoskrnl!_WNF_NAME_DESCRIPTOR",
        "dt ntoskrnl!_WNF_SUBSCRIPTION",
        "!wnf.state"
    )
    foreach ($cmd in $commands) {
        Write-Host "    > $cmd"
    }
}

Write-Host "=== WNF Reverse Engineering Helper ==="
Get-WnfStateNames
Dump-WnfStructures
