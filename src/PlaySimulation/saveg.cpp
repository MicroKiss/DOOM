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
// $Log:$
//
// DESCRIPTION:
//	Archiving: SaveGame I/O.
//
//-----------------------------------------------------------------------------

#include "SystemInterface/system.hpp"
#include "ZoneMemory/zone.hpp"
#include "PlaySimulation/local.hpp"
#include "PlaySimulation/saveg.hpp"

// State.
#include "doomstat.hpp"
#include "Renderer/state.hpp"

byte *save_p;

static uint8_t SaveRead8(void)
{
	return *save_p++;
}

static int16_t SaveRead16(void)
{
	uint16_t value = SaveRead8();
	value |= (uint16_t)SaveRead8() << 8;
	return (int16_t)value;
}

static int32_t SaveRead32(void)
{
	uint32_t value = SaveRead8();
	value |= (uint32_t)SaveRead8() << 8;
	value |= (uint32_t)SaveRead8() << 16;
	value |= (uint32_t)SaveRead8() << 24;
	return (int32_t)value;
}

static void SaveWrite8(uint8_t value)
{
	*save_p++ = value;
}

static void SaveWrite16(int16_t value)
{
	uint16_t bits = (uint16_t)value;
	SaveWrite8((uint8_t)bits);
	SaveWrite8((uint8_t)(bits >> 8));
}

static void SaveWrite32(int32_t value)
{
	uint32_t bits = (uint32_t)value;
	SaveWrite8((uint8_t)bits);
	SaveWrite8((uint8_t)(bits >> 8));
	SaveWrite8((uint8_t)(bits >> 16));
	SaveWrite8((uint8_t)(bits >> 24));
}

// Pads save_p to a 4-byte boundary
//  so that the load/save works on SGI&Gecko.
#define PADSAVEP() save_p += (4 - ((uintptr_t)save_p & 3u)) & 3u

//
// P_ArchivePlayers
//
void P_ArchivePlayers(void)
{
	int i;
	int j;
	player_t *player;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		if (!playeringame[i])
			continue;

		PADSAVEP();
		player = &players[i];
		SaveWrite32(0);
		SaveWrite32(player->playerstate);
		SaveWrite8(player->cmd.forwardmove);
		SaveWrite8(player->cmd.sidemove);
		SaveWrite16(player->cmd.angleturn);
		SaveWrite16(player->cmd.consistancy);
		SaveWrite8(player->cmd.chatchar);
		SaveWrite8(player->cmd.buttons);
		SaveWrite32(player->viewz);
		SaveWrite32(player->viewheight);
		SaveWrite32(player->deltaviewheight);
		SaveWrite32(player->bob);
		SaveWrite32(player->health);
		SaveWrite32(player->armorpoints);
		SaveWrite32(player->armortype);
		for (j = 0; j < NUMPOWERS; j++)
			SaveWrite32(player->powers[j]);
		for (j = 0; j < NUMCARDS; j++)
			SaveWrite32(player->cards[j]);
		SaveWrite32(player->backpack);
		for (j = 0; j < MAXPLAYERS; j++)
			SaveWrite32(player->frags[j]);
		SaveWrite32(player->readyweapon);
		SaveWrite32(player->pendingweapon);
		for (j = 0; j < NUMWEAPONS; j++)
			SaveWrite32(player->weaponowned[j]);
		for (j = 0; j < NUMAMMO; j++)
			SaveWrite32(player->ammo[j]);
		for (j = 0; j < NUMAMMO; j++)
			SaveWrite32(player->maxammo[j]);
		SaveWrite32(player->attackdown);
		SaveWrite32(player->usedown);
		SaveWrite32(player->cheats);
		SaveWrite32(player->refire);
		SaveWrite32(player->killcount);
		SaveWrite32(player->itemcount);
		SaveWrite32(player->secretcount);
		SaveWrite32(0);
		SaveWrite32(player->damagecount);
		SaveWrite32(player->bonuscount);
		SaveWrite32(0);
		SaveWrite32(player->extralight);
		SaveWrite32(player->fixedcolormap);
		SaveWrite32(player->colormap);
		for (j = 0; j < NUMPSPRITES; j++)
		{
			SaveWrite32(player->psprites[j].state
							? (int32_t)(player->psprites[j].state - states)
							: 0);
			SaveWrite32(player->psprites[j].tics);
			SaveWrite32(player->psprites[j].sx);
			SaveWrite32(player->psprites[j].sy);
		}
		SaveWrite32(player->didsecret);
	}
}

