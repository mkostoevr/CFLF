#include <cflf.h>

static LPSTR GetErrorString(DWORD dwErrorCode) {
    LPSTR szErrorString;
    LPSTR szErrorStringBuffer;
    DWORD dwErrorStringBufferSize;
    if(dwErrorCode == 0) {
        szErrorString = HeapAlloc(process.hHeap, 0, sizeof(LOCAL_ERR_noError));
        lstrcpyA(szErrorString, LOCAL_ERR_noError);
        return szErrorString;
    }
    dwErrorStringBufferSize = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&szErrorStringBuffer,
        0,
        NULL
        );
    szErrorString = HeapAlloc(process.hHeap, 0, dwErrorStringBufferSize + 1);
    lstrcpyA(szErrorString, szErrorStringBuffer);
    LocalFree(szErrorStringBuffer);
    return szErrorString;
}

static LPSTR CreateErrorMessage(DWORD dwErrorCode, LPCSTR szErrorLocation) {
    CHAR szErrorCode[10];
    LPSTR szErrorInfo;
    DWORD_PTR nMessageSize;
    LPSTR szMessage;
    DwordToStr(dwErrorCode, szErrorCode, 16, "0x");
    szErrorInfo = GetErrorString(dwErrorCode);
    nMessageSize = sizeof(LOCAL_ERR_errorNumber)
        + sizeof(szErrorCode)
        + sizeof(LOCAL_ERR_atLocation)
        + lstrlenA(szErrorLocation)
        + sizeof(LOCAL_ERR_moreInfo)
        + lstrlenA(szErrorInfo)
        + 1;
    szMessage = HeapAlloc(process.hHeap, 0, nMessageSize);
    lstrcpyA(szMessage, LOCAL_ERR_errorNumber);
    lstrcatA(szMessage, szErrorCode);
    lstrcatA(szMessage, LOCAL_ERR_atLocation);
    lstrcatA(szMessage, szErrorLocation);
    lstrcatA(szMessage, LOCAL_ERR_moreInfo);
    lstrcatA(szMessage, szErrorInfo);
    HeapFree(process.hHeap, 0, szErrorInfo);
    return szMessage;
}

VOID Error(LPCSTR szErrorLocation) {
    DWORD dwErrorCode;
    LPSTR szErrorMessage;
    dwErrorCode = GetLastError();
    szErrorMessage = CreateErrorMessage(dwErrorCode, szErrorLocation);
    MessageBoxA(
        userInterface.cMainWindow.hHandle,
        szErrorMessage,
        userInterface.cMainWindow.lpWindowName,
        MB_ICONERROR | MB_OK
        );
    HeapFree(process.hHeap, 0, szErrorMessage);
}

VOID FatalError(LPCSTR szErrorLocation) {
    Error(szErrorLocation);
    ExitProcess(-1);
}

VOID EarlyError(LPCSTR szErrorLocation) {
    DWORD dwErrorCode;
    CHAR szErrorCode[11];
    dwErrorCode = GetLastError();
    DwordToStr(dwErrorCode, szErrorCode, 16, "0x");
    MessageBoxA(0, szErrorCode, szErrorLocation, MB_ICONERROR | MB_OK);
}

VOID EarlyFatalError(LPCSTR szErrorLocation) {
    EarlyError(szErrorLocation);
    ExitProcess(-1);
}
