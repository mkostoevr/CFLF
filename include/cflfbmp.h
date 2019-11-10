#include <cflf.h>

#include <windows.h>

typedef struct tagFULLBITMAP {
    HBITMAP hHandle;
    PBYTE pBits;
    UINT cbBits;
    UINT lWidth;
    UINT lHeight;
} FULLBITMAP;

FUNDEC COLORREF GetBitmapPixel(FULLBITMAP fbmp, UINT x, UINT y);
