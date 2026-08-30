// DESCRIPTION:
//	Single-player game tick scheduling.
//-----------------------------------------------------------------------------

#include "Game/game.hpp"
#include "Miscellaneous/menu.hpp"
#include "SystemInterface/system.hpp"
#include "doomdef.hpp"
#include "doomstat.hpp"

doomcom_t *doomcom;

int maketic;
int ticdup = 1;

void D_ProcessEvents(void);

void NetUpdate(void)
{
}

void D_CheckNetGame(void)
{
    consoleplayer = 0;
    displayplayer = 0;
    playeringame[0] = true;
    netgame = false;
}

void D_QuitNetGame(void)
{
    if (debugfile)
        fclose(debugfile);
}

void TryRunTics(void)
{
    static int oldentertics;
    int entertic = I_GetTime();

    if (!oldentertics)
        oldentertics = entertic - 1;

    int counts = entertic - oldentertics;
    oldentertics = entertic;

    if (counts < 1)
        counts = 1;

    while (counts--)
    {
        I_StartTic();
        D_ProcessEvents();
        M_Ticker();
        G_Ticker();
        gametic++;
        maketic++;
    }
}