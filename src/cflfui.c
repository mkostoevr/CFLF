#include <cflf.h>

#define HMENU_MAINBUTTON ((HMENU)43)

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
    pcBotControlButton->hMenu = HMENU_MAINBUTTON;
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

static void ConfigureInterface(PUI pUserInterface) {
    ConfigureBotControlButton(&pUserInterface->cBotControlButton);
    ConfigureStatusLabel(&pUserInterface->cDebugLabel);
}

static void CreateInterface(PUI pUserInterface) {
    CreateControl(&pUserInterface->cBotControlButton);
    CreateControl(&pUserInterface->cDebugLabel);
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        // this message handler executes before CreateWindowEx returns, so let's save
        // handle of main window here, so that we could append controls to this handle
        userInterface.cMainWindow.hHandle = hWnd;
        ConfigureInterface(&userInterface);
        CreateInterface(&userInterface);
        InitializeBot(&bot);
        return 0;
    case WM_COMMAND:
        if (LOWORD(wParam) == (DWORD)userInterface.cBotControlButton.hMenu) {
            SwitchBotRunningState(&bot);
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
    ATOM bClassRegistredSuccesfully = RegisterClassA(&wc);
    if (!bClassRegistredSuccesfully) {
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
