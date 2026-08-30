
// DESCRIPTION:  none
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

#include "doomdef.hpp"
#include "doomstat.hpp"

#include "Finale/finale.hpp"
#include "Miscellaneous/argv.hpp"
#include "Miscellaneous/menu.hpp"
#include "Miscellaneous/misc.hpp"
#include "Miscellaneous/random.hpp"
#include "SystemInterface/system.hpp"
#include "ZoneMemory/zone.hpp"

#include "PlaySimulation/saveg.hpp"
#include "PlaySimulation/setup.hpp"
#include "PlaySimulation/tick.hpp"

#include "Doom/main.hpp"

#include "AutoMap/map.hpp"
#include "HeadsUpDisplay/stuff.hpp"
#include "Inputs/InputHandler.hpp"
#include "Intermission/stuff.hpp"
#include "StatusBar/stuff.hpp"

// Needs access to LFB.
#include "Renderer/video.hpp"

#include "Wad/wad.hpp"

#include "PlaySimulation/local.hpp"

#include "Sound/sound.hpp"

// Data.
#include "dstrings.hpp"
#include "sounds.hpp"

// SKY handling - still the wrong place.
#include "Renderer/data.hpp"
#include "Renderer/sky.hpp"

#include "Game/game.hpp"

#define SAVEGAMESIZE 0x2c000
#define SAVESTRINGSIZE 24

void G_PlayerReborn(void);
void G_InitNew(skill_t skill, int episode, int map);

void G_DoReborn(void);

void G_DoLoadLevel(void);
void G_DoNewGame(void);
void G_DoLoadGame(void);
void G_DoCompleted(void);
void G_DoVictory(void);
void G_DoWorldDone(void);
void G_DoSaveGame(void);

gameaction_t gameaction;
gamestate_t gamestate;
skill_t gameskill;
bool respawnmonsters;
int gameepisode;
int gamemap;

bool paused;
bool sendpause; // send a pause event next tic
bool sendsave;  // send a save event next tic

bool viewactive;

player_t gamePlayer;

int gametic;
int levelstarttic;                       // gametic at level start
int totalkills, totalitems, totalsecret; // for intermission

bool precache = true; // if true, load all graphics at start

wbstartstruct_t wminfo; // parms for world map / intermission

byte *savebuffer;

int key_right;
int key_left;
int key_up;
int key_down;
int key_strafeleft;
int key_straferight;
int key_up_alt;
int key_down_alt;
int key_strafeleft_alt;
int key_straferight_alt;
int key_fire;
int key_use;
int key_strafe;
int key_speed;

int mousebfire;
int mousebstrafe;
int mousebforward;
int joybfire;
int joybstrafe;
int joybuse;
int joybspeed;

int dclicktime;
bool dclickstate;
int dclicks;
int dclicktime2;
bool dclickstate2;
int dclicks2;

// joystick values are repeated
int savegameslot;
char savedescription[32];

#define BODYQUESIZE 32

mobj_t *bodyque[BODYQUESIZE];
int bodyqueslot;

void *statcopy; // for statistics driver

// G_DoLoadLevel
extern gamestate_t wipegamestate;

void G_DoLoadLevel(void)
{
    int i;

    // Set the sky map.
    // First thing, we have a dummy sky texture name,
    //  a flat. The data is in the WAD only because
    //  we look for an actual index, instead of simply
    //  setting one.
    skyflatnum = R_FlatNumForName(SKYFLATNAME);

    // DOOM determines the sky texture to be used
    // depending on the current episode, and the game version.
    if ((gamemode == commercial) || (gamemode == pack_tnt) || (gamemode == pack_plut))
    {
        skytexture = R_TextureNumForName("SKY3");
        if (gamemap < 12)
            skytexture = R_TextureNumForName("SKY1");
        else if (gamemap < 21)
            skytexture = R_TextureNumForName("SKY2");
    }

    levelstarttic = gametic; // for time calculation

    if (wipegamestate == GS_LEVEL)
        wipegamestate = static_cast<gamestate_t>(-1); // force a wipe

    gamestate = GS_LEVEL;

    if (gamePlayer.playerstate == PST_DEAD)
        gamePlayer.playerstate = PST_REBORN;

    P_SetupLevel(gameepisode, gamemap, gameskill);
    gameaction = ga_nothing;
    Z_CheckHeap();

    // clear cmd building stuff
    inputHandler.ReleaseAll();
    sendpause = sendsave = paused = false;
}

