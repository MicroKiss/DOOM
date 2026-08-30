#pragma once
// DESCRIPTION:
//	Items: key cards, artifacts, weapon, ammunition.
//-----------------------------------------------------------------------------

#include "doomdef.hpp"

struct WeaponInfo
{
    ammotype_t ammo;
    int upstate;
    int downstate;
    int readystate;
    int atkstate;
    int flashstate;
};

extern WeaponInfo weaponinfo[NUMWEAPONS];