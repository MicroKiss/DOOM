#pragma once
#include "doomtype.h"

#ifdef __cplusplus
extern "C"
{
#endif
    boolean M_WriteFile(char const *name, void *source, int length);

    int M_ReadFile(char const *name, byte **buffer);

    void M_ScreenShot(void);

    void M_LoadDefaults(void);

    void M_SaveDefaults(void);

    int
    M_DrawText(int x,
               int y,
               boolean direct,
               char *string);

#ifdef __cplusplus
}
#endif