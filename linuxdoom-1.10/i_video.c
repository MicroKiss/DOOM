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
// This source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//     Temporary video and input platform stubs.
//
//-----------------------------------------------------------------------------

#include "i_system.h"
#include "i_video.h"

void I_InitGraphics(void)
{
    I_Error("I_InitGraphics is not implemented");
}

void I_ShutdownGraphics(void)
{
    // No video resources exist until the SDL graphics backend is implemented.
}

void I_StartFrame(void)
{
    I_Error("I_StartFrame is not implemented");
}

void I_StartTic(void)
{
    I_Error("I_StartTic is not implemented");
}

void I_UpdateNoBlit(void)
{
    I_Error("I_UpdateNoBlit is not implemented");
}

void I_FinishUpdate(void)
{
    I_Error("I_FinishUpdate is not implemented");
}

void I_ReadScreen(byte *screen)
{
    I_Error("I_ReadScreen is not implemented");
}

void I_SetPalette(byte *palette)
{
    I_Error("I_SetPalette is not implemented");
}