//
// P_UnArchivePlayers
//
void P_UnArchivePlayers(void)
{
	int i;
	int j;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		if (!playeringame[i])
			continue;

		PADSAVEP();
		player_t *player = &players[i];
		SaveRead32();
		player->playerstate = static_cast<decltype(player->playerstate)>(SaveRead32());
		player->cmd.forwardmove = SaveRead8();
		player->cmd.sidemove = SaveRead8();
		player->cmd.angleturn = SaveRead16();
		player->cmd.consistancy = SaveRead16();
		player->cmd.chatchar = SaveRead8();
		player->cmd.buttons = SaveRead8();
		player->viewz = SaveRead32();
		player->viewheight = SaveRead32();
		player->deltaviewheight = SaveRead32();
		player->bob = SaveRead32();
		player->health = SaveRead32();
		player->armorpoints = SaveRead32();
		player->armortype = SaveRead32();
		for (j = 0; j < NUMPOWERS; j++)
			player->powers[j] = SaveRead32();
		for (j = 0; j < NUMCARDS; j++)
			player->cards[j] = SaveRead32();
		player->backpack = SaveRead32();
		for (j = 0; j < MAXPLAYERS; j++)
			player->frags[j] = SaveRead32();
		player->readyweapon = static_cast<decltype(player->readyweapon)>(SaveRead32());
		player->pendingweapon = static_cast<decltype(player->pendingweapon)>(SaveRead32());
		for (j = 0; j < NUMWEAPONS; j++)
			player->weaponowned[j] = SaveRead32();
		for (j = 0; j < NUMAMMO; j++)
			player->ammo[j] = SaveRead32();
		for (j = 0; j < NUMAMMO; j++)
			player->maxammo[j] = SaveRead32();
		player->attackdown = SaveRead32();
		player->usedown = SaveRead32();
		player->cheats = SaveRead32();
		player->refire = SaveRead32();
		player->killcount = SaveRead32();
		player->itemcount = SaveRead32();
		player->secretcount = SaveRead32();
		SaveRead32();
		player->damagecount = SaveRead32();
		player->bonuscount = SaveRead32();
		SaveRead32();
		player->extralight = SaveRead32();
		player->fixedcolormap = SaveRead32();
		player->colormap = SaveRead32();
		for (j = 0; j < NUMPSPRITES; j++)
		{
			int32_t state = SaveRead32();
			player->psprites[j].state = state ? &states[state] : NULL;
			player->psprites[j].tics = SaveRead32();
			player->psprites[j].sx = SaveRead32();
			player->psprites[j].sy = SaveRead32();
		}
		player->didsecret = SaveRead32();

		// will be set when unarc thinker
		player->mo = NULL;
		player->message = NULL;
		player->attacker = NULL;
	}
}

//
// P_ArchiveWorld
//
void P_ArchiveWorld(void)
{
	int i;
	int j;
	sector_t *sec;
	line_t *li;
	side_t *si;

	// do sectors
	for (i = 0, sec = sectors; i < numsectors; i++, sec++)
	{
		SaveWrite16(sec->floorheight >> FRACBITS);
		SaveWrite16(sec->ceilingheight >> FRACBITS);
		SaveWrite16(sec->floorpic);
		SaveWrite16(sec->ceilingpic);
		SaveWrite16(sec->lightlevel);
		SaveWrite16(sec->special);
		SaveWrite16(sec->tag);
	}

	// do lines
	for (i = 0, li = lines; i < numlines; i++, li++)
	{
		SaveWrite16(li->flags);
		SaveWrite16(li->special);
		SaveWrite16(li->tag);
		for (j = 0; j < 2; j++)
		{
			if (li->sidenum[j] == -1)
				continue;

			si = &sides[li->sidenum[j]];

			SaveWrite16(si->textureoffset >> FRACBITS);
			SaveWrite16(si->rowoffset >> FRACBITS);
			SaveWrite16(si->toptexture);
			SaveWrite16(si->bottomtexture);
			SaveWrite16(si->midtexture);
		}
	}
}

