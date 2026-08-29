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
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	Play functions, animation, global header.
//
//-----------------------------------------------------------------------------

#ifndef __P_LOCAL__
#define __P_LOCAL__

#ifndef __R_LOCAL__
#include "Renderer/local.hpp"
#endif


#define FLOATSPEED (FRACUNIT * 4)

#define MAXHEALTH 100
#define VIEWHEIGHT (41 * FRACUNIT)

// mapblocks are used to check movement
// against lines and things
#define MAPBLOCKUNITS 128
#define MAPBLOCKSIZE (MAPBLOCKUNITS * FRACUNIT)
#define MAPBLOCKSHIFT (FRACBITS + 7)
#define MAPBMASK (MAPBLOCKSIZE - 1)
#define MAPBTOFRAC (MAPBLOCKSHIFT - FRACBITS)

// player radius for movement checking
#define PLAYERRADIUS 16 * FRACUNIT

// MAXRADIUS is for precalculated sector block boxes
// the spider demon is larger,
// but we do not have any moving sectors nearby
#define MAXRADIUS 32 * FRACUNIT

#define GRAVITY FRACUNIT
#define MAXMOVE (30 * FRACUNIT)

#define USERANGE (64 * FRACUNIT)
#define MELEERANGE (64 * FRACUNIT)
#define MISSILERANGE (32 * 64 * FRACUNIT)

// follow a player exlusively for 3 seconds
#define BASETHRESHOLD 100

    //
    // P_TICK
    //

    // both the head and tail of the thinker list
    extern thinker_t thinkercap;

    void P_InitThinkers(void);
    void P_AddThinker(thinker_t *thinker);
    void P_RemoveThinker(thinker_t *thinker);

    //
    // P_PSPR
    //
    void P_SetupPsprites(player_t *curplayer);
    void P_MovePsprites(player_t *curplayer);
    void P_DropWeapon(player_t *player);

    //
    // P_USER
    //
    void P_PlayerThink(player_t *player);

//
// P_MOBJ
//
#define ONFLOORZ MININT
#define ONCEILINGZ MAXINT

// Time interval for item respawning.
#define ITEMQUESIZE 128

    extern mapthing_t itemrespawnque[ITEMQUESIZE];
    extern int itemrespawntime[ITEMQUESIZE];
    extern int iquehead;
    extern int iquetail;

    void P_RespawnSpecials(void);

    mobj_t *
    P_SpawnMobj(int32_t x,
                int32_t y,
                int32_t z,
                mobjtype_t type);

    void P_RemoveMobj(mobj_t *th);
    boolean P_SetMobjState(mobj_t *mobj, statenum_t state);
    void P_MobjThinker(mobj_t *mobj);

    void P_SpawnPuff(int32_t x, int32_t y, int32_t z);
    void P_SpawnBlood(int32_t x, int32_t y, int32_t z, int damage);
    mobj_t *P_SpawnMissile(mobj_t *source, mobj_t *dest, mobjtype_t type);
    void P_SpawnPlayerMissile(mobj_t *source, mobjtype_t type);

    //
    // P_ENEMY
    //
    void P_NoiseAlert(mobj_t *target, mobj_t *emmiter);

    //
    // P_MAPUTL
    //
    typedef struct
    {
        int32_t x;
        int32_t y;
        int32_t dx;
        int32_t dy;

    } divline_t;

    typedef struct
    {
        int32_t frac; // along trace line
        boolean isaline;
        union
        {
            mobj_t *thing;
            line_t *line;
        } d;
    } intercept_t;

#define MAXINTERCEPTS 128

    extern intercept_t intercepts[MAXINTERCEPTS];
    extern intercept_t *intercept_p;

    typedef boolean (*traverser_t)(intercept_t *in);

    int32_t P_AproxDistance(int32_t dx, int32_t dy);
    int P_PointOnLineSide(int32_t x, int32_t y, line_t *line);
    int P_PointOnDivlineSide(int32_t x, int32_t y, divline_t *line);
    void P_MakeDivline(line_t *li, divline_t *dl);
    int32_t P_InterceptVector(divline_t *v2, divline_t *v1);
    int P_BoxOnLineSide(int32_t *tmbox, line_t *ld);

    extern int32_t opentop;
    extern int32_t openbottom;
    extern int32_t openrange;
    extern int32_t lowfloor;

    void P_LineOpening(line_t *linedef);

    boolean P_BlockLinesIterator(int x, int y, boolean (*func)(line_t *));
    boolean P_BlockThingsIterator(int x, int y, boolean (*func)(mobj_t *));

#define PT_ADDLINES 1
#define PT_ADDTHINGS 2
#define PT_EARLYOUT 4

    extern divline_t trace;

    boolean
    P_PathTraverse(int32_t x1,
                   int32_t y1,
                   int32_t x2,
                   int32_t y2,
                   int flags,
                   boolean (*trav)(intercept_t *));

    void P_UnsetThingPosition(mobj_t *thing);
    void P_SetThingPosition(mobj_t *thing);

    //
    // P_MAP
    //

    // If "floatok" true, move would be ok
    // if within "tmfloorz - tmceilingz".
    extern boolean floatok;
    extern int32_t tmfloorz;
    extern int32_t tmceilingz;
    extern line_t *spechit[];
    extern int numspechit;
    extern int32_t attackrange;

    extern line_t *ceilingline;

    boolean P_CheckPosition(mobj_t *thing, int32_t x, int32_t y);
    boolean P_TryMove(mobj_t *thing, int32_t x, int32_t y);
    boolean P_TeleportMove(mobj_t *thing, int32_t x, int32_t y);
    void P_SlideMove(mobj_t *mo);
    boolean P_CheckSight(mobj_t *t1, mobj_t *t2);
    void P_UseLines(player_t *player);

    boolean P_ChangeSector(sector_t *sector, boolean crunch);

    extern mobj_t *linetarget; // who got hit (or NULL)

    int32_t
    P_AimLineAttack(mobj_t *t1,
                    angle_t angle,
                    int32_t distance);

    void P_LineAttack(mobj_t *t1,
                      angle_t angle,
                      int32_t distance,
                      int32_t slope,
                      int damage);

    void P_RadiusAttack(mobj_t *spot,
                        mobj_t *source,
                        int damage);

    //
    // P_SETUP
    //
    extern byte *rejectmatrix;  // for fast sight rejection
    extern short *blockmaplump; // offsets in blockmap are from here
    extern short *blockmap;
    extern int bmapwidth;
    extern int bmapheight; // in mapblocks
    extern int32_t bmaporgx;
    extern int32_t bmaporgy;    // origin of block map
    extern mobj_t **blocklinks; // for thing chains

    //
    // P_INTER
    //
    extern int maxammo[NUMAMMO];
    extern int clipammo[NUMAMMO];

    void P_TouchSpecialThing(mobj_t *special,
                             mobj_t *toucher);

    void P_DamageMobj(mobj_t *target,
                      mobj_t *inflictor,
                      mobj_t *source,
                      int damage);

//
// P_SPEC
//
#include "PlaySimulation/spec.hpp"


#endif // __P_LOCAL__
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
