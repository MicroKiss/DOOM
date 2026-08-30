
// DESCRIPTION:
//	Player related stuff.
//	Bobbing POV/weapon, movement.
//	Pending weapon.
//-----------------------------------------------------------------------------

#include "Inputs/InputHandler.hpp"
#include "PlaySimulation/local.hpp"
#include "doomstat.hpp"

#include "doomstat.hpp"

// Index of the special effects (INVUL inverse) map.
#define INVERSECOLORMAP 32

// Movement.

// 16 pixels of bob
#define MAXBOB 0x100000

int32_t forwardWalkSpeed = 25;
int32_t forwardRunSpeed = 50;
int32_t sideWalkSpeed = 24;
int32_t sideRunSpeed = 40;

bool onground;

// P_Thrust
// Moves the given origin along a given angle.
void P_Thrust(player_t *player,
              angle_t angle,
              int32_t move)
{
    angle >>= ANGLETOFINESHIFT;

    player->mo->momx += FixedMul(move, finecosine[angle]);
    player->mo->momy += FixedMul(move, finesine[angle]);
}

// P_CalcHeight
// Calculate the walking / running height adjustment
void P_CalcHeight(player_t *player)
{
    int angle;
    int32_t bob;

    // Regular movement bobbing
    // (needs to be calculated for gun swing
    // even if not on ground)
    // OPTIMIZE: tablify angle
    // Note: a LUT allows for effects
    //  like a ramp with low health.
    player->bob =
        FixedMul(player->mo->momx, player->mo->momx) + FixedMul(player->mo->momy, player->mo->momy);

    player->bob >>= 2;

    if (player->bob > MAXBOB)
        player->bob = MAXBOB;

    if ((player->cheats & CF_NOMOMENTUM) || !onground)
    {
        player->viewz = player->mo->z + VIEWHEIGHT;

        if (player->viewz > player->mo->ceilingz - 4 * FRACUNIT)
            player->viewz = player->mo->ceilingz - 4 * FRACUNIT;

        player->viewz = player->mo->z + player->viewheight;
        return;
    }

    angle = (FINEANGLES / 20 * leveltime) & FINEMASK;
    bob = FixedMul(player->bob / 2, finesine[angle]);

    // move viewheight
    if (player->playerstate == PST_LIVE)
    {
        player->viewheight += player->deltaviewheight;

        if (player->viewheight > VIEWHEIGHT)
        {
            player->viewheight = VIEWHEIGHT;
            player->deltaviewheight = 0;
        }

        if (player->viewheight < VIEWHEIGHT / 2)
        {
            player->viewheight = VIEWHEIGHT / 2;
            if (player->deltaviewheight <= 0)
                player->deltaviewheight = 1;
        }

        if (player->deltaviewheight)
        {
            player->deltaviewheight += FRACUNIT / 4;
            if (!player->deltaviewheight)
                player->deltaviewheight = 1;
        }
    }
    player->viewz = player->mo->z + player->viewheight + bob;

    if (player->viewz > player->mo->ceilingz - 4 * FRACUNIT)
        player->viewz = player->mo->ceilingz - 4 * FRACUNIT;
}

// P_MovePlayer
void P_MovePlayer(player_t *player)
{
    const MouseMotion &mouse = inputHandler.GetMouseMotion();
    int64_t mouseTurn = static_cast<int64_t>(mouse.x) * (mouseSensitivity + 5) * 4096 * 4;
    player->mo->angle -= static_cast<angle_t>(mouseTurn);

    // Do not let the player control movement
    //  if not onground.
    onground = (player->mo->z <= player->mo->floorz);

    if (onground)
    {
        int32_t forwardDir = inputHandler.IsDown(INPUTS::MOVE_FORWARD) - inputHandler.IsDown(INPUTS::MOVE_BACKWARD);
        int32_t forwardMovement = forwardDir * (inputHandler.IsDown(INPUTS::MOVE_SPRINT) ? forwardRunSpeed : forwardWalkSpeed);

        int32_t sideDir = inputHandler.IsDown(INPUTS::MOVE_RIGHT) - inputHandler.IsDown(INPUTS::MOVE_LEFT);
        int32_t sideMovement = sideDir * (inputHandler.IsDown(INPUTS::MOVE_SPRINT) ? sideRunSpeed : sideWalkSpeed);

        if (forwardMovement)
            P_Thrust(player, player->mo->angle, forwardMovement * 2048);

        if (sideMovement)
            P_Thrust(player, player->mo->angle - ANG90, sideMovement * 2048);

        if ((forwardMovement || sideMovement) && player->mo->state == &states[S_PLAY])
        {
            P_SetMobjState(player->mo, S_PLAY_RUN1);
        }

        // jump
        if (inputHandler.IsDown(INPUTS::JUMP))
        {
            auto jumpHeight = GRAVITY * 35;
            player->mo->z += jumpHeight;
        }
    }
}

// P_DeathThink
// Fall on your face when dying.
// Decrease POV height to floor height.
#define ANG5 (ANG90 / 18)

