#include <cflf.h>

VARDEF PROCESS process;

VOID InitializeProcess(PPROCESS pProcess) {
    pProcess->hHeap = GetProcessHeap();
    pProcess->hInstance = GetModuleHandleA(NULL);
    if (pProcess->hHeap == NULL || pProcess->hInstance == NULL) {
        EarlyFatalError(FILE_LINE);
    }
}