//
// P_UnArchiveWorld
//
void P_UnArchiveWorld(void)
{
	int i;
	int j;
	sector_t *sec;
	line_t *li;
	side_t *si;

	// do sectors
	for (i = 0, sec = sectors; i < numsectors; i++, sec++)
	{
		sec->floorheight = SaveRead16() << FRACBITS;
		sec->ceilingheight = SaveRead16() << FRACBITS;
		sec->floorpic = SaveRead16();
		sec->ceilingpic = SaveRead16();
		sec->lightlevel = SaveRead16();
		sec->special = SaveRead16();
		sec->tag = SaveRead16();
		sec->specialdata = 0;
		sec->soundtarget = 0;
	}

	// do lines
	for (i = 0, li = lines; i < numlines; i++, li++)
	{
		li->flags = SaveRead16();
		li->special = SaveRead16();
		li->tag = SaveRead16();
		for (j = 0; j < 2; j++)
		{
			if (li->sidenum[j] == -1)
				continue;
			si = &sides[li->sidenum[j]];
			si->textureoffset = SaveRead16() << FRACBITS;
			si->rowoffset = SaveRead16() << FRACBITS;
			si->toptexture = SaveRead16();
			si->bottomtexture = SaveRead16();
			si->midtexture = SaveRead16();
		}
	}
}

//
// Thinkers
//
static void SaveWriteThinker(const thinker_t *thinker)
{
	SaveWrite32(0);
	SaveWrite32(0);
	SaveWrite32(thinker->function.acp1 ? 1 : 0);
}

static void SaveReadThinker(thinker_t *thinker)
{
	SaveRead32();
	SaveRead32();
	thinker->prev = NULL;
	thinker->next = NULL;
	thinker->function.acp1 = SaveRead32() ? (actionf_p1)(intptr_t)1 : NULL;
}

static void SaveWriteMobj(const mobj_t *mobj)
{
	SaveWriteThinker(&mobj->thinker);
	SaveWrite32(mobj->x);
	SaveWrite32(mobj->y);
	SaveWrite32(mobj->z);
	SaveWrite32(0);
	SaveWrite32(0);
	SaveWrite32(mobj->angle);
	SaveWrite32(mobj->sprite);
	SaveWrite32(mobj->frame);
	SaveWrite32(0);
	SaveWrite32(0);
	SaveWrite32(0);
	SaveWrite32(mobj->floorz);
	SaveWrite32(mobj->ceilingz);
	SaveWrite32(mobj->radius);
	SaveWrite32(mobj->height);
	SaveWrite32(mobj->momx);
	SaveWrite32(mobj->momy);
	SaveWrite32(mobj->momz);
	SaveWrite32(mobj->validcount);
	SaveWrite32(mobj->type);
	SaveWrite32(0);
	SaveWrite32(mobj->tics);
	SaveWrite32((int32_t)(mobj->state - states));
	SaveWrite32(mobj->flags);
	SaveWrite32(mobj->health);
	SaveWrite32(mobj->movedir);
	SaveWrite32(mobj->movecount);
	SaveWrite32(0);
	SaveWrite32(mobj->reactiontime);
	SaveWrite32(mobj->threshold);
	SaveWrite32(mobj->player ? (int32_t)(mobj->player - players + 1) : 0);
	SaveWrite32(mobj->lastlook);
	SaveWrite16(mobj->spawnpoint.x);
	SaveWrite16(mobj->spawnpoint.y);
	SaveWrite16(mobj->spawnpoint.angle);
	SaveWrite16(mobj->spawnpoint.type);
	SaveWrite16(mobj->spawnpoint.options);
	SaveWrite32(0);
}

