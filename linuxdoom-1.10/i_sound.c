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
//     Temporary sound and music platform stubs.
//
//-----------------------------------------------------------------------------

#include <stdio.h>

#include "i_sound.h"

static boolean sound_warnings_enabled = true;

static void I_WarnSoundDisabled(void)
{
    static boolean warning_shown = false;

    if (sound_warnings_enabled && !warning_shown)
    {
        fprintf(stderr, "Warning: sound and music are disabled.\n");
        warning_shown = true;
    }
}

void I_InitSound(void)
{
    I_WarnSoundDisabled();
}

void I_UpdateSound(void)
{
    I_WarnSoundDisabled();
}

void I_SubmitSound(void)
{
    I_WarnSoundDisabled();
}

void I_ShutdownSound(void)
{
    I_WarnSoundDisabled();
}

void I_SetChannels(void)
{
    I_WarnSoundDisabled();
}

void I_SetSfxVolume(int volume)
{
    I_WarnSoundDisabled();
}

int I_GetSfxLumpNum(sfxinfo_t *sfxinfo)
{
    I_WarnSoundDisabled();
    return 0;
}

int I_StartSound(int id, int volume, int separation, int pitch, int priority)
{
    I_WarnSoundDisabled();
    return -1;
}

void I_StopSound(int handle)
{
    I_WarnSoundDisabled();
}

int I_SoundIsPlaying(int handle)
{
    I_WarnSoundDisabled();
    return 0;
}

void I_UpdateSoundParams(int handle, int volume, int separation, int pitch)
{
    I_WarnSoundDisabled();
}

void I_InitMusic(void)
{
    I_WarnSoundDisabled();
}

void I_ShutdownMusic(void)
{
    I_WarnSoundDisabled();
}

void I_SetMusicVolume(int volume)
{
    I_WarnSoundDisabled();
}

void I_PauseSong(int handle)
{
    I_WarnSoundDisabled();
}

void I_ResumeSong(int handle)
{
    I_WarnSoundDisabled();
}

int I_RegisterSong(void *data)
{
    I_WarnSoundDisabled();
    return 0;
}

void I_PlaySong(int handle, int looping)
{
    I_WarnSoundDisabled();
}

void I_StopSong(int handle)
{
    I_WarnSoundDisabled();
}

void I_UnRegisterSong(int handle)
{
    I_WarnSoundDisabled();
}

int I_QrySongPlaying(int handle)
{
    I_WarnSoundDisabled();
    return 0;
}