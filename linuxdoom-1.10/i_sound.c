// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//     SDL2 sound-effect backend. Music is implemented separately in milestone 9.
//
//-----------------------------------------------------------------------------

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "doomstat.h"
#include "i_sound.h"
#include "sounds.h"
#include "w_wad.h"
#include "z_zone.h"

#define OUTPUT_RATE 44100
#define OUTPUT_CHANNELS 2
#define OUTPUT_SAMPLES 1024
#define MAX_MIX_CHANNELS 32
#define POSITION_FRAC_BITS 32

typedef struct
{
    uint8_t *data;
    uint32_t length;
    uint16_t sample_rate;
    boolean loaded;
} sample_t;

typedef struct
{
    boolean active;
    int handle;
    int sfx_id;
    int volume;
    int separation;
    int pitch;
    int priority;
    uint64_t position;
    uint64_t step;
    uint64_t start_order;
} mix_channel_t;

static SDL_AudioDeviceID audio_device;
static SDL_AudioSpec audio_spec;
static sample_t samples[NUMSFX];
static mix_channel_t mix_channels[MAX_MIX_CHANNELS];
static int next_handle = 1;
static uint64_t next_start_order;

static uint16_t ReadLE16(const uint8_t *data)
{
    return (uint16_t)(data[0] | ((uint16_t)data[1] << 8));
}

static uint32_t ReadLE32(const uint8_t *data)
{
    return (uint32_t)data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
}

static uint64_t ChannelStep(const sample_t *sample, int pitch)
{
    double pitch_scale = pow(2.0, (pitch - 128) / 64.0);
    double step = ((double)sample->sample_rate / audio_spec.freq) * pitch_scale * 4294967296.0;

    if (step < 1.0)
        step = 1.0;

    return (uint64_t)step;
}

static boolean LoadSample(int sfx_id)
{
    sample_t *sample;
    sfxinfo_t *sfx;
    const uint8_t *lump_data;
    int lump;
    int lump_length;
    uint16_t format;
    uint32_t sample_count;

    if (sfx_id <= 0 || sfx_id >= NUMSFX)
        return false;

    sample = &samples[sfx_id];
    if (sample->loaded)
        return sample->data != NULL;

    sfx = &S_sfx[sfx_id];
    if (sfx->link)
    {
        int linked_id = (int)(sfx->link - S_sfx);

        if (!LoadSample(linked_id))
        {
            sample->loaded = true;
            return false;
        }

        *sample = samples[linked_id];
        return true;
    }

    lump = I_GetSfxLumpNum(sfx);
    lump_length = W_LumpLength(lump);
    if (lump_length < 9)
    {
        fprintf(stderr, "I_InitSound: sound lump DS%s is too short.\n", sfx->name);
        sample->loaded = true;
        return false;
    }

    lump_data = (const uint8_t *)W_CacheLumpNum(lump, PU_CACHE);
    format = ReadLE16(lump_data);
    sample->sample_rate = ReadLE16(lump_data + 2);
    sample_count = ReadLE32(lump_data + 4);

    if (format != 3 || sample->sample_rate == 0)
    {
        fprintf(stderr, "I_InitSound: unsupported sound lump DS%s.\n", sfx->name);
        sample->loaded = true;
        return false;
    }

    if (sample_count > (uint32_t)(lump_length - 8))
        sample_count = (uint32_t)(lump_length - 8);

    sample->data = (uint8_t *)malloc(sample_count);
    if (!sample->data)
    {
        fprintf(stderr, "I_InitSound: could not allocate sound DS%s.\n", sfx->name);
        sample->loaded = true;
        return false;
    }

    memcpy(sample->data, lump_data + 8, sample_count);
    sample->length = sample_count;
    sample->loaded = true;
    return true;
}

static void ChannelVolumes(const mix_channel_t *channel, int *left, int *right)
{
    int separation = channel->separation + 1;

    *left = channel->volume - ((channel->volume * separation * separation) >> 16);
    separation -= 257;
    *right = channel->volume - ((channel->volume * separation * separation) >> 16);

    if (*left < 0)
        *left = 0;
    if (*right < 0)
        *right = 0;
}

static void SDLCALL AudioCallback(void *userdata, Uint8 *stream, int length)
{
    int16_t *output = (int16_t *)stream;
    int frame_count = length / (OUTPUT_CHANNELS * (int)sizeof(int16_t));
    int frame;
    int channel_index;

    (void)userdata;
    SDL_memset(stream, 0, (size_t)length);

    for (frame = 0; frame < frame_count; ++frame)
    {
        int left_mix = 0;
        int right_mix = 0;

        for (channel_index = 0; channel_index < MAX_MIX_CHANNELS; ++channel_index)
        {
            mix_channel_t *channel = &mix_channels[channel_index];
            sample_t *sample;
            uint32_t sample_index;
            int value;
            int left_volume;
            int right_volume;

            if (!channel->active)
                continue;

            sample = &samples[channel->sfx_id];
            sample_index = (uint32_t)(channel->position >> POSITION_FRAC_BITS);
            if (sample_index >= sample->length)
            {
                channel->active = false;
                continue;
            }

            value = (int)sample->data[sample_index] - 128;
            ChannelVolumes(channel, &left_volume, &right_volume);
            left_mix += value * left_volume * 2;
            right_mix += value * right_volume * 2;
            channel->position += channel->step;
        }

        if (left_mix < -32768)
            left_mix = -32768;
        else if (left_mix > 32767)
            left_mix = 32767;

        if (right_mix < -32768)
            right_mix = -32768;
        else if (right_mix > 32767)
            right_mix = 32767;

        output[frame * 2] = (int16_t)left_mix;
        output[frame * 2 + 1] = (int16_t)right_mix;
    }
}

