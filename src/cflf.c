#include <cflf.h>

#ifdef ONE_SOURCE
#   include "cflfbmp.c"
#   include "cflfbot.c"
#   include "cflfdword.c"
#   include "cflferr.c"
#   include "cflfproc.c"
#   include "cflfscr.c"
#   include "cflfui.c"
#endif

static void MessageLoop() {
    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void Start() {
    InitializeProcess(&process);
    InitializeUserInterface(&userInterface);
    MessageLoop();
    ExitProcess(0);
}

// this part is hidden by macros cause the fucking golink sucks my dick too
// if main entry will be defined this stupid will say that it can't find "main" symbol
// if you wish to compile this without makefile, just define NO_MAKEFILE and specify include path
// gcc *.c -I../include -luser32 -lgdi32 -lkernel32 -DNO_MAKEFILE
#ifdef NO_MAKEFILE
void main() {
    Start();
}
#endif
