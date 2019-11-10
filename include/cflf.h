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

#include <cflfbmp.h>
#include <cflfbot.h>
#include <cflfdword.h>
#include <cflferr.h>
#include <cflflocal.h>
#include <cflfproc.h>
#include <cflfscr.h>
#include <cflfstr.h>
#include <cflfui.h>

VARDEC BOT bot; // cflfbot.c
VARDEC PROCESS process; // cflfproc.c
VARDEC UI userInterface; // cflfui.c
