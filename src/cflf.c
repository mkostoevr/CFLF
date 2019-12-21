#include <cflf.h>

#ifdef ONE_SOURCE
#   include "cflfbmp.c"
#   include "cflfbot.c"
#   include "cflfcfg.c"
#   include "cflferr.c"
#   include "cflfproc.c"
#   include "cflfscr.c"
#   include "cflfui.c"
#endif

static VOID MessageLoop() {
    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

VOID Start() {
    InitializeProcess(&process);
    InitializeUserInterface(&userInterface);
    MessageLoop();
    ExitProcess(0);
}

// this part is hidden for makefile by macros cause the fucking golink sucks my dick too
// if main entry will be defined this stupid will say that it can't find "main" symbol
#ifndef MAKEFILE
VOID main() {
    Start();
}
#endif
