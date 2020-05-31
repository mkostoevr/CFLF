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
    pBot->pFirstBrownTabDownerRighterCorner.y = y;
    pBot->pFirstBrownTabDownerRighterCorner.x = x;
    x += 87;
    y += 17;
    pBot->pPlusButtonCenter.x = x;
    pBot->pPlusButtonCenter.y = y;
    x += 67;
    y += 241;
    pBot->pGameplayArcVertex.x = x;
    pBot->pGameplayArcVertex.y = y;
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
    // find first brown tab bottom right pixel
    for (UINT y = pBot->fbmp.lHeight - 1; y != 0 && !bItsTimeToStop; y--) {
        for (UINT x = 0; x < pBot->fbmp.lWidth; x++) {
            const COLORREF cBrownTab = 0x482927;
            COLORREF cPixel;
            UINT nBrownPixelInRow;

            nBrownPixelInRow = 0;
            for (;;) {
                cPixel = GetBitmapPixel(pBot->fbmp, x, y);
                if (cPixel & 0xff000000) {
                    break;
                }
                if (cPixel == cBrownTab) {
                    nBrownPixelInRow++;
                    x++;
                } else {
                    break;
                }
            }
            if (nBrownPixelInRow >= 100) {
                const UINT bcX = 11;
                const UINT bcY = 11;
                const UINT bcStatus = sizeof(LOCAL_BOT_gameFirstBrownTabFoundAt) + bcX + 1 + bcY;
                CHAR szStatus[bcStatus];
                
                x--; // back to brown pixel
                sprintf(szStatus, "%s%u:%u", LOCAL_BOT_gameFirstBrownTabFoundAt, x, y);
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
    BOOL bItsTimeToPlay;
    
    pBot = lpParam;
    nEscPressed = 0;
    bItsTimeToPlay = FALSE;
    pBot->bGameIsFound = FALSE;
    while (pBot->bIsWorking) {
        // without this condition the bot would try to play when its own window is in focus
        // so it would frequently press SPACE key, what activates last pressed button's click event
        // it looks ugly when after bot activating the button starts frequently click itself
        if (GetForegroundWindow() != userInterface.cMainWindow.hHandle) {
            const COLORREF cGameplayArcVertexGreen = 0x34b133;
            const COLORREF cGameplayArcVertexBlue = 0x3d6387;
            const COLORREF cPlusButtonCenterUsual = 0x614707;
            COLORREF cGameplayArcVertex;
            COLORREF cPlusButtonCenter;

            if (UpdateBotBitmap(pBot) == FALSE) {
                Error(FILE_LINE);
            }
            // if plus center color is unusual, highly likely this is cause some window openned
            // (catched fish information, championship results, etc.)
            // anyway, each window in the game may be closed pressing ESC key
            cPlusButtonCenter = GetBitmapPixel(pBot->fbmp, pBot->pPlusButtonCenter.x, pBot->pPlusButtonCenter.y);
            if (cPlusButtonCenter != cPlusButtonCenterUsual) {
                // no usual color = no gameplay arc = no reason to play
                bItsTimeToPlay = FALSE;
                // if bot won't sleep here, before and between keyboard emulation, it may be detected
                SleepBot(pBot);
                keybd_event(VK_ESCAPE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
                SleepBot(pBot);
                keybd_event(VK_ESCAPE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
                nEscPressed++;
                // if the button has no its usual gray color, and after 25-time ESC key simulating the
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
            } else {
                // if color of gameplay arc's vertex is blue or green, that means that the minigame started
                cGameplayArcVertex = GetBitmapPixel(pBot->fbmp, pBot->pGameplayArcVertex.x, pBot->pGameplayArcVertex.y);
                if (cGameplayArcVertex == cGameplayArcVertexBlue ||
                    cGameplayArcVertex == cGameplayArcVertexGreen) {
                    bItsTimeToPlay = TRUE;
                } else {
                    // try to start the minigame if it isn't started
                    keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
                }
            }
            if (bItsTimeToPlay) {
                cGameplayArcVertex = GetBitmapPixel(pBot->fbmp,
                                                    pBot->pGameplayArcVertex.x + pBot->pGameplayArcCatchPointOffset.x,
                                                    pBot->pGameplayArcCenter.y + pBot->pGameplayArcCatchPointOffset.y);
                if (cGameplayArcVertex == cGameplayArcVertexBlue) {
                    keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
                } else {
                    nEscPressed = 0;
                    // XXX: possible detection - more ups than downs
                    keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
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
        if (WaitForSingleObject(pBot->hThread, INFINITE) == WAIT_FAILED) {
            Error(FILE_LINE);
        }
        if (CloseHandle(pBot->hThread) == FALSE) {
            Error(FILE_LINE);
        }
        pBot->hThread = NULL;
    }
}

VOID InitializeBot(PBOT pBot) {
    if (!LoadConfigDword("dwSleepTime", &pBot->dwSleepTime, 250)) {
        Error(FILE_LINE);
    }
    if (!LoadConfigDword("dwCatchingAngle", &pBot->dwCatchingAngle, 90)) {
        Error(FILE_LINE);
    }
    pBot->nGameplayArcRadius = 136;
    SetBotCatchingAngle(pBot, pBot->dwCatchingAngle);
    pBot->bIsWorking = FALSE;
    pBot->bGameIsFound = FALSE;
    pBot->fbmp.hHandle = NULL;
}

VOID DeinitializeBot(PBOT pBot) {
    SaveConfigDword("dwSleepTime", pBot->dwSleepTime);
    SaveConfigDword("dwCatchingAngle", pBot->dwCatchingAngle);
}

VOID SwitchBotRunningState(PBOT pBot) {
    if (pBot->bIsWorking) {
        SetWindowTextA(userInterface.cBotControlButton.hHandle, LOCAL_BOT_botIsStopping);
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

VOID SetBotCatchingAngle(PBOT pBot, DWORD dwCatchingAngle) {
    DOUBLE fCatchingAngle;

    fCatchingAngle = dwCatchingAngle * 3.14 / 180.0;
    pBot->dwCatchingAngle = dwCatchingAngle;
    pBot->pGameplayArcCatchPointOffset.x = pBot->nGameplayArcRadius * cos(fCatchingAngle) * -1.0;
    pBot->pGameplayArcCatchPointOffset.y = pBot->nGameplayArcRadius * sin(fCatchingAngle) * -1.0;
}
