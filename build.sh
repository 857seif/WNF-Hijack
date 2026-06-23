#!/bin/bash
set -e

echo "========================================"
echo "  WNF Hijack Framework - Build Script"
echo "========================================"
echo

if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "[-] MinGW not found. Install: sudo apt-get install mingw-w64"
    exit 1
fi

mkdir -p build-mingw

SOURCES=$(find src -name "*.cpp" | sort)
echo "[*] Sources:"
echo "$SOURCES" | sed 's/^/    /'
echo

echo "[*] Building..."
x86_64-w64-mingw32-g++ -static -O2 -std=c++17 -m64 -Wall -Wextra -Iinclude -Isrc $SOURCES -o build-mingw/wnf_hijack.exe

if [ -f "build-mingw/wnf_hijack.exe" ]; then
    echo "[+] Build successful: build-mingw/wnf_hijack.exe"
    ls -lh build-mingw/wnf_hijack.exe
else
    echo "[-] Build failed"
    exit 1
fi