void I_InitSound(void)
{
    SDL_AudioSpec desired;

    if (audio_device)
        return;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "I_InitSound: SDL audio initialization failed: %s\n", SDL_GetError());
        return;
    }

    SDL_zero(desired);
    desired.freq = OUTPUT_RATE;
    desired.format = AUDIO_S16SYS;
    desired.channels = OUTPUT_CHANNELS;
    desired.samples = OUTPUT_SAMPLES;
    desired.callback = AudioCallback;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &desired, &audio_spec, 0);
    if (!audio_device)
    {
        fprintf(stderr, "I_InitSound: could not open SDL audio device: %s\n", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return;
    }

    SDL_PauseAudioDevice(audio_device, 0);
    fprintf(stderr, "I_InitSound: %d Hz, signed 16-bit stereo SDL audio ready.\n",
            audio_spec.freq);
}

void I_UpdateSound(void)
{
}

void I_SubmitSound(void)
{
}

void I_ShutdownSound(void)
{
    int i;

    if (audio_device)
    {
        SDL_PauseAudioDevice(audio_device, 1);
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }

    for (i = 1; i < NUMSFX; ++i)
    {
        if (!S_sfx[i].link)
            free(samples[i].data);
    }

    memset(samples, 0, sizeof(samples));
    memset(mix_channels, 0, sizeof(mix_channels));
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void I_SetChannels(void)
{
}

void I_SetSfxVolume(int volume)
{
    snd_SfxVolume = volume;
}

int I_GetSfxLumpNum(sfxinfo_t *sfxinfo)
{
    char name[9];
    int lump;

    SDL_snprintf(name, sizeof(name), "ds%s", sfxinfo->name);
    lump = W_CheckNumForName(name);
    if (lump < 0)
        lump = W_GetNumForName("dspistol");

    return lump;
}

int I_StartSound(int id, int volume, int separation, int pitch, int priority)
{
    int i;
    int selected = -1;
    uint64_t oldest_order = UINT64_MAX;
    int handle;

    if (!audio_device || !LoadSample(id))
        return -1;

    SDL_LockAudioDevice(audio_device);

    for (i = 0; i < MAX_MIX_CHANNELS; ++i)
    {
        if (!mix_channels[i].active)
        {
            selected = i;
            break;
        }

        if (mix_channels[i].priority >= priority && mix_channels[i].start_order < oldest_order)
        {
            selected = i;
            oldest_order = mix_channels[i].start_order;
        }
    }

    if (selected < 0)
    {
        SDL_UnlockAudioDevice(audio_device);
        return -1;
    }

    handle = next_handle++;
    if (next_handle <= 0)
        next_handle = 1;

    mix_channels[selected].active = true;
    mix_channels[selected].handle = handle;
    mix_channels[selected].sfx_id = id;
    mix_channels[selected].volume = volume;
    mix_channels[selected].separation = separation;
    mix_channels[selected].pitch = pitch;
    mix_channels[selected].priority = priority;
    mix_channels[selected].position = 0;
    mix_channels[selected].step = ChannelStep(&samples[id], pitch);
    mix_channels[selected].start_order = next_start_order++;

    SDL_UnlockAudioDevice(audio_device);
    return handle;
}

void I_StopSound(int handle)
{
    int i;

    if (!audio_device || handle < 0)
        return;

    SDL_LockAudioDevice(audio_device);
    for (i = 0; i < MAX_MIX_CHANNELS; ++i)
    {
        if (mix_channels[i].active && mix_channels[i].handle == handle)
        {
            mix_channels[i].active = false;
            break;
        }
    }
    SDL_UnlockAudioDevice(audio_device);
}

int I_SoundIsPlaying(int handle)
{
    int i;
    int playing = 0;

    if (!audio_device || handle < 0)
        return 0;

    SDL_LockAudioDevice(audio_device);
    for (i = 0; i < MAX_MIX_CHANNELS; ++i)
    {
        if (mix_channels[i].active && mix_channels[i].handle == handle)
        {
            playing = 1;
            break;
        }
    }
    SDL_UnlockAudioDevice(audio_device);
    return playing;
}

void I_UpdateSoundParams(int handle, int volume, int separation, int pitch)
{
    int i;

    if (!audio_device || handle < 0)
        return;

    SDL_LockAudioDevice(audio_device);
    for (i = 0; i < MAX_MIX_CHANNELS; ++i)
    {
        mix_channel_t *channel = &mix_channels[i];

        if (channel->active && channel->handle == handle)
        {
            channel->volume = volume;
            channel->separation = separation;
            channel->pitch = pitch;
            channel->step = ChannelStep(&samples[channel->sfx_id], pitch);
            break;
        }
    }
    SDL_UnlockAudioDevice(audio_device);
}

void I_InitMusic(void)
{
}

void I_ShutdownMusic(void)
{
}

void I_SetMusicVolume(int volume)
{
    snd_MusicVolume = volume;
}

void I_PauseSong(int handle)
{
    (void)handle;
}

void I_ResumeSong(int handle)
{
    (void)handle;
}

int I_RegisterSong(void *data)
{
    (void)data;
    return 0;
}

void I_PlaySong(int handle, int looping)
{
    (void)handle;
    (void)looping;
}

void I_StopSong(int handle)
{
    (void)handle;
}

void I_UnRegisterSong(int handle)
{
    (void)handle;
}

int I_QrySongPlaying(int handle)
{
    (void)handle;
    return 0;
}
