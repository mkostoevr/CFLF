#include <cflf.h>

VARDEF BOT bot;

static VOID SleepBot(PBOT pBot) {
    if (pBot->dwSleepTime > 0) {
        Sleep(pBot->dwSleepTime);
    } else {
        return;
    }
}

static BOOL FreeBotBitmap(PBOT pBot) {
    if (pBot->fbmp.hHandle != NULL) {
        if (DeleteObject(pBot->fbmp.hHandle) == FALSE) {
            return FALSE;
        }
    }
    pBot->fbmp.hHandle = NULL;
    return TRUE;
}

static BOOL UpdateBotBitmap(PBOT pBot) {
    if (FreeBotBitmap(pBot) == FALSE){
        return FALSE;
    }
    pBot->fbmp = GetScreenBitmap();
    return TRUE;
}

static VOID FindKeyPoints(PBOT pBot, UINT x, UINT y) {
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
}

static VOID FindGame(PBOT pBot) {
    BOOL bItsTimeToStop;

    bItsTimeToStop = FALSE;
    if (UpdateBotBitmap(pBot) == FALSE) {
        Error(FILE_LINE);
    }
    // find first grow tab upper lefter pixel
    for (UINT y = 0; y < pBot->fbmp.lHeight && !bItsTimeToStop; y++) {
        for (UINT x = 0; x < pBot->fbmp.lWidth; x++) {
            const COLORREF cFirstGrayTabUpperLefterCornerUsual = 0x482927;
            COLORREF cPixel;

            cPixel = GetBitmapPixel(pBot->fbmp, x, y);
            if (cPixel & 0xff000000) {
                Error(FILE_LINE);
                bItsTimeToStop = TRUE;
                break;
            }
            if (cPixel == cFirstGrayTabUpperLefterCornerUsual) {
                const UINT bcX = 11;
                const UINT bcY = 11;
                const UINT bcStatus = sizeof(LOCAL_BOT_gameFirstGrayTabFoundAt) + bcX + 1 + bcY;
                CHAR szX[bcX];
                CHAR szY[bcY];
                CHAR szStatus[bcStatus];

                DwordToStr(x, szX, 10, "");
                DwordToStr(y, szY, 10, "");
                lstrcpyA(szStatus, LOCAL_BOT_gameFirstGrayTabFoundAt);
                lstrcatA(szStatus, szX);
                lstrcatA(szStatus, ":");
                lstrcatA(szStatus, szY);
                SetWindowTextA(userInterface.cStatusLabel.hHandle, szStatus);
                FindKeyPoints(pBot, x, y);
                pBot->bGameIsFound = TRUE;
                bItsTimeToStop = TRUE;
                break;
            }
        }
    }
    if (pBot->bGameIsFound == FALSE) {
        SetWindowTextA(userInterface.cStatusLabel.hHandle, LOCAL_BOT_gameNotFound);
    }
    return;
}

static DWORD WINAPI BotCicle(LPVOID lpParam) {
    PBOT pBot;
    LONG nEscPressed;
    
    pBot = lpParam;
    nEscPressed = 0;
    pBot->bGameIsFound = FALSE;
    while (pBot->bIsWorking) {
        // without this condition the bot would try to play when its own window is in focus
        // so it would frequently press SPACE key, what activates last pressed button's click event
        // it looks ugly when after bot activating the button starts frequently click itself
        if (GetForegroundWindow() != userInterface.cMainWindow.hHandle) {
            const COLORREF cGameplayArcVertexGreen = 0x33B032;
            const COLORREF cPlusButtonCenterUsual = 0x614707;
            COLORREF cGameplayArcVertex;
            COLORREF cPlusButtonCenter;

            // < 80ms
            if (UpdateBotBitmap(pBot) == FALSE) {
                Error(FILE_LINE);
            }
            cGameplayArcVertex = GetBitmapPixel(pBot->fbmp, pBot->pGameplayArcVertex.x, pBot->pGameplayArcVertex.y);
            if (cGameplayArcVertex == cGameplayArcVertexGreen) {
                nEscPressed = 0;
                keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
            } else {
                keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
            }
            // if plus center color is unusual, highly likely this is cause some window openned
            // (catched fish information, championship results, etc.)
            // anyway, each window in the game may be closed pressing ESC key
            cPlusButtonCenter = GetBitmapPixel(pBot->fbmp, pBot->pPlusButtonCenter.x, pBot->pPlusButtonCenter.y);
            if (cPlusButtonCenter != cPlusButtonCenterUsual) {
                keybd_event(VK_ESCAPE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
                keybd_event(VK_ESCAPE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
                nEscPressed++;
                // if the button has no its usual gray color, and after 100-time ESC key simulating the
                // color not changes, that means that the game is lost, and it's time to find it again
                if (nEscPressed > 25 && pBot->bGameIsFound == TRUE) {
                    pBot->bGameIsFound = FALSE;
                    SetWindowTextA(userInterface.cStatusLabel.hHandle, LOCAL_BOT_gameIsLost);
                }
                if (pBot->bGameIsFound == FALSE) {
                    FindGame(pBot);
                    if (pBot->bGameIsFound == TRUE) {
                        nEscPressed = 0;
                    }
                }
            }
        }
        SleepBot(pBot);
    }
    ExitThread(0);
}

static VOID StartBot(PBOT pBot) {
    pBot->bIsWorking = TRUE;
    pBot->hThread = CreateThread(NULL, 0, BotCicle, pBot, 0, &pBot->dwThreadId);
    if (pBot->hThread == NULL) {
        Error(FILE_LINE);
        SendMessage(userInterface.cBotControlButton.hHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        SendMessage(userInterface.cBotControlButton.hHandle, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
        return;
    }
}

static VOID StopBot(PBOT pBot) {
    pBot->bIsWorking = FALSE;
    if (pBot->hThread != NULL) {
        if (CloseHandle(pBot->hThread) == FALSE) {
            Error(FILE_LINE);
        }
    }
}

VOID InitializeBot(PBOT pBot) {
    pBot->dwSleepTime = 250;
    pBot->bIsWorking = FALSE;
    pBot->bGameIsFound = FALSE;
    pBot->fbmp.hHandle = NULL;
}

VOID SwitchBotRunningState(PBOT pBot) {
    if (pBot->bIsWorking) {
        StopBot(pBot);
        SetWindowTextA(userInterface.cBotControlButton.hHandle, LOCAL_UI_start);
    } else {
        StartBot(pBot);
        SetWindowTextA(userInterface.cBotControlButton.hHandle, LOCAL_UI_stop);
    }
}

VOID SetBotSleepTime(PBOT pBot, DWORD dwSleepTime) {
    pBot->dwSleepTime = dwSleepTime;
}