void P_DeathThink(player_t *player)
{
    angle_t angle;
    angle_t delta;

    P_MovePsprites(player);

    // fall to the ground
    if (player->viewheight > 6 * FRACUNIT)
        player->viewheight -= FRACUNIT;

    if (player->viewheight < 6 * FRACUNIT)
        player->viewheight = 6 * FRACUNIT;

    player->deltaviewheight = 0;
    onground = (player->mo->z <= player->mo->floorz);
    P_CalcHeight(player);

    if (player->attacker && player->attacker != player->mo)
    {
        angle = R_PointToAngle2(player->mo->x,
                                player->mo->y,
                                player->attacker->x,
                                player->attacker->y);

        delta = angle - player->mo->angle;

        if (delta < ANG5 || delta > (unsigned)-ANG5)
        {
            // Looking at killer,
            //  so fade damage flash down.
            player->mo->angle = angle;

            if (player->damagecount)
                player->damagecount--;
        }
        else if (delta < ANG180)
            player->mo->angle += ANG5;
        else
            player->mo->angle -= ANG5;
    }
    else if (player->damagecount)
        player->damagecount--;

    if (inputHandler.IsDown(INPUTS::USE))
        player->playerstate = PST_REBORN;
}

WeaponType NextAvailableWeapon(player_t *player)
{
    WeaponType current = player->readyweapon;
    WeaponType next = static_cast<WeaponType>((static_cast<int>(current) + 1) % NUMWEAPONS);

    while (!player->weaponowned[next])
    {
        next = static_cast<WeaponType>((static_cast<int>(next) + 1) % NUMWEAPONS);
    }
    return next;
}

WeaponType PrevAvailableWeapon(player_t *player)
{
    WeaponType current = player->readyweapon;
    WeaponType prev = static_cast<WeaponType>((static_cast<int>(current) - 1 + NUMWEAPONS) % NUMWEAPONS);

    while (!player->weaponowned[prev])
    {
        prev = static_cast<WeaponType>((static_cast<int>(prev) - 1 + NUMWEAPONS) % NUMWEAPONS);
    }
    return prev;
}

// P_PlayerThink
void P_PlayerThink(player_t *player)
{
    WeaponType newweapon;

    // fixme: do this in the cheat code
    if (player->cheats & CF_NOCLIP)
        player->mo->flags |= MF_NOCLIP;
    else
        player->mo->flags &= ~MF_NOCLIP;

    // chain saw run forward
    if (player->mo->flags & MF_JUSTATTACKED)
    {
        if (player->mo->z <= player->mo->floorz)
            P_Thrust(player, player->mo->angle, (0xc800 / 512) * 2048);
        player->mo->flags &= ~MF_JUSTATTACKED;
    }

    if (player->playerstate == PST_DEAD)
    {
        P_DeathThink(player);
        return;
    }

    // Move around.
    // Reactiontime is used to prevent movement
    //  for a bit after a teleport.
    if (player->mo->reactiontime)
    {
        player->mo->reactiontime--;
    }
    else
    {
        P_MovePlayer(player);
    }

    P_CalcHeight(player);

    if (player->mo->subsector->sector->special)
        P_PlayerInSpecialSector(player);

    // Check for weapon change.

    if (inputHandler.IsPressed(INPUTS::NEXT_WEAPON))
    {
        player->pendingweapon = NextAvailableWeapon(player);
    }

    if (inputHandler.IsPressed(INPUTS::PREV_WEAPON))
    {
        player->pendingweapon = PrevAvailableWeapon(player);
    }

    newweapon = wp_nochange;
    for (int weapon = 0; weapon < NUMWEAPONS - 1; ++weapon)
    {
        INPUTS weaponInput = static_cast<INPUTS>(INPUTS::WEAPON_1 + weapon);
        if (inputHandler.IsPressed(weaponInput))
        {
            newweapon = static_cast<WeaponType>(weapon);
            break;
        }
    }

    if (newweapon != wp_nochange)
    {
        // The actual changing of the weapon is done
        //  when the weapon psprite can do it
        //  (read: not in the middle of an attack).
        if (newweapon == wp_fist && player->weaponowned[wp_chainsaw] && !(player->readyweapon == wp_chainsaw && player->powers[pw_strength]))
        {
            newweapon = wp_chainsaw;
        }

        if ((gamemode == commercial) && newweapon == wp_shotgun && player->weaponowned[wp_supershotgun] && player->readyweapon != wp_supershotgun)
        {
            newweapon = wp_supershotgun;
        }

        if (player->weaponowned[newweapon] && newweapon != player->readyweapon)
        {

            player->pendingweapon = newweapon;
        }
    }

    // check for use
    if (inputHandler.IsDown(INPUTS::USE))
    {
        if (!player->usedown)
        {
            P_UseLines(player);
            player->usedown = true;
        }
    }
    else
        player->usedown = false;

    // cycle psprites
    P_MovePsprites(player);

    // Counters, time dependend power ups.

    // Strength counts up to diminish fade.
    if (player->powers[pw_strength])
        player->powers[pw_strength]++;

    if (player->powers[pw_invulnerability])
        player->powers[pw_invulnerability]--;

    if (player->powers[pw_invisibility])
        if (!--player->powers[pw_invisibility])
            player->mo->flags &= ~MF_SHADOW;

    if (player->powers[pw_infrared])
        player->powers[pw_infrared]--;

    if (player->powers[pw_ironfeet])
        player->powers[pw_ironfeet]--;

    if (player->damagecount)
        player->damagecount--;

    if (player->bonuscount)
        player->bonuscount--;

    // Handling colormaps.
    if (player->powers[pw_invulnerability])
    {
        if (player->powers[pw_invulnerability] > 4 * 32 || (player->powers[pw_invulnerability] & 8))
            player->fixedcolormap = INVERSECOLORMAP;
        else
            player->fixedcolormap = 0;
    }
    else if (player->powers[pw_infrared])
    {
        if (player->powers[pw_infrared] > 4 * 32 || (player->powers[pw_infrared] & 8))
        {
            // almost full bright
            player->fixedcolormap = 1;
        }
        else
            player->fixedcolormap = 0;
    }
    else
        player->fixedcolormap = 0;
}
