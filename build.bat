@echo off
chcp 65001 >nul
cls

echo ========================================
echo  WNF Hijack Framework - Build Script
echo ========================================
echo.

where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [-] CMake not found. Install from https://cmake.org/download/
    pause
    exit /b 1
)

if not exist "build" mkdir build
cd build

echo [*] Generating build files...
cmake .. -A x64
if %errorlevel% neq 0 (
    echo [-] CMake configuration failed
    pause
    exit /b 1
)

echo [*] Building...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo [-] Build failed
    pause
    exit /b 1
)

echo.
echo [+] Build successful!
echo [*] Output: build\Release\wnf_hijack.exe
echo.
pause