// G_Ticker
// Advance the game state by one tick.
void G_Ticker(void)
{
    if (!menuactive && inputHandler.IsPressed(INPUTS::PAUSE))
        sendpause = true;

    // do player reborns if needed
    if (gamePlayer.playerstate == PST_REBORN)
        G_DoReborn();

    // do things to change the game state
    while (gameaction != ga_nothing)
    {
        switch (gameaction)
        {
        case ga_loadlevel:
            G_DoLoadLevel();
            break;
        case ga_newgame:
            G_DoNewGame();
            break;
        case ga_loadgame:
            G_DoLoadGame();
            break;
        case ga_savegame:
            G_DoSaveGame();
            break;
        case ga_completed:
            G_DoCompleted();
            break;
        case ga_victory:
            F_StartFinale();
            break;
        case ga_worlddone:
            G_DoWorldDone();
            break;
        case ga_screenshot:
            M_ScreenShot();
            gameaction = ga_nothing;
            break;
        case ga_nothing:
            break;
        }
    }

    if (sendpause)
    {
        sendpause = false;
        paused ^= 1;
        if (paused)
            S_PauseSound();
        else
            S_ResumeSound();
    }

    if (sendsave)
    {
        sendsave = false;
        gameaction = ga_savegame;
    }

    // do main actions
    switch (gamestate)
    {
    case GS_LEVEL:
        P_Ticker();
        ST_Ticker();
        AM_Ticker();
        HU_Ticker();
        break;

    case GS_INTERMISSION:
        WI_Ticker();
        break;

    case GS_FINALE:
        F_Ticker();
        break;

    case GS_MENUSCREEN:
        break;
    }
}

// G_PlayerFinishLevel
// Can when a player completes a level.
void G_PlayerFinishLevel(void)
{
    player_t *p;

    p = &gamePlayer;

    memset(p->powers, 0, sizeof(p->powers));
    memset(p->cards, 0, sizeof(p->cards));
    p->mo->flags &= ~MF_SHADOW; // cancel invisibility
    p->extralight = 0;          // cancel gun flashes
    p->fixedcolormap = 0;       // cancel ir gogles
    p->damagecount = 0;         // no palette changes
    p->bonuscount = 0;
}

// G_PlayerReborn
// Called after a player dies
// almost everything is cleared and initialized
void G_PlayerReborn(void)
{
    player_t *p;

    int killcount;
    int itemcount;
    int secretcount;

    killcount = gamePlayer.killcount;
    itemcount = gamePlayer.itemcount;
    secretcount = gamePlayer.secretcount;

    p = &gamePlayer;
    memset(p, 0, sizeof(*p));

    gamePlayer.killcount = killcount;
    gamePlayer.itemcount = itemcount;
    gamePlayer.secretcount = secretcount;

    p->usedown = true; // don't do anything immediately
    p->attackdown = true;
    p->playerstate = PST_LIVE;
    p->health = MAXHEALTH;
    p->readyweapon = wp_pistol;
    p->pendingweapon = wp_pistol;
    p->weaponowned[wp_fist] = true;
    p->weaponowned[wp_pistol] = true;
    p->ammo[am_clip] = 50;

    for (int i = 0; i < NUMAMMO; i++)
        p->maxammo[i] = maxammo[i];
}

// G_DoReborn
void G_DoReborn(void)
{
    gameaction = ga_loadlevel;
}

