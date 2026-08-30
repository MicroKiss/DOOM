// DESCRIPTION:
//     Single-player game tick scheduling.
//-----------------------------------------------------------------------------

#include "Doom/tick.hpp"
#include "Game/game.hpp"
#include "Miscellaneous/menu.hpp"
#include "SystemInterface/system.hpp"
#include "doomstat.hpp"

void TryRunTics(void)
{
    static int previousEnterTicks;
    static bool initialized;
    int enterTic = I_GetTime();

    if (!initialized)
    {
        previousEnterTicks = enterTic;
        initialized = true;
        return;
    }

    int counts = enterTic - previousEnterTicks;
    if (counts < 1)
        return;

    previousEnterTicks = enterTic;

    while (counts--)
    {
        I_StartTic();
        M_Ticker();
        G_Ticker();
        gametic++;
    }
}
