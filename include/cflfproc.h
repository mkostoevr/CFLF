#pragma once

#include <cflf.h>

#include <windows.h>

typedef struct tagPROCESS {
    HANDLE hHeap;
    HINSTANCE hInstance;
} PROCESS, *PPROCESS;

FUNDEC VOID InitializeProcess(PPROCESS pProcess);