void G_ScreenShot(void)
{
    gameaction = ga_screenshot;
}

// DOOM Par Times
int pars[4][10] = {
    { 0 },
    { 0, 30, 75, 120, 90, 165, 180, 180, 30, 165 },
    { 0, 90, 90, 90, 120, 90, 360, 240, 30, 170 },
    { 0, 90, 45, 90, 150, 90, 90, 165, 30, 135 }
};

// DOOM II Par Times
int cpars[32] = {
    30, 90, 120, 120, 90, 150, 120, 120, 270, 90, //  1-10
    210,
    150,
    150,
    150,
    210,
    150,
    420,
    150,
    210,
    150, // 11-20
    240,
    150,
    180,
    150,
    150,
    300,
    330,
    420,
    300,
    180, // 21-30
    120,
    30 // 31-32
};

// G_DoCompleted
bool secretexit;

void G_ExitLevel(void)
{
    secretexit = false;
    gameaction = ga_completed;
}

// Here's for the german edition.
void G_SecretExitLevel(void)
{
    // IF NO WOLF3D LEVELS, NO SECRET EXIT!
    if ((gamemode == commercial) && (W_CheckNumForName("map31") < 0))
        secretexit = false;
    else
        secretexit = true;
    gameaction = ga_completed;
}

void G_DoCompleted(void)
{
    gameaction = ga_nothing;

    G_PlayerFinishLevel(); // take away cards and stuff

    if (automapactive)
        AM_Stop();

    if (gamemode != commercial)
        switch (gamemap)
        {
        case 8:
            gameaction = ga_victory;
            return;
        case 9:
            gamePlayer.didsecret = true;
            break;
        }

    // #if 0  Hmmm - why?
    if ((gamemap == 8) && (gamemode != commercial))
    {
        // victory
        gameaction = ga_victory;
        return;
    }

    if ((gamemap == 9) && (gamemode != commercial))
    {
        // exit secret level
        gamePlayer.didsecret = true;
    }
    // #endif

    wminfo.didsecret = gamePlayer.didsecret;
    wminfo.epsd = gameepisode - 1;
    wminfo.last = gamemap - 1;

    // wminfo.next is 0 biased, unlike gamemap
    if (gamemode == commercial)
    {
        if (secretexit)
            switch (gamemap)
            {
            case 15:
                wminfo.next = 30;
                break;
            case 31:
                wminfo.next = 31;
                break;
            }
        else
            switch (gamemap)
            {
            case 31:
            case 32:
                wminfo.next = 15;
                break;
            default:
                wminfo.next = gamemap;
            }
    }
    else
    {
        if (secretexit)
            wminfo.next = 8; // go to secret level
        else if (gamemap == 9)
        {
            // returning from secret level
            switch (gameepisode)
            {
            case 1:
                wminfo.next = 3;
                break;
            case 2:
                wminfo.next = 5;
                break;
            case 3:
                wminfo.next = 6;
                break;
            case 4:
                wminfo.next = 2;
                break;
            }
        }
        else
            wminfo.next = gamemap; // go to next level
    }

    wminfo.maxkills = totalkills;
    wminfo.maxitems = totalitems;
    wminfo.maxsecret = totalsecret;
    wminfo.maxfrags = 0;
    if (gamemode == commercial)
        wminfo.partime = 35 * cpars[gamemap - 1];
    else
        wminfo.partime = 35 * pars[gameepisode][gamemap];
    wminfo.player.skills = gamePlayer.killcount;
    wminfo.player.sitems = gamePlayer.itemcount;
    wminfo.player.ssecret = gamePlayer.secretcount;
    wminfo.player.stime = leveltime;

    gamestate = GS_INTERMISSION;
    viewactive = false;
    automapactive = false;

    if (statcopy)
        memcpy(statcopy, &wminfo, sizeof(wminfo));

    WI_Start(&wminfo);
}

