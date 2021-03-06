#include <cflf.h>

#define HMENU_BOTCONTROLBUTTON ((HMENU)43)
#define HMENU_SPEEDEDIT ((HMENU)44)
#define HMENU_ANGLETRACKBAR ((HMENU)45)

VARDEF UI userInterface;

static VOID CreateControl(PCONTROL pcControl) {
    pcControl->hHandle = CreateWindowExA(
        pcControl->dwExStyle,
        pcControl->lpClassName,
        pcControl->lpWindowName,
        pcControl->dwStyle,
        pcControl->X,
        pcControl->Y,
        pcControl->nWidth,
        pcControl->nHeight,
        pcControl->hWndParent,
        pcControl->hMenu,
        pcControl->hInstance,
        pcControl->lpParam);
}

static VOID AdjustMainWindowPosition(PCONTROL pcMainWindow) {
    RECT r;
    INT nTitleBarButtonWitdh;
    INT nTitleBarButtonHeight;
    INT nOffsetFromRightBorderOfTitleBar;
    
    // yes, invalid coordinates can't be saved by the program
    // but what if user will change em manually? why not?
    if (pcMainWindow->Y < 0) {
        pcMainWindow->Y = 0;
    }
    nTitleBarButtonWitdh = GetSystemMetrics(SM_CXSIZE);
    nOffsetFromRightBorderOfTitleBar = nTitleBarButtonWitdh * 4;
    if (pcMainWindow->X < nOffsetFromRightBorderOfTitleBar - pcMainWindow->nWidth) {
        pcMainWindow->X = nOffsetFromRightBorderOfTitleBar - pcMainWindow->nWidth;
    }
    // what if display parameters was changed?
    if (!SystemParametersInfoA(SPI_GETWORKAREA, 0, &r, 0)) {
        Error(FILE_LINE);
        pcMainWindow->X = CW_USEDEFAULT;
        pcMainWindow->Y = CW_USEDEFAULT;
        return;
    }
    if (pcMainWindow->X > r.right - 64) {
        pcMainWindow->X = r.right - 64;
    }
    nTitleBarButtonHeight = GetSystemMetrics(SM_CYSIZE);
    if (pcMainWindow->Y > r.bottom - nTitleBarButtonHeight) {
        pcMainWindow->Y = r.bottom - nTitleBarButtonHeight;
    }
}

static VOID ConfigureMainWindow(PCONTROL pcMainWindow) {
    pcMainWindow->dwExStyle = WS_EX_TOPMOST;
    pcMainWindow->lpClassName = "CFLF";
    pcMainWindow->lpWindowName = "CFLF";
    pcMainWindow->dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    pcMainWindow->nWidth = 250 + 6; // to match sizes of window and its controls in modern windows
    pcMainWindow->nHeight = 400;
    pcMainWindow->hWndParent = NULL;
    pcMainWindow->hMenu = NULL;
    pcMainWindow->hInstance = process.hInstance;
    pcMainWindow->lpParam = NULL;
    LoadConfigDword("x", (DWORD *)&pcMainWindow->X, CW_USEDEFAULT);
    LoadConfigDword("y", (DWORD *)&pcMainWindow->Y, CW_USEDEFAULT);
    if (pcMainWindow->X != CW_USEDEFAULT || pcMainWindow->Y != CW_USEDEFAULT) {
        AdjustMainWindowPosition(&userInterface.cMainWindow);
    }
}

static VOID ConfigureBotControlButton(PCONTROL pcBotControlButton) {
    pcBotControlButton->dwExStyle = 0;
    pcBotControlButton->lpClassName = "Button";
    pcBotControlButton->lpWindowName = LOCAL_UI_start;
    pcBotControlButton->dwStyle = WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
    pcBotControlButton->X = 0;
    pcBotControlButton->Y = 0;
    pcBotControlButton->nWidth = 250;
    pcBotControlButton->nHeight = 32;
    pcBotControlButton->hWndParent = userInterface.cMainWindow.hHandle;
    pcBotControlButton->hMenu = HMENU_BOTCONTROLBUTTON;
    pcBotControlButton->hInstance = userInterface.cMainWindow.hInstance;
    pcBotControlButton->lpParam = NULL;
}

