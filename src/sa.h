#pragma once
#include <windows.h>

unsigned char* GetPayload(SIZE_T* outSize, BOOL fallbackNotepad);
VOID ExecuteLocalPayload();
VOID SpawnFallbackProcess();
