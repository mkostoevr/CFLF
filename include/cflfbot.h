#pragma once

#include <cflf.h>

#include <windows.h>

typedef struct tagBOT {
    POINT pFirstBrownTabUpperLefterCorner;
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

FUNDEC VOID InitializeBot(PBOT pBot);
FUNDEC VOID SwitchBotRunningState(PBOT pBot);
FUNDEC VOID SetBotSleepTime(PBOT pBot, DWORD dwSleepTime);