// G_WorldDone
void G_WorldDone(void)
{
    gameaction = ga_worlddone;

    if (secretexit)
        gamePlayer.didsecret = true;

    if (gamemode == commercial)
    {
        switch (gamemap)
        {
        case 15:
        case 31:
            if (!secretexit)
                break;
        case 6:
        case 11:
        case 20:
        case 30:
            F_StartFinale();
            break;
        }
    }
}

void G_DoWorldDone(void)
{
    gamestate = GS_LEVEL;
    gamemap = wminfo.next + 1;
    G_DoLoadLevel();
    gameaction = ga_nothing;
    viewactive = true;
}

// G_InitFromSavegame
// Can be called by the startup code or the menu task.
extern bool setsizeneeded;
void R_ExecuteSetViewSize(void);

char savename[256];

void G_LoadGame(char *name)
{
    strcpy(savename, name);
    gameaction = ga_loadgame;
}

#define VERSIONSIZE 16

void G_DoLoadGame(void)
{
    int length;
    int i;
    int a, b, c;
    char vcheck[VERSIONSIZE];

    gameaction = ga_nothing;

    length = M_ReadFile(savename, &savebuffer);
    save_p = savebuffer + SAVESTRINGSIZE;

    // skip the description field
    memset(vcheck, 0, sizeof(vcheck));
    sprintf(vcheck, "version %i", VERSION);
    if (strcmp(reinterpret_cast<char *>(save_p), vcheck))
        return; // bad version
    save_p += VERSIONSIZE;

    gameskill = static_cast<skill_t>(*save_p++);
    gameepisode = *save_p++;
    gamemap = *save_p++;
    save_p += 4; // legacy player-presence bytes

    // load a base level
    G_InitNew(gameskill, gameepisode, gamemap);

    // get the times
    a = *save_p++;
    b = *save_p++;
    c = *save_p++;
    leveltime = (a << 16) + (b << 8) + c;

    // dearchive all the modifications
    P_UnArchivePlayers();
    P_UnArchiveWorld();
    P_UnArchiveThinkers();
    P_UnArchiveSpecials();

    if (*save_p != 0x1d)
        I_Error("Bad savegame");

    // done
    Z_Free(savebuffer);

    if (setsizeneeded)
        R_ExecuteSetViewSize();

    // draw the pattern into the back screen
    R_FillBackScreen();
}

// G_SaveGame
// Called by the menu task.
// Description is a 24 byte text string
void G_SaveGame(int slot,
                char *description)
{
    savegameslot = slot;
    strcpy(savedescription, description);
    sendsave = true;
}

void G_DoSaveGame(void)
{
    char name[100];
    char name2[VERSIONSIZE];
    char *description;
    int length;
    int i;

    if (M_CheckParm("-cdrom"))
        sprintf(name, "c:\\doomdata\\" SAVEGAMENAME "%d.dsg", savegameslot);
    else
        sprintf(name, SAVEGAMENAME "%d.dsg", savegameslot);
    description = savedescription;

    save_p = savebuffer = screens[1] + 0x4000;

    memcpy(save_p, description, SAVESTRINGSIZE);
    save_p += SAVESTRINGSIZE;
    memset(name2, 0, sizeof(name2));
    sprintf(name2, "version %i", VERSION);
    memcpy(save_p, name2, VERSIONSIZE);
    save_p += VERSIONSIZE;

    *save_p++ = gameskill;
    *save_p++ = gameepisode;
    *save_p++ = gamemap;
    *save_p++ = 1;
    *save_p++ = 0;
    *save_p++ = 0;
    *save_p++ = 0;
    *save_p++ = leveltime >> 16;
    *save_p++ = leveltime >> 8;
    *save_p++ = leveltime;

    P_ArchivePlayers();
    P_ArchiveWorld();
    P_ArchiveThinkers();
    P_ArchiveSpecials();

    *save_p++ = 0x1d; // consistancy marker

    length = save_p - savebuffer;
    if (length > SAVEGAMESIZE)
        I_Error("Savegame buffer overrun");
    M_WriteFile(name, savebuffer, length);
    gameaction = ga_nothing;
    savedescription[0] = 0;

    gamePlayer.message = GGSAVED;

    // draw the pattern into the back screen
    R_FillBackScreen();
}

