#pragma once
#include "doomtype.h"

#include "doomdef.h"
#include "m_bbox.hpp"

// Needed because we are refering to patches.
#include "r_data.hpp"

#define CENTERY (SCREENHEIGHT / 2)

// Screen 0 is the screen updated by I_Update screen.
// Screen 1 is an extra buffer.

#ifdef __cplusplus
extern "C"
{
#endif

    extern byte *screens[5];

    extern BBox dirtybox;

    extern byte gammatable[5][256];
    extern int usegamma;

    // Allocates buffer screens, call before R_Init.
    void V_Init(void);

    void V_CopyRect(int srcx, int srcy, int srcscrn, int width, int height, int destx, int desty, int destscrn);

    void V_DrawPatch(int x, int y, int scrn, patch_t *patch);

    void V_DrawPatchFlipped(int x, int y, int scrn, patch_t *patch);

    void V_DrawPatchDirect(int x, int y, int scrn, patch_t *patch);

    // Draw a linear block of pixels into the view buffer.
    void V_DrawBlock(int x, int y, int scrn, int width, int height, byte *src);

    // Reads a linear block of pixels into the view buffer.
    void V_GetBlock(int x, int y, int scrn, int width, int height, byte *dest);

    void V_MarkRect(int x, int y, int width, int height);

#ifdef __cplusplus
}
#endif