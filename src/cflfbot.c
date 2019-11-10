#include <cflf.h>

VARDEF BOT bot;

static VOID UpdateBotBitmap(PBOT pBot) {
    pBot->fbmp = GetScreenBitmap();
}

static VOID FindGame(PBOT pBot) {
    UpdateBotBitmap(pBot);
    // this promises to be useful
    UINT x = 0;
    UINT y = 0;
    // find first grow tab upper lefter pixel
    for (y = 0; y < pBot->fbmp.lHeight; y++) {
        for (x = 0; x < pBot->fbmp.lWidth; x++) {
            COLORREF cPixel = GetBitmapPixel(pBot->fbmp, x, y);
            const COLORREF cUsualFirstGrayTabUpperLefterCornerColor = 0x482927;
            if (cPixel == cUsualFirstGrayTabUpperLefterCornerColor) {
                CHAR szX[11];
                CHAR szY[11];
                CHAR szStatus[256] = LOCAL_BOT_gameFirstGrayTabFoundAt;
                DwordToStr(x, szX, 10, "");
                DwordToStr(y, szY, 10, "");
                StrCat(szStatus, szX, 256);
                StrCat(szStatus, ":", 256);
                StrCat(szStatus, szY, 256);
                SetWindowText(userInterface.cDebugLabel.hHandle, szStatus);
                goto FIRST_BROWN_TAB_UPPER_LEFTER_CORNER_COORDINATES_FOUND;
            }
        }
    }
    SetWindowText(userInterface.cDebugLabel.hHandle, LOCAL_BOT_gameNotFound);
    pBot->bGameIsFound = FALSE;
    goto CLEANUP_AND_RETURN;
FIRST_BROWN_TAB_UPPER_LEFTER_CORNER_COORDINATES_FOUND:
    // calculate next key points
    pBot->pFirstBrownTabUpperLefterCorner.x = x;
    pBot->pFirstBrownTabUpperLefterCorner.y = y;
    x += 192;
    y += 40;
    pBot->pPlusButtonCenter.x = x;
    pBot->pPlusButtonCenter.y = y;
    x += 67;
    y += 241;
    pBot->pGameplayArcVertex.x = x;
    pBot->pGameplayArcVertex.y = y;
    pBot->nGameplayArcRadius = 136;
    y += pBot->nGameplayArcRadius;
    pBot->pGameplayArcCenter.x = x;
    pBot->pGameplayArcCenter.y = y;
    // trigger
    pBot->bGameIsFound = TRUE;
CLEANUP_AND_RETURN:
    // free bitmap
    DeleteObject(pBot->fbmp.hHandle);
    return;
}

static DWORD WINAPI BotCicle(LPVOID lpParam) {
    PBOT pBot = lpParam;
    static LONG nEscPressed = 0;
    pBot->bGameIsFound = FALSE;
    while (pBot->bIsWorking) {
        if (GetForegroundWindow() != userInterface.cMainWindow.hHandle) {
            const COLORREF cUsualGameplayArcVertexColor = 0x33B032;
            const COLORREF cUsualPlusButtonCenterColor = 0x614707;
            UpdateBotBitmap(pBot);
            if (GetBitmapPixel(pBot->fbmp, pBot->pGameplayArcVertex.x, pBot->pGameplayArcVertex.y)
                == cUsualGameplayArcVertexColor) {
                nEscPressed = 0;
                keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
            } else {
                keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
            }
            // if plus center color is unusual, highly likely this is cause some window openned
            // (catched fish information, championship results, etc.)
            // anyway, each window in the game may be closed pressing ESC key
            if (GetBitmapPixel(pBot->fbmp, pBot->pPlusButtonCenter.x, pBot->pPlusButtonCenter.y)
                != cUsualPlusButtonCenterColor) {
                keybd_event(VK_ESCAPE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
                keybd_event(VK_ESCAPE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
                nEscPressed++;
                // if the button has no its usual gray color, and after 100-time ESC key simulating the
                // color not changes, that means that the game is lost, and it's time to find it again
                if (nEscPressed > 100) {
                    pBot->bGameIsFound = FALSE;
                    SetWindowText(userInterface.cDebugLabel.hHandle, LOCAL_BOT_gameIsLost);
                }
                if (pBot->bGameIsFound == FALSE) {
                    FindGame(pBot);
                    if (pBot->bGameIsFound == TRUE) {
                        nEscPressed = 0;
                    }
                }
            }
            DeleteObject(pBot->fbmp.hHandle);
        }
        Sleep(pBot->nSleepTime);
    }
    ExitThread(0);
}

static VOID StartBot(PBOT pBot) {
    pBot->bIsWorking = TRUE;
    pBot->hThread = CreateThread(NULL, 0, BotCicle, pBot, 0, &pBot->dwThreadId);
}

static VOID StopBot(PBOT pBot) {
    pBot->bIsWorking = FALSE;
    CloseHandle(pBot->hThread);
}

VOID InitializeBot(PBOT pBot) {
    pBot->nSleepTime = 250;
    pBot->bIsWorking = FALSE;
    pBot->bGameIsFound = FALSE;
}

VOID SwitchBotRunningState(PBOT pBot) {
    if (pBot->bIsWorking) {
        StopBot(pBot);
        SetWindowText(userInterface.cBotControlButton.hHandle, LOCAL_UI_start);
    } else {
        StartBot(pBot);
        SetWindowText(userInterface.cBotControlButton.hHandle, LOCAL_UI_stop);
    }
}