static void SaveReadMobj(mobj_t *mobj)
{
	int32_t playernum;
	SaveReadThinker(&mobj->thinker);
	mobj->x = SaveRead32();
	mobj->y = SaveRead32();
	mobj->z = SaveRead32();
	SaveRead32();
	SaveRead32();
	mobj->snext = mobj->sprev = NULL;
	mobj->angle = SaveRead32();
	mobj->sprite = static_cast<decltype(mobj->sprite)>(SaveRead32());
	mobj->frame = SaveRead32();
	SaveRead32();
	SaveRead32();
	SaveRead32();
	mobj->bnext = mobj->bprev = NULL;
	mobj->subsector = NULL;
	mobj->floorz = SaveRead32();
	mobj->ceilingz = SaveRead32();
	mobj->radius = SaveRead32();
	mobj->height = SaveRead32();
	mobj->momx = SaveRead32();
	mobj->momy = SaveRead32();
	mobj->momz = SaveRead32();
	mobj->validcount = SaveRead32();
	mobj->type = static_cast<decltype(mobj->type)>(SaveRead32());
	SaveRead32();
	mobj->info = NULL;
	mobj->tics = SaveRead32();
	mobj->state = &states[SaveRead32()];
	mobj->flags = SaveRead32();
	mobj->health = SaveRead32();
	mobj->movedir = SaveRead32();
	mobj->movecount = SaveRead32();
	SaveRead32();
	mobj->target = NULL;
	mobj->reactiontime = SaveRead32();
	mobj->threshold = SaveRead32();
	playernum = SaveRead32();
	mobj->player = playernum ? &players[playernum - 1] : NULL;
	if (mobj->player)
		mobj->player->mo = mobj;
	mobj->lastlook = SaveRead32();
	mobj->spawnpoint.x = SaveRead16();
	mobj->spawnpoint.y = SaveRead16();
	mobj->spawnpoint.angle = SaveRead16();
	mobj->spawnpoint.type = SaveRead16();
	mobj->spawnpoint.options = SaveRead16();
	SaveRead32();
	mobj->tracer = NULL;
}

typedef enum
{
	tc_end,
	tc_mobj

} thinkerclass_t;

//
// P_ArchiveThinkers
//
void P_ArchiveThinkers(void)
{
	thinker_t *th;

	// save off the current thinkers
	for (th = thinkercap.next; th != &thinkercap; th = th->next)
	{
		if (th->function.acp1 == (actionf_p1)P_MobjThinker)
		{
			SaveWrite8(tc_mobj);
			PADSAVEP();
			SaveWriteMobj((mobj_t *)th);
			continue;
		}

		// I_Error ("P_ArchiveThinkers: Unknown thinker function");
	}

	// add a terminating marker
	SaveWrite8(tc_end);
}

//
// P_UnArchiveThinkers
//
void P_UnArchiveThinkers(void)
{
	byte tclass;
	thinker_t *currentthinker;
	thinker_t *next;
	mobj_t *mobj;

	// remove all the current thinkers
	currentthinker = thinkercap.next;
	while (currentthinker != &thinkercap)
	{
		next = currentthinker->next;

		if (currentthinker->function.acp1 == (actionf_p1)P_MobjThinker)
			P_RemoveMobj((mobj_t *)currentthinker);
		else
			Z_Free(currentthinker);

		currentthinker = next;
	}
	P_InitThinkers();

	// read in saved thinkers
	while (1)
	{
		tclass = SaveRead8();
		switch (tclass)
		{
		case tc_end:
			return; // end of list

		case tc_mobj:
			PADSAVEP();
			mobj = static_cast<decltype(mobj)>(Z_Malloc(sizeof(*mobj), PU_LEVEL, NULL));
			SaveReadMobj(mobj);
			P_SetThingPosition(mobj);
			mobj->info = &mobjinfo[mobj->type];
			mobj->floorz = mobj->subsector->sector->floorheight;
			mobj->ceilingz = mobj->subsector->sector->ceilingheight;
			mobj->thinker.function.acp1 = (actionf_p1)P_MobjThinker;
			P_AddThinker(&mobj->thinker);
			break;

		default:
			I_Error("Unknown tclass %i in savegame", tclass);
		}
	}
}

