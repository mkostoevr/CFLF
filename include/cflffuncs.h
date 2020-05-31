// cflfbmp.c
FUNDEC COLORREF GetBitmapPixel(FULLBITMAP fbmp, UINT x, UINT y);

// cflfbot.c
FUNDEC VOID InitializeBot(PBOT pBot);
FUNDEC VOID DeinitializeBot(PBOT pBot);
FUNDEC VOID SwitchBotRunningState(PBOT pBot);
FUNDEC VOID SetBotSleepTime(PBOT pBot, DWORD dwSleepTime);
FUNDEC VOID SetBotCatchingAngle(PBOT pBot, DWORD dwCatchingAngle);

// cflfcfg.c
FUNDEC BOOL SaveConfigDword(LPCSTR szName, DWORD dwValue);
FUNDEC BOOL LoadConfigDword(LPCSTR szName, DWORD *dwValue, DWORD dwDefaultValue);

// cflferr.c
FUNDEC VOID Error(LPCSTR szErrorLocation);
FUNDEC VOID FatalError(LPCSTR szErrorLocation);
FUNDEC VOID LowLevelError(LPCSTR szErrorLocation);
FUNDEC VOID FatalLowLevelError(LPCSTR szErrorLocation);

// cflfproc.c
FUNDEC VOID InitializeProcess(PPROCESS pProcess);

// cflfscr.c
FUNDEC FULLBITMAP WINAPI GetScreenBitmap();

// cflfui.c
FUNDEC VOID InitializeUserInterface(PUI pUserInterface);
