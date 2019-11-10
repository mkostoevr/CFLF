#include <cflf.h>

static void Swap(PCHAR t1, PCHAR t2) {
    CHAR temp = *t1;
    *t1 = *t2;
    *t2 = temp;
}

static void Reverse(LPSTR str, int length) {
    int start = 0;
    int end = length -1;
    while (start < end) {
        Swap(str + start, str + end);
        start++;
        end--;
    }
}

LPSTR DwordToStr(DWORD dwNumber, LPSTR szDestination, int base, LPCSTR szPrefix) {
    int prefixLength = StrLen(szPrefix);
    StrCpy(szDestination, szPrefix, prefixLength + 1);
    int i = 0;
    szDestination += prefixLength;
    // If zero won't be handled individually, empty string will be printed
    if (dwNumber == 0) {
        szDestination[i++] = '0';
        szDestination[i] = '\0';
        return szDestination;
    }
    while (dwNumber != 0) {
        int rem = dwNumber % base;
        szDestination[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        dwNumber = dwNumber / base;
    }
    szDestination[i] = '\0';
    Reverse(szDestination, i);
    return szDestination;
}