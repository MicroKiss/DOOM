
// We are referring to sprite numbers.
#include "Doom/WeaponInfo.hpp"
#include "info.hpp"

// PSPRITE ACTIONS for waepons.
// This struct controls the weapon animations.
// Each entry is:
//   ammo/amunition type
//  upstate
//  downstate
// readystate
// atkstate, i.e. attack/fire/hit frame
// flashstate, muzzle flash

WeaponInfo fist = {
    am_noammo,
    S_PUNCHUP,
    S_PUNCHDOWN,
    S_PUNCH,
    S_PUNCH1,
    S_NULL
};

WeaponInfo pistol = {
    am_clip,
    S_PISTOLUP,
    S_PISTOLDOWN,
    S_PISTOL,
    S_PISTOL1,
    S_PISTOLFLASH
};

WeaponInfo shotgun = {
    am_shell,
    S_SGUNUP,
    S_SGUNDOWN,
    S_SGUN,
    S_SGUN1,
    S_SGUNFLASH1
};

WeaponInfo chaingun = {
    am_clip,
    S_CHAINUP,
    S_CHAINDOWN,
    S_CHAIN,
    S_CHAIN1,
    S_CHAINFLASH1
};

WeaponInfo missilelauncher = {
    am_misl,
    S_MISSILEUP,
    S_MISSILEDOWN,
    S_MISSILE,
    S_MISSILE1,
    S_MISSILEFLASH1
};

WeaponInfo plasmaRifle = {
    am_cell,
    S_PLASMAUP,
    S_PLASMADOWN,
    S_PLASMA,
    S_PLASMA1,
    S_PLASMAFLASH1
};

WeaponInfo bfg9000 = {
    am_cell,
    S_BFGUP,
    S_BFGDOWN,
    S_BFG,
    S_BFG1,
    S_BFGFLASH1
};

WeaponInfo chainsaw = {
    am_noammo,
    S_SAWUP,
    S_SAWDOWN,
    S_SAW,
    S_SAW1,
    S_NULL
};

WeaponInfo superShotgun = {
    am_shell,
    S_DSGUNUP,
    S_DSGUNDOWN,
    S_DSGUN,
    S_DSGUN1,
    S_DSGUNFLASH1
};

WeaponInfo weaponinfo[NUMWEAPONS] = {
    fist,
    pistol,
    shotgun,
    chaingun,
    missilelauncher,
    plasmaRifle,
    bfg9000,
    chainsaw,
    superShotgun,
};
