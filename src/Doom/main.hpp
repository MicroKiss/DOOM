
// DESCRIPTION:
//	System specific interface stuff.
//-----------------------------------------------------------------------------

#ifndef __D_MAIN__
#define __D_MAIN__

#include "Doom/event.hpp"

#ifdef __GNUG__
#pragma interface
#endif

#define MAXWADFILES 20
extern char *wadfiles[MAXWADFILES];

void D_AddFile(char *file);

// D_DoomMain()
// Not a globally visible function, just included for source reference,
// calls all startup code, parses command line options.
void D_DoomMain(void);

// Called by IO functions when input is detected.
void D_PostEvent(event_t *ev);

void D_StartTitle(void);

#endif
