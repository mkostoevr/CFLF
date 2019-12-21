typedef struct tagFULLBITMAP {
    HBITMAP hHandle;
    PBYTE pBits;
    UINT cbBits;
    UINT lWidth;
    UINT lHeight;
} FULLBITMAP;

typedef struct tagBOT {
    POINT pFirstBrownTabDownerRighterCorner;
    POINT pPlusButtonCenter;
    POINT pGameplayArcVertex;
    POINT pGameplayArcCenter;
    UINT nGameplayArcRadius;
    FULLBITMAP fbmp;
    BOOL bIsWorking;
    BOOL bGameIsFound;
    DWORD dwThreadId;
    HANDLE hThread;
    DWORD dwSleepTime;
} BOT, *PBOT;

typedef struct tagPROCESS {
    HANDLE hHeap;
    HINSTANCE hInstance;
} PROCESS, *PPROCESS;

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
    CONTROL cSleepTimeEdit;
    CONTROL cSleepTimeLabel;
} UI, *PUI;
