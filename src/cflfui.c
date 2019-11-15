#include <cflf.h>

#define HMENU_BOTCONTROLBUTTON ((HMENU)43)
#define HMENU_SPEEDEDIT ((HMENU)44)

VARDEF UI userInterface;

static void CreateControl(PCONTROL pcControl)
{
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

static void ConfigureMainWindow(PCONTROL pcMainWindow) {
    pcMainWindow->dwExStyle = WS_EX_TOPMOST;
    pcMainWindow->lpClassName = "CFLF";
    pcMainWindow->lpWindowName = "CFLF";
    pcMainWindow->dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    pcMainWindow->X = CW_USEDEFAULT;
    pcMainWindow->Y = CW_USEDEFAULT;
    pcMainWindow->nWidth = 250 + 6; // to match sizes of window and its controls in modern windows
    pcMainWindow->nHeight = 400;
    pcMainWindow->hWndParent = NULL;
    pcMainWindow->hMenu = NULL;
    pcMainWindow->hInstance = process.hInstance;
    pcMainWindow->lpParam = NULL;
}

static void ConfigureBotControlButton(PCONTROL pcBotControlButton) {
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

static void ConfigureStatusLabel(PCONTROL pcBotControlButton) {
    pcBotControlButton->dwExStyle = 0;
    pcBotControlButton->lpClassName = "Static";
    pcBotControlButton->lpWindowName = LOCAL_UI_initialStatus;
    pcBotControlButton->dwStyle = WS_VISIBLE | WS_CHILD;
    pcBotControlButton->X = 0;
    pcBotControlButton->Y = 32;
    pcBotControlButton->nWidth = 250;
    pcBotControlButton->nHeight = 48;
    pcBotControlButton->hWndParent = userInterface.cMainWindow.hHandle;
    pcBotControlButton->hMenu = NULL;
    pcBotControlButton->hInstance = userInterface.cMainWindow.hInstance;
    pcBotControlButton->lpParam = NULL;
}

static void ConfigureSleepTimeEdit(PCONTROL pcSpeedEdit) {
    CHAR szSleepTime[11];

    DwordToStr(bot.dwSleepTime, szSleepTime, 10, "");
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

static void ConfigureSleepTimeLabel(PCONTROL pcSpeedEdit) {
    pcSpeedEdit->dwExStyle = 0;
    pcSpeedEdit->lpClassName = "Static";
    pcSpeedEdit->lpWindowName = LOCAL_UI_sleepTime;
    pcSpeedEdit->dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER;
    pcSpeedEdit->X = 35;
    pcSpeedEdit->Y = 80;
    pcSpeedEdit->nWidth = 215;
    pcSpeedEdit->nHeight = 21;
    pcSpeedEdit->hWndParent = userInterface.cMainWindow.hHandle;
    pcSpeedEdit->hMenu = HMENU_SPEEDEDIT;
    pcSpeedEdit->hInstance = userInterface.cMainWindow.hInstance;
    pcSpeedEdit->lpParam = NULL;
}

static void ConfigureInterface(PUI pUserInterface) {
    ConfigureBotControlButton(&pUserInterface->cBotControlButton);
    ConfigureStatusLabel(&pUserInterface->cStatusLabel);
    ConfigureSleepTimeEdit(&pUserInterface->cSleepTimeEdit);
    ConfigureSleepTimeLabel(&pUserInterface->cSleepTimeLabel);
}

static void CreateInterface(PUI pUserInterface) {
    CreateControl(&pUserInterface->cBotControlButton);
    CreateControl(&pUserInterface->cStatusLabel);
    CreateControl(&pUserInterface->cSleepTimeEdit);
    CreateControl(&pUserInterface->cSleepTimeLabel);
}

static VOID ShiftStringLeft(LPSTR szValue) {
    while (*szValue != '\0') {
        *szValue = *(szValue + 1);
        szValue++;
    }
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        InitializeBot(&bot);
        // this message handler executes before CreateWindowEx returns, so let's save
        // handle of main window here, so that we could append controls to this handle
        userInterface.cMainWindow.hHandle = hWnd;
        ConfigureInterface(&userInterface);
        CreateInterface(&userInterface);
        return 0;
    case WM_COMMAND:
        if (LOWORD(wParam) == (DWORD)userInterface.cBotControlButton.hMenu) {
            SwitchBotRunningState(&bot);
        } else if (LOWORD(wParam) == (DWORD)userInterface.cSleepTimeLabel.hMenu) {
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
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static void RegisterMainWindowClass(PCONTROL pcMainWindow) {
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
