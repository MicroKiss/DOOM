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
//
//-----------------------------------------------------------------------------

static const char
    rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_timer.h>
#include "i_system.h"
#include "doomdef.h"

int mb_used = 6;

void I_Tactile(int on,
               int off,
               int total)
{
    // UNUSED.
    on = off = total = 0;
}

ticcmd_t emptycmd;
ticcmd_t *I_BaseTiccmd(void)
{
    return &emptycmd;
}

int I_GetHeapSize(void)
{
    return mb_used * 1024 * 1024;
}

byte *I_ZoneBase(int *size)
{
    *size = mb_used * 1024 * 1024;
    return (byte *)malloc(*size);
}

//
// I_GetTime
// returns time in 1/70th second tics
//
int I_GetTime(void)
{
    static Uint32 basetime;
    static boolean initialized;
    Uint32 now = SDL_GetTicks();

    if (!initialized)
    {
        basetime = now;
        initialized = true;
    }

    return (int)(((Uint64)(now - basetime) * TICRATE) / 1000);
}

void I_Init(void)
{
    I_InitSound();
    //  I_InitGraphics();
}

void I_Quit(void)
{
    D_QuitNetGame();
    I_ShutdownSound();
    I_ShutdownMusic();
    M_SaveDefaults();
    I_ShutdownGraphics();
    exit(0);
}

void I_WaitVBL(int count)
{
    SDL_Delay(count * (1000 / 70));
}

void I_BeginRead(void)
{
    // unused
}

void I_EndRead(void)
{
    // unused
}

byte *I_AllocLow(int length)
{
    byte *mem;
    mem = calloc(1, length);
    return mem;
}

//
// I_Error
//
void I_Error(char *error, ...)
{
    va_list argptr;

    va_start(argptr, error);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, error, argptr);
    fprintf(stderr, "\n");
    va_end(argptr);

    fflush(stderr);
    exit(EXIT_FAILURE);
}
