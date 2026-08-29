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
//	Refresh/render internal state variables (global).
//-----------------------------------------------------------------------------

#ifndef __R_STATE__
#define __R_STATE__

// Need data structure definitions.
#include "Doom/player.hpp"
#include "Renderer/data.hpp"

#ifdef __GNUG__
#pragma interface
#endif


        // Refresh internal data structures,
    //  for rendering.
    
    // needed for texture pegging
    extern int32_t *textureheight;

    // needed for pre rendering (fracs)
    extern int32_t *spritewidth;

    extern int32_t *spriteoffset;
    extern int32_t *spritetopoffset;

    extern lighttable_t *colormaps;

    extern int viewwidth;
    extern int scaledviewwidth;
    extern int viewheight;

    extern int firstflat;

    // for global animation
    extern int *flattranslation;
    extern int *texturetranslation;

    // Sprite....
    extern int firstspritelump;
    extern int lastspritelump;
    extern int numspritelumps;

        // Lookup tables for map data.
        extern int numsprites;
    extern spritedef_t *sprites;

    extern int numvertexes;
    extern vertex_t *vertexes;

    extern int numsegs;
    extern seg_t *segs;

    extern int numsectors;
    extern sector_t *sectors;

    extern int numsubsectors;
    extern subsector_t *subsectors;

    extern int numnodes;
    extern node_t *nodes;

    extern int numlines;
    extern line_t *lines;

    extern int numsides;
    extern side_t *sides;

        // POV data.
        extern int32_t viewx;
    extern int32_t viewy;
    extern int32_t viewz;

    extern angle_t viewangle;
    extern player_t *viewplayer;

    // ?
    extern angle_t clipangle;

    extern int viewangletox[FINEANGLES / 2];
    extern angle_t xtoviewangle[SCREENWIDTH + 1];
    // extern int32_t		finetangent[FINEANGLES/2];

    extern int32_t rw_distance;
    extern angle_t rw_normalangle;

    // angle to line origin
    extern int rw_angle1;

    // Segs count?
    extern int sscount;

    extern visplane_t *floorplane;
    extern visplane_t *ceilingplane;


#endif
//-----------------------------------------------------------------------------
// $Log:$
//-----------------------------------------------------------------------------