static VOID ConfigureStatusLabel(PCONTROL pcStatusLabel) {
    pcStatusLabel->dwExStyle = 0;
    pcStatusLabel->lpClassName = "Static";
    pcStatusLabel->lpWindowName = LOCAL_UI_initialStatus;
    pcStatusLabel->dwStyle = WS_VISIBLE | WS_CHILD;
    pcStatusLabel->X = 0;
    pcStatusLabel->Y = 32;
    pcStatusLabel->nWidth = 250;
    pcStatusLabel->nHeight = 48;
    pcStatusLabel->hWndParent = userInterface.cMainWindow.hHandle;
    pcStatusLabel->hMenu = NULL;
    pcStatusLabel->hInstance = userInterface.cMainWindow.hInstance;
    pcStatusLabel->lpParam = NULL;
}

static VOID ConfigureSleepTimeEdit(PCONTROL pcSpeedEdit) {
    CHAR szSleepTime[11];

    sprintf(szSleepTime, "%lu", bot.dwSleepTime);
    pcSpeedEdit->dwExStyle = 0;
    pcSpeedEdit->lpClassName = "Edit";
    pcSpeedEdit->lpWindowName = szSleepTime;
    pcSpeedEdit->dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER;
    pcSpeedEdit->X = 0;
    pcSpeedEdit->Y = 80;
    pcSpeedEdit->nWidth = 35;
    pcSpeedEdit->nHeight = 21;
    pcSpeedEdit->hWndParent = userInterface.cMainWindow.hHandle;
    pcSpeedEdit->hMenu = HMENU_SPEEDEDIT;
    pcSpeedEdit->hInstance = userInterface.cMainWindow.hInstance;
    pcSpeedEdit->lpParam = NULL;
}

static VOID ConfigureSleepTimeLabel(PCONTROL pcSpeedLabel) {
    pcSpeedLabel->dwExStyle = 0;
    pcSpeedLabel->lpClassName = "Static";
    pcSpeedLabel->lpWindowName = LOCAL_UI_sleepTime;
    pcSpeedLabel->dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER;
    pcSpeedLabel->X = 35;
    pcSpeedLabel->Y = 80;
    pcSpeedLabel->nWidth = 215;
    pcSpeedLabel->nHeight = 21;
    pcSpeedLabel->hWndParent = userInterface.cMainWindow.hHandle;
    pcSpeedLabel->hMenu = NULL;
    pcSpeedLabel->hInstance = userInterface.cMainWindow.hInstance;
    pcSpeedLabel->lpParam = NULL;
}

static VOID ConfigureAngleTrackbar(PCONTROL pcAngleTrackbar) {
    pcAngleTrackbar->dwExStyle = 0;
    pcAngleTrackbar->lpClassName = TRACKBAR_CLASS;
    pcAngleTrackbar->lpWindowName = NULL;
    pcAngleTrackbar->dwStyle = WS_VISIBLE | WS_CHILD | TBS_HORZ;
    pcAngleTrackbar->X = 0;
    pcAngleTrackbar->Y = 101;
    pcAngleTrackbar->nWidth = 250;
    pcAngleTrackbar->nHeight = 21;
    pcAngleTrackbar->hWndParent = userInterface.cMainWindow.hHandle;
    pcAngleTrackbar->hMenu = HMENU_ANGLETRACKBAR;
    pcAngleTrackbar->hInstance = userInterface.cMainWindow.hInstance;
    pcAngleTrackbar->lpParam = NULL;
}

static VOID ConfigureInterface(PUI pUserInterface) {
    ConfigureBotControlButton(&pUserInterface->cBotControlButton);
    ConfigureStatusLabel(&pUserInterface->cStatusLabel);
    ConfigureSleepTimeEdit(&pUserInterface->cSleepTimeEdit);
    ConfigureSleepTimeLabel(&pUserInterface->cSleepTimeLabel);
    ConfigureAngleTrackbar(&pUserInterface->cAngleTrackbar);
}

static VOID CreateInterface(PUI pUserInterface) {
    CreateControl(&pUserInterface->cBotControlButton);
    CreateControl(&pUserInterface->cStatusLabel);
    CreateControl(&pUserInterface->cSleepTimeEdit);
    CreateControl(&pUserInterface->cSleepTimeLabel);
    CreateControl(&pUserInterface->cAngleTrackbar);
    SendMessageA(pUserInterface->cAngleTrackbar.hHandle, TBM_SETRANGE, FALSE, MAKELPARAM(30, 150));
    SendMessageA(pUserInterface->cAngleTrackbar.hHandle, TBM_SETPOS, TRUE, bot.dwCatchingAngle);
}

static VOID ShiftStringLeft(LPSTR szValue) {
    while (*szValue != '\0') {
        *szValue = *(szValue + 1);
        szValue++;
    }
}

