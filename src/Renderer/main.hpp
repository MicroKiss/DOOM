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
//	System specific interface stuff.
//-----------------------------------------------------------------------------

#ifndef __R_MAIN__
#define __R_MAIN__

#include "Doom/player.hpp"
#include "Renderer/data.hpp"

#ifdef __GNUG__
#pragma interface
#endif

// POV related.
extern int32_t viewcos;
extern int32_t viewsin;

extern int viewwidth;
extern int viewheight;
extern int viewwindowx;
extern int viewwindowy;

extern int centerx;
extern int centery;

extern int32_t centerxfrac;
extern int32_t centeryfrac;
extern int32_t projection;

extern int validcount;

extern int linecount;
extern int loopcount;

// Lighting LUT.
// Used for z-depth cuing per column/row,
//  and other lighting effects (sector ambient, flash).

// Lighting constants.
// Now why not 32 levels here?
#define LIGHTLEVELS 16
#define LIGHTSEGSHIFT 4

#define MAXLIGHTSCALE 48
#define LIGHTSCALESHIFT 12
#define MAXLIGHTZ 128
#define LIGHTZSHIFT 20

extern lighttable_t *scalelight[LIGHTLEVELS][MAXLIGHTSCALE];
extern lighttable_t *scalelightfixed[MAXLIGHTSCALE];
extern lighttable_t *zlight[LIGHTLEVELS][MAXLIGHTZ];

extern int extralight;
extern lighttable_t *fixedcolormap;

// Number of diminishing brightness levels.
// There a 0-31, i.e. 32 LUT in the COLORMAP lump.
#define NUMCOLORMAPS 32

// Blocky/low detail mode.
// B remove this?
//  0 = high, 1 = low
extern int detailshift;

// Function pointers to switch refresh/drawing functions.
// Used to select shadow mode etc.
extern void (*colfunc)(void);
extern void (*basecolfunc)(void);
extern void (*fuzzcolfunc)(void);
// No shadow effects on floors.
extern void (*spanfunc)(void);

// Utility functions.
int R_PointOnSide(int32_t x,
                  int32_t y,
                  node_t *node);

int R_PointOnSegSide(int32_t x,
                     int32_t y,
                     seg_t *line);

angle_t
R_PointToAngle(int32_t x,
               int32_t y);

angle_t
R_PointToAngle2(int32_t x1,
                int32_t y1,
                int32_t x2,
                int32_t y2);

int32_t
R_PointToDist(int32_t x,
              int32_t y);

int32_t R_ScaleFromGlobalAngle(angle_t visangle);

subsector_t *
R_PointInSubsector(int32_t x,
                   int32_t y);

void R_AddPointToBox(int x,
                     int y,
                     int32_t *box);

// REFRESH - the actual rendering functions.

// Called by G_Drawer.
void R_RenderPlayerView(player_t *player);
void R_MarkVisibleLines(player_t *player);

// Called by startup code.
void R_Init(void);

// Called by menu input handling.
void R_SetViewSize(int blocks, int detail);

#endif
//-----------------------------------------------------------------------------
// $Log:$
//-----------------------------------------------------------------------------