//
// P_ArchiveSpecials
//
static void SaveWriteCeiling(const ceiling_t *value)
{
	SaveWriteThinker(&value->thinker);
	SaveWrite32(value->type);
	SaveWrite32((int32_t)(value->sector - sectors));
	SaveWrite32(value->bottomheight);
	SaveWrite32(value->topheight);
	SaveWrite32(value->speed);
	SaveWrite32(value->crush);
	SaveWrite32(value->direction);
	SaveWrite32(value->tag);
	SaveWrite32(value->olddirection);
}

static void SaveReadCeiling(ceiling_t *value)
{
	SaveReadThinker(&value->thinker);
	value->type = static_cast<decltype(value->type)>(SaveRead32());
	value->sector = &sectors[SaveRead32()];
	value->bottomheight = SaveRead32();
	value->topheight = SaveRead32();
	value->speed = SaveRead32();
	value->crush = SaveRead32();
	value->direction = SaveRead32();
	value->tag = SaveRead32();
	value->olddirection = SaveRead32();
}

static void SaveWriteDoor(const vldoor_t *value)
{
	SaveWriteThinker(&value->thinker);
	SaveWrite32(value->type);
	SaveWrite32((int32_t)(value->sector - sectors));
	SaveWrite32(value->topheight);
	SaveWrite32(value->speed);
	SaveWrite32(value->direction);
	SaveWrite32(value->topwait);
	SaveWrite32(value->topcountdown);
}

static void SaveReadDoor(vldoor_t *value)
{
	SaveReadThinker(&value->thinker);
	value->type = static_cast<decltype(value->type)>(SaveRead32());
	value->sector = &sectors[SaveRead32()];
	value->topheight = SaveRead32();
	value->speed = SaveRead32();
	value->direction = SaveRead32();
	value->topwait = SaveRead32();
	value->topcountdown = SaveRead32();
}

static void SaveWriteFloor(const floormove_t *value)
{
	SaveWriteThinker(&value->thinker);
	SaveWrite32(value->type);
	SaveWrite32(value->crush);
	SaveWrite32((int32_t)(value->sector - sectors));
	SaveWrite32(value->direction);
	SaveWrite32(value->newspecial);
	SaveWrite16(value->texture);
	SaveWrite32(value->floordestheight);
	SaveWrite32(value->speed);
}

static void SaveReadFloor(floormove_t *value)
{
	SaveReadThinker(&value->thinker);
	value->type = static_cast<decltype(value->type)>(SaveRead32());
	value->crush = SaveRead32();
	value->sector = &sectors[SaveRead32()];
	value->direction = SaveRead32();
	value->newspecial = SaveRead32();
	value->texture = SaveRead16();
	value->floordestheight = SaveRead32();
	value->speed = SaveRead32();
}

static void SaveWritePlat(const plat_t *value)
{
	SaveWriteThinker(&value->thinker);
	SaveWrite32((int32_t)(value->sector - sectors));
	SaveWrite32(value->speed);
	SaveWrite32(value->low);
	SaveWrite32(value->high);
	SaveWrite32(value->wait);
	SaveWrite32(value->count);
	SaveWrite32(value->status);
	SaveWrite32(value->oldstatus);
	SaveWrite32(value->crush);
	SaveWrite32(value->tag);
	SaveWrite32(value->type);
}

static void SaveReadPlat(plat_t *value)
{
	SaveReadThinker(&value->thinker);
	value->sector = &sectors[SaveRead32()];
	value->speed = SaveRead32();
	value->low = SaveRead32();
	value->high = SaveRead32();
	value->wait = SaveRead32();
	value->count = SaveRead32();
	value->status = static_cast<decltype(value->status)>(SaveRead32());
	value->oldstatus = static_cast<decltype(value->oldstatus)>(SaveRead32());
	value->crush = SaveRead32();
	value->tag = SaveRead32();
	value->type = static_cast<decltype(value->type)>(SaveRead32());
}

