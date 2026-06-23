#!/usr/bin/env python3
"""
WNF Hijack Framework - Build Script (Python)
Works on GitHub Codespaces (Linux) to build Windows .exe

Usage:
    python build.py
    python build.py --clean
"""

import os
import sys
import subprocess
import shutil
import argparse

def run(cmd, cwd=None):
    """Run a shell command and return output."""
    print(f"[*] Running: {cmd}")
    result = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True)
    if result.stdout:
        print(result.stdout)
    if result.stderr and result.returncode != 0:
        print(f"[-] {result.stderr}")
    return result

def install_mingw():
    """Install MinGW if not present."""
    if shutil.which("x86_64-w64-mingw32-g++"):
        print("[+] MinGW already installed")
        return True

    print("[*] MinGW not found. Attempting to install...")

    result = run("apt-get update -qq")
    if result.returncode == 0:
        result = run("apt-get install -y -qq mingw-w64 g++-mingw-w64-x86-64 gcc-mingw-w64-x86-64")
        if result.returncode == 0:
            print("[+] MinGW installed successfully")
            return True

    print("[*] Trying with sudo...")
    result = run("sudo apt-get update -qq")
    if result.returncode == 0:
        result = run("sudo apt-get install -y -qq mingw-w64 g++-mingw-w64-x86-64 gcc-mingw-w64-x86-64")
        if result.returncode == 0:
            print("[+] MinGW installed with sudo")
            return True

    print("[-] Failed to install MinGW")
    return False

def build():
    """Build the Windows .exe."""
    base = os.path.dirname(os.path.abspath(__file__))

    # Detect if we are inside the framework directory or at the parent
    # If the current directory contains src/ and include/, we are in the framework root.
    if os.path.isdir(os.path.join(base, "src")) and os.path.isdir(os.path.join(base, "include")):
        framework_dir = base
    else:
        # Otherwise assume we have a subdirectory named 'wnf-hijack-framework'
        framework_dir = os.path.join(base, "wnf-hijack-framework")
        if not os.path.isdir(framework_dir):
            print(f"[-] Framework directory not found at {framework_dir}")
            return False

    build_dir = os.path.join(framework_dir, "build-mingw")
    os.makedirs(build_dir, exist_ok=True)

    # Collect source files using os.walk
    sources = []
    src_dir = os.path.join(framework_dir, "src")
    for root, dirs, files in os.walk(src_dir):
        for f in files:
            if f.endswith(".cpp"):
                sources.append(os.path.join(root, f))

    if not sources:
        print("[-] No .cpp files found in src/")
        return False

    print(f"[*] Found {len(sources)} source files:")
    for s in sources:
        print(f"    {os.path.relpath(s, base)}")

    includes = [
        f"-I{framework_dir}/include",
        f"-I{framework_dir}/src"
    ]
    flags = ["-static", "-O2", "-std=c++17", "-m64", "-Wall", "-Wextra"]
    output = os.path.join(build_dir, "wnf_hijack.exe")

    source_paths = " ".join(f'"{s}"' for s in sources)
    flags_str = " ".join(flags)
    includes_str = " ".join(includes)
    cmd = f'x86_64-w64-mingw32-g++ {flags_str} {includes_str} {source_paths} -o "{output}"'

    print("")
    print("[*] Building...")
    result = run(cmd)

    if result.returncode == 0 and os.path.exists(output):
        print(f"")
        print(f"[+] Build successful: {output}")
        size = os.path.getsize(output)
        print(f"[+] Size: {size / 1024:.1f} KB")
        file_result = run(f'file "{output}"')
        return True
    else:
        print("[-] Build failed")
        return False

def clean():
    """Remove build directories."""
    base = os.path.dirname(os.path.abspath(__file__))
    # Try to find framework directory
    if os.path.isdir(os.path.join(base, "src")):
        framework_dir = base
    else:
        framework_dir = os.path.join(base, "wnf-hijack-framework")
        if not os.path.isdir(framework_dir):
            print(f"[-] Framework directory not found at {framework_dir}")
            return
    for d in ["build", "build-mingw", "build-linux"]:
        path = os.path.join(framework_dir, d)
        if os.path.exists(path):
            shutil.rmtree(path)
            print(f"[+] Removed: {path}")

def main():
    parser = argparse.ArgumentParser(description="Build WNF Hijack Framework")
    parser.add_argument("--clean", action="store_true", help="Clean build directories")
    args = parser.parse_args()

    if args.clean:
        clean()
        return

    print("=" * 50)
    print("  WNF Hijack Framework - Build")
    print("=" * 50)
    print("")

    if not install_mingw():
        print("")
        print("[!] MinGW not available.")
        print("[*] Options:")
        print("    1. Use GitHub Actions (recommended):")
        print("       - Push to main branch")
        print("       - Go to Actions tab")
        print("       - Download artifact")
        print("    2. Install manually: sudo apt-get install -y mingw-w64")
        print("    3. Use .devcontainer/devcontainer.json to auto-install")
        sys.exit(1)

    print("")
    if build():
        print("")
        print("[+] Done!")
        print("[*] To test: wine build-mingw/wnf_hijack.exe")
    else:
        sys.exit(1)

if __name__ == "__main__":
    main()
