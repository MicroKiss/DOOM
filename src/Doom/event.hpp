// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
// $Id:$
// Copyright (C) 1993-1996 by id Software, Inc.
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
// DESCRIPTION:
//-----------------------------------------------------------------------------

#ifndef __D_EVENT__
#define __D_EVENT__

#include "doomtype.hpp"

// Event handling.

// Input event types.
typedef enum
{
    ev_keydown = 0,
    ev_keyup = 1,
    ev_mouse = 2,
    ev_joystick = 3,
    ev_mousewheel = 4
} evtype_t;

// Event structure.
typedef struct
{
    evtype_t type;
    int data1; // keys / mouse/joystick buttons
    int data2; // mouse/joystick x move
    int data3; // mouse/joystick y move
} event_t;

typedef enum
{
    ga_nothing,
    ga_loadlevel,
    ga_newgame,
    ga_loadgame,
    ga_savegame,
    ga_completed,
    ga_victory,
    ga_worlddone,
    ga_screenshot
} gameaction_t;

// GLOBAL VARIABLES
#define MAXEVENTS 64

extern event_t events[MAXEVENTS];
extern int eventhead;
extern int eventtail;

extern gameaction_t gameaction;

#endif
//-----------------------------------------------------------------------------
// $Log:$
//-----------------------------------------------------------------------------