static void SaveWriteFlash(const lightflash_t *value)
{
	SaveWriteThinker(&value->thinker);
	SaveWrite32((int32_t)(value->sector - sectors));
	SaveWrite32(value->count);
	SaveWrite32(value->maxlight);
	SaveWrite32(value->minlight);
	SaveWrite32(value->maxtime);
	SaveWrite32(value->mintime);
}

static void SaveReadFlash(lightflash_t *value)
{
	SaveReadThinker(&value->thinker);
	value->sector = &sectors[SaveRead32()];
	value->count = SaveRead32();
	value->maxlight = SaveRead32();
	value->minlight = SaveRead32();
	value->maxtime = SaveRead32();
	value->mintime = SaveRead32();
}

static void SaveWriteStrobe(const strobe_t *value)
{
	SaveWriteThinker(&value->thinker);
	SaveWrite32((int32_t)(value->sector - sectors));
	SaveWrite32(value->count);
	SaveWrite32(value->minlight);
	SaveWrite32(value->maxlight);
	SaveWrite32(value->darktime);
	SaveWrite32(value->brighttime);
}

static void SaveReadStrobe(strobe_t *value)
{
	SaveReadThinker(&value->thinker);
	value->sector = &sectors[SaveRead32()];
	value->count = SaveRead32();
	value->minlight = SaveRead32();
	value->maxlight = SaveRead32();
	value->darktime = SaveRead32();
	value->brighttime = SaveRead32();
}

static void SaveWriteGlow(const glow_t *value)
{
	SaveWriteThinker(&value->thinker);
	SaveWrite32((int32_t)(value->sector - sectors));
	SaveWrite32(value->minlight);
	SaveWrite32(value->maxlight);
	SaveWrite32(value->direction);
}

static void SaveReadGlow(glow_t *value)
{
	SaveReadThinker(&value->thinker);
	value->sector = &sectors[SaveRead32()];
	value->minlight = SaveRead32();
	value->maxlight = SaveRead32();
	value->direction = SaveRead32();
}

enum
{
	tc_ceiling,
	tc_door,
	tc_floor,
	tc_plat,
	tc_flash,
	tc_strobe,
	tc_glow,
	tc_endspecials

} specials_e;

//
// Things to handle:
//
// T_MoveCeiling, (ceiling_t: sector_t * swizzle), - active list
// T_VerticalDoor, (vldoor_t: sector_t * swizzle),
// T_MoveFloor, (floormove_t: sector_t * swizzle),
// T_LightFlash, (lightflash_t: sector_t * swizzle),
// T_StrobeFlash, (strobe_t: sector_t *),
// T_Glow, (glow_t: sector_t *),
// T_PlatRaise, (plat_t: sector_t *), - active list
//
void P_ArchiveSpecials(void)
{
	thinker_t *th;
	ceiling_t *ceiling;
	vldoor_t *door;
	floormove_t *floor;
	plat_t *plat;
	lightflash_t *flash;
	strobe_t *strobe;
	glow_t *glow;
	int i;

	// save off the current thinkers
	for (th = thinkercap.next; th != &thinkercap; th = th->next)
	{
		if (th->function.acv == (actionf_v)NULL)
		{
			for (i = 0; i < MAXCEILINGS; i++)
				if (activeceilings[i] == (ceiling_t *)th)
					break;

			if (i < MAXCEILINGS)
			{
				SaveWrite8(tc_ceiling);
				PADSAVEP();
				SaveWriteCeiling((ceiling_t *)th);
			}
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_MoveCeiling)
		{
			SaveWrite8(tc_ceiling);
			PADSAVEP();
			SaveWriteCeiling((ceiling_t *)th);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_VerticalDoor)
		{
			SaveWrite8(tc_door);
			PADSAVEP();
			SaveWriteDoor((vldoor_t *)th);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_MoveFloor)
		{
			SaveWrite8(tc_floor);
			PADSAVEP();
			SaveWriteFloor((floormove_t *)th);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_PlatRaise)
		{
			SaveWrite8(tc_plat);
			PADSAVEP();
			SaveWritePlat((plat_t *)th);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_LightFlash)
		{
			SaveWrite8(tc_flash);
			PADSAVEP();
			SaveWriteFlash((lightflash_t *)th);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_StrobeFlash)
		{
			SaveWrite8(tc_strobe);
			PADSAVEP();
			SaveWriteStrobe((strobe_t *)th);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_Glow)
		{
			SaveWrite8(tc_glow);
			PADSAVEP();
			SaveWriteGlow((glow_t *)th);
			continue;
		}
	}

	// add a terminating marker
	SaveWrite8(tc_endspecials);
}

