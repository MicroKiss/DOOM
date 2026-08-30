#pragma once

#include "Inputs/InputHandler.hpp"
#include "doomdef.hpp"

#ifdef __GNUG__
#pragma interface
#endif

// Called by DoomMain.
void I_Init(void);

// Called by startup code
// to get the ammount of memory to malloc
// for the zone management.
byte *I_ZoneBase(int *size);

// Called by D_DoomLoop,
// returns current time in tics.
int I_GetTime(void);

// Called by D_DoomLoop,
// called before processing any tics in a frame
// (just after displaying a frame).
// Time consuming synchronous operations are performed here.
void I_StartFrame(void);

// Called by D_DoomLoop,
// called before processing each tic in a frame.
// Polls platform input into InputHandler.
void I_StartTic();

// Called when quit is selected.
// Clean exit, displays sell blurb.
void I_Quit(void);

// Releases initialized platform resources without terminating the process.
void I_Shutdown(void);

#ifdef DOOM_DEBUG
bool I_RunTimerTest(void);
#endif

// Allocates from low memory under dos,
// just mallocs under unix
byte *I_AllocLow(int length);

void I_Tactile(int on, int off, int total);

void I_Error(char *error, ...);
