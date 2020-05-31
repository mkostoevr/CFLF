#pragma once

#ifdef ONE_SOURCE
#   define FUNDEC static
#   define VARDEC static
#   define VARDEF static
#else
#   define FUNDEC
#   define VARDEC extern
#   define VARDEF
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define FILE_LINE "(" __FILE__ ": " STR(__LINE__) ")"
#define DEBUG(...) { CHAR msg[512];                                        \
                   sprintf(msg, __VA_ARGS__);                              \
                   SetWindowTextA(userInterface.cStatusLabel.hHandle, msg);}
#define DEBUGMB(...) { CHAR msg[512];                \
                     sprintf(msg, __VA_ARGS__);      \
                     MessageBoxA(NULL, msg, NULL, 0);}

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>

#include <cflftypes.h>
#include <cflffuncs.h>
#include <cflflocal.h>

VARDEC BOT bot;          // cflfbot.c
VARDEC PROCESS process;  // cflfproc.c
VARDEC UI userInterface; // cflfui.c
