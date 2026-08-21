#include "doomdef.h"

#include <string.h>

#include "m_argv.h"
#include "d_main.h"
#include "i_system.h"
#include "i_video.h"
#include "v_video.h"

int TimerTest()
{
    boolean passed;
    I_Init();
    passed = I_RunTimerTest();
    I_Shutdown();
    return passed ? 0 : 1;
}

int VideoTest()
{
    byte palette[256 * 3];
    int color;
    int pixel;

    for (color = 0; color < 256; ++color)
    {
        palette[color * 3] = (byte)color;
        palette[color * 3 + 1] = (byte)(255 - color);
        palette[color * 3 + 2] = (byte)(color / 2);
    }

    I_Init();
    V_Init();
    I_InitGraphics();
    I_SetPalette(palette);

    for (pixel = 0; pixel < SCREENWIDTH * SCREENHEIGHT; ++pixel)
        screens[0][pixel] = (byte)(pixel % 256);

    I_FinishUpdate();
    I_Shutdown();
    return 0;
}

int TestMain()
{

    for (int i = 1; i < myargc; ++i)
    {
        if (!strcmp(myargv[i], "-timertest"))
        {
            return TimerTest();
        }

        if (!strcmp(myargv[i], "-videotest"))
            return VideoTest();
    }
    return -1;
}