//
// P_UnArchiveSpecials
//
void P_UnArchiveSpecials(void)
{
	byte tclass;
	ceiling_t *ceiling;
	vldoor_t *door;
	floormove_t *floor;
	plat_t *plat;
	lightflash_t *flash;
	strobe_t *strobe;
	glow_t *glow;

	// read in saved thinkers
	while (1)
	{
		tclass = SaveRead8();
		switch (tclass)
		{
		case tc_endspecials:
			return; // end of list

		case tc_ceiling:
			PADSAVEP();
			ceiling = static_cast<decltype(ceiling)>(Z_Malloc(sizeof(*ceiling), PU_LEVEL, NULL));
			SaveReadCeiling(ceiling);
			ceiling->sector->specialdata = ceiling;

			if (ceiling->thinker.function.acp1)
				ceiling->thinker.function.acp1 = (actionf_p1)T_MoveCeiling;

			P_AddThinker(&ceiling->thinker);
			P_AddActiveCeiling(ceiling);
			break;

		case tc_door:
			PADSAVEP();
			door = static_cast<decltype(door)>(Z_Malloc(sizeof(*door), PU_LEVEL, NULL));
			SaveReadDoor(door);
			door->sector->specialdata = door;
			door->thinker.function.acp1 = (actionf_p1)T_VerticalDoor;
			P_AddThinker(&door->thinker);
			break;

		case tc_floor:
			PADSAVEP();
			floor = static_cast<decltype(floor)>(Z_Malloc(sizeof(*floor), PU_LEVEL, NULL));
			SaveReadFloor(floor);
			floor->sector->specialdata = floor;
			floor->thinker.function.acp1 = (actionf_p1)T_MoveFloor;
			P_AddThinker(&floor->thinker);
			break;

		case tc_plat:
			PADSAVEP();
			plat = static_cast<decltype(plat)>(Z_Malloc(sizeof(*plat), PU_LEVEL, NULL));
			SaveReadPlat(plat);
			plat->sector->specialdata = plat;

			if (plat->thinker.function.acp1)
				plat->thinker.function.acp1 = (actionf_p1)T_PlatRaise;

			P_AddThinker(&plat->thinker);
			P_AddActivePlat(plat);
			break;

		case tc_flash:
			PADSAVEP();
			flash = static_cast<decltype(flash)>(Z_Malloc(sizeof(*flash), PU_LEVEL, NULL));
			SaveReadFlash(flash);
			flash->thinker.function.acp1 = (actionf_p1)T_LightFlash;
			P_AddThinker(&flash->thinker);
			break;

		case tc_strobe:
			PADSAVEP();
			strobe = static_cast<decltype(strobe)>(Z_Malloc(sizeof(*strobe), PU_LEVEL, NULL));
			SaveReadStrobe(strobe);
			strobe->thinker.function.acp1 = (actionf_p1)T_StrobeFlash;
			P_AddThinker(&strobe->thinker);
			break;

		case tc_glow:
			PADSAVEP();
			glow = static_cast<decltype(glow)>(Z_Malloc(sizeof(*glow), PU_LEVEL, NULL));
			SaveReadGlow(glow);
			glow->thinker.function.acp1 = (actionf_p1)T_Glow;
			P_AddThinker(&glow->thinker);
			break;

		default:
			I_Error("P_UnarchiveSpecials:Unknown tclass %i "
					"in savegame",
					tclass);
		}
	}
}