static VOID SaveWindowPosition(PCONTROL pcWindow, LPCSTR szConfigNameX, LPCSTR szConfigNameY) {
    SaveConfigDword(szConfigNameX, pcWindow->X);
    SaveConfigDword(szConfigNameY, pcWindow->Y);
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_NCHITTEST:
    {
        LRESULT hit = DefWindowProc(hWnd, uMsg, wParam, lParam);
        if (hit == HTCLIENT) {
            hit = HTCAPTION;
        }
        return hit;
    }
    case WM_DISPLAYCHANGE:
        if (!IsIconic(userInterface.cMainWindow.hHandle)) {        
            AdjustMainWindowPosition(&userInterface.cMainWindow);
            MoveWindow(userInterface.cMainWindow.hHandle,
                       userInterface.cMainWindow.X,
                       userInterface.cMainWindow.Y,
                       userInterface.cMainWindow.nWidth,
                       userInterface.cMainWindow.nHeight,
                       FALSE);
        }
        break;
    case WM_MOVE:
        if (!IsIconic(userInterface.cMainWindow.hHandle)) {        
            userInterface.cMainWindow.X = (INT)(INT16)LOWORD(lParam);
            userInterface.cMainWindow.Y = (INT)(INT16)HIWORD(lParam);
        }
        return 0;
    case WM_CREATE:
        InitializeBot(&bot);
        // this message handler executes before CreateWindowEx returns, so let's save
        // handle of main window here, so that we could append controls to this handle
        userInterface.cMainWindow.hHandle = hWnd;
        ConfigureInterface(&userInterface);
        CreateInterface(&userInterface);
        return 0;
    case WM_HSCROLL:
        if (lParam == (LPARAM)userInterface.cAngleTrackbar.hHandle) {
            DWORD dwAngle;

            dwAngle = SendMessageA(userInterface.cAngleTrackbar.hHandle, TBM_GETPOS, 0, 0);
            SetBotCatchingAngle(&bot, dwAngle);
        }
        return 0;
    case WM_COMMAND:
        if (LOWORD(wParam) == (DWORD)userInterface.cBotControlButton.hMenu) {
            SwitchBotRunningState(&bot);
        } else if (LOWORD(wParam) == (DWORD)userInterface.cSleepTimeEdit.hMenu) {
            if (HIWORD(wParam) == EN_UPDATE) {
                CHAR szText[8];
                INT bcText;

                bcText = GetWindowTextA((HWND)lParam, szText, 8);
                if (bcText != 0) {
                    BOOL bChanged;

                    bChanged = FALSE;
                    for (INT i = 0; i < bcText; i++) {
                        if (szText[i] < '0' || szText[i] > '9') {
                            ShiftStringLeft(&szText[i]);
                            bChanged = TRUE;
                        }
                    }
                    if (bChanged == TRUE) {
                        DWORD dwSelectionBegin;
                        DWORD dwSelectionEnd;

                        SendMessage((HWND)lParam, EM_GETSEL, (WPARAM)&dwSelectionBegin, (LPARAM)&dwSelectionEnd);
                        dwSelectionBegin -= 1;
                        dwSelectionEnd -= 1;
                        SetWindowText((HWND)lParam, szText);
                        SendMessage((HWND)lParam, EM_SETSEL, (WPARAM)dwSelectionBegin, (LPARAM)dwSelectionEnd);
                    }
                    SetBotSleepTime(&bot, atoi(szText));
                }
            }
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
        SaveWindowPosition(&userInterface.cMainWindow, "x", "y");
        DeinitializeBot(&bot);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static VOID RegisterMainWindowClass(PCONTROL pcMainWindow) {
    WNDCLASSA wc;

    wc.style = 0;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = pcMainWindow->hInstance;
    wc.hIcon = LoadIconA(pcMainWindow->hInstance, MAKEINTRESOURCE(42));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = pcMainWindow->lpClassName;
    if (RegisterClassA(&wc) == 0) {
        FatalError(FILE_LINE);
    }
}

VOID InitializeUserInterface(PUI pUserInterface) {
    ConfigureMainWindow(&pUserInterface->cMainWindow);
    RegisterMainWindowClass(&pUserInterface->cMainWindow);
    CreateControl(&userInterface.cMainWindow);
    if (userInterface.cMainWindow.hHandle == NULL) {
        FatalError(FILE_LINE);
    }
    ShowWindow(userInterface.cMainWindow.hHandle, SW_SHOWNORMAL);
}
