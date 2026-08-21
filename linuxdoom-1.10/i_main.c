// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//
//-----------------------------------------------------------------------------

static const char
    rcsid[] = "$Id: i_main.c,v 1.4 1997/02/03 22:45:10 b1 Exp $";

#include "doomdef.h"

#include <string.h>

#include "m_argv.h"
#include "d_main.h"
#include "i_system.h"

int main(int argc, char **argv)
{
    int i;

    myargc = argc;
    myargv = argv;

#ifdef DOOM_DEBUG
    for (i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-timertest"))
        {
            boolean passed;
            I_Init();
            passed = I_RunTimerTest();
            I_Shutdown();
            return passed ? 0 : 1;
        }
    }
#else
    (void)i;
#endif

    D_DoomMain();

    I_Quit();
    return 0;
}
