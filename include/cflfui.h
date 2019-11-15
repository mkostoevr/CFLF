#pragma once

#include <cflf.h>

#include <windows.h>

#define UNUSED(x) ((void)(x))

typedef struct tagCONTROL {
    HWND hHandle;
    DWORD dwExStyle;
    LPCSTR lpClassName;
    LPCSTR lpWindowName;
    DWORD dwStyle;
    INT X;
    INT Y;
    INT nWidth;
    INT nHeight;
    HWND hWndParent;
    HMENU hMenu;
    HINSTANCE hInstance;
    LPVOID lpParam;
} CONTROL, *PCONTROL;

typedef struct tagUI {
    CONTROL cMainWindow;
    CONTROL cBotControlButton;
    CONTROL cStatusLabel;
} UI, *PUI;

FUNDEC VOID InitializeUserInterface(PUI pUserInterface);
