#include <cflf.h>

VARDEF PROCESS process;

VOID InitializeProcess(PPROCESS pProcess) {
    pProcess->hHeap = GetProcessHeap();
    pProcess->hInstance = GetModuleHandleA(NULL);
    if (pProcess->hHeap == NULL || pProcess->hInstance == NULL) {
        FatalLowLevelError(FILE_LINE);
    }
}
