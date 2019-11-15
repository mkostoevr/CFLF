#include <cflf.h>

static VOID Swap(PCHAR t1, PCHAR t2) {
    CHAR temp;

    temp = *t1;
    *t1 = *t2;
    *t2 = temp;
}

static VOID Reverse(LPSTR str, INT length) {
    INT start;
    INT end;

    start = 0;
    end = length -1;
    while (start < end) {
        Swap(str + start, str + end);
        start++;
        end--;
    }
}

LPSTR DwordToStr(DWORD dwNumber, LPSTR szDestination, INT base, LPCSTR szPrefix) {
    LPSTR szOrigDestination;
    INT bcPrefix;
    INT i;

    if (base > 36 || base == 0) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
    bcPrefix = lstrlenA(szPrefix);
    lstrcpyA(szDestination, szPrefix);
    szOrigDestination = szDestination;
    szDestination += bcPrefix;
    i = 0;
    do {
        INT rem;
        
        rem = dwNumber % base;
        szDestination[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        dwNumber = dwNumber / base;
    } while (dwNumber != 0);
    szDestination[i] = '\0';
    Reverse(szDestination, i);
    return szOrigDestination;
}