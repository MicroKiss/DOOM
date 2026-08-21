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
//     Temporary networking platform stubs.
//
//-----------------------------------------------------------------------------

#include <string.h>

#include "doomstat.h"
#include "d_net.h"
#include "i_net.h"
#include "i_system.h"

static doomcom_t singleplayer_doomcom;

void I_InitNetwork(void)
{
    memset(&singleplayer_doomcom, 0, sizeof(singleplayer_doomcom));

    singleplayer_doomcom.id = DOOMCOM_ID;
    singleplayer_doomcom.remotenode = -1;
    singleplayer_doomcom.numnodes = 1;
    singleplayer_doomcom.ticdup = 1;
    singleplayer_doomcom.consoleplayer = 0;
    singleplayer_doomcom.numplayers = 1;

    doomcom = &singleplayer_doomcom;
    netgame = false;
}

void I_NetCmd(void)
{
    I_Error("I_NetCmd called while networking is disabled");
}