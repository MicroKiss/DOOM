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

#include "doomdef.h"

#include <string.h>

#include "m_argv.hpp"
#include "d_main.hpp"
#include "i_system.h"
#include "i_video.h"
#include "v_video.hpp"
#include "i_main_test.h"

int main(int argc, char **argv)
{
    int i;

    myargc = argc;
    myargv = argv;

#ifdef DOOM_DEBUG
    int test_result = TestMain();
    if (test_result >= 0)
        return test_result;
#endif

    D_DoomMain();

    I_Quit();
    return 0;
}