// G_InitNew
// Can be called by the startup code or the menu task,
skill_t d_skill;
int d_episode;
int d_map;

void G_DeferedInitNew(skill_t skill,
                      int episode,
                      int map)
{
    d_skill = skill;
    d_episode = episode;
    d_map = map;
    gameaction = ga_newgame;
}

void G_DoNewGame(void)
{
    respawnparm = false;
    fastparm = false;
    nomonsters = false;
    G_InitNew(d_skill, d_episode, d_map);
    gameaction = ga_nothing;
}

// The sky texture to be used instead of the F_SKY1 dummy.
extern int skytexture;

void G_InitNew(skill_t skill,
               int episode,
               int map)
{
    int i;

    if (paused)
    {
        paused = false;
        S_ResumeSound();
    }

    if (skill > sk_nightmare)
        skill = sk_nightmare;

    // This was quite messy with SPECIAL and commented parts.
    // Supposedly hacks to make the latest edition work.
    // It might not work properly.
    if (episode < 1)
        episode = 1;

    if (gamemode == retail)
    {
        if (episode > 4)
            episode = 4;
    }
    else if (gamemode == shareware)
    {
        if (episode > 1)
            episode = 1; // only start episode 1 on shareware
    }
    else
    {
        if (episode > 3)
            episode = 3;
    }

    if (map < 1)
        map = 1;

    if ((map > 9) && (gamemode != commercial))
        map = 9;

    M_ClearRandom();

    if (skill == sk_nightmare || respawnparm)
        respawnmonsters = true;
    else
        respawnmonsters = false;

    if (fastparm || (skill == sk_nightmare && gameskill != sk_nightmare))
    {
        for (i = S_SARG_RUN1; i <= S_SARG_PAIN2; i++)
            states[i].tics >>= 1;
        mobjinfo[MT_BRUISERSHOT].speed = 20 * FRACUNIT;
        mobjinfo[MT_HEADSHOT].speed = 20 * FRACUNIT;
        mobjinfo[MT_TROOPSHOT].speed = 20 * FRACUNIT;
    }
    else if (skill != sk_nightmare && gameskill == sk_nightmare)
    {
        for (i = S_SARG_RUN1; i <= S_SARG_PAIN2; i++)
            states[i].tics <<= 1;
        mobjinfo[MT_BRUISERSHOT].speed = 15 * FRACUNIT;
        mobjinfo[MT_HEADSHOT].speed = 10 * FRACUNIT;
        mobjinfo[MT_TROOPSHOT].speed = 10 * FRACUNIT;
    }

    gamePlayer.playerstate = PST_REBORN;

    paused = false;
    automapactive = false;
    viewactive = true;
    gameepisode = episode;
    gamemap = map;
    gameskill = skill;

    viewactive = true;

    // set the sky map for the episode
    if (gamemode == commercial)
    {
        skytexture = R_TextureNumForName("SKY3");
        if (gamemap < 12)
            skytexture = R_TextureNumForName("SKY1");
        else if (gamemap < 21)
            skytexture = R_TextureNumForName("SKY2");
    }
    else
        switch (episode)
        {
        case 1:
            skytexture = R_TextureNumForName("SKY1");
            break;
        case 2:
            skytexture = R_TextureNumForName("SKY2");
            break;
        case 3:
            skytexture = R_TextureNumForName("SKY3");
            break;
        case 4: // Special Edition sky
            skytexture = R_TextureNumForName("SKY4");
            break;
        }

    G_DoLoadLevel();
}
