
// DESCRIPTION:
//-----------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "SystemInterface/system.hpp"
#include "SystemInterface/sound.hpp"
#include "SystemInterface/video.hpp"
#include "Doom/net.hpp"
#include "Miscellaneous/misc.hpp"
#include "doomdef.hpp"

int mb_used = sizeof(void *) == 8 ? 32 : 6;

static boolean sdl_initialized;
static boolean shutdown_started;

void I_Shutdown(void)
{
    if (shutdown_started)
        return;

    shutdown_started = true;

    D_QuitNetGame();
    I_ShutdownSound();
    I_ShutdownMusic();
    I_ShutdownGraphics();

    if (sdl_initialized)
    {
        SDL_Quit();
        sdl_initialized = false;
    }
}

void I_Tactile(int on,
               int off,
               int total)
{
    // UNUSED.
    on = off = total = 0;
}

ticcmd_t emptycmd;
ticcmd_t *I_BaseTiccmd(void)
{
    return &emptycmd;
}

int I_GetHeapSize(void)
{
    return mb_used * 1024 * 1024;
}

byte *I_ZoneBase(int *size)
{
    *size = mb_used * 1024 * 1024;
    return (byte *)malloc(*size);
}

// I_GetTime
// returns time in TICRATE (35 Hz) tics
int I_GetTime(void)
{
    static Uint64 basetime;
    static Uint64 frequency;
    static boolean initialized;
    Uint64 now = SDL_GetPerformanceCounter();

    if (!initialized)
    {
        basetime = now;
        frequency = SDL_GetPerformanceFrequency();
        initialized = true;
    }

    return (int)(((now - basetime) * TICRATE) / frequency);
}

void I_Init(void)
{
    if (sdl_initialized)
        return;

    if (SDL_Init(SDL_INIT_TIMER) < 0)
    {
        char error[256];
        snprintf(error, sizeof(error), "%s", SDL_GetError());
        SDL_Quit();
        I_Error("SDL initialization failed: %s", error);
    }

    sdl_initialized = true;

#ifdef DOOM_DEBUG
    {
        SDL_version version;
        SDL_GetVersion(&version);
        fprintf(stderr, "I_Init: SDL %u.%u.%u timer initialized.\n",
                version.major, version.minor, version.patch);
        fprintf(stderr, "I_Init: timer frequency %llu Hz, game rate %d Hz.\n",
                (unsigned long long)SDL_GetPerformanceFrequency(), TICRATE);
        fprintf(stderr, "I_Init: zone heap %d MiB allocated through malloc.\n", mb_used);
    }
#endif

    I_InitSound();
    //  I_InitGraphics();
}

void I_Quit(void)
{
    M_SaveDefaults();
    I_Shutdown();
    exit(EXIT_SUCCESS);
}

#ifdef DOOM_DEBUG
boolean I_RunTimerTest(void)
{
    Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 start_counter = SDL_GetPerformanceCounter();
    Uint64 end_counter;
    int start_tic = I_GetTime();
    int previous_tic = start_tic;
    int end_tic;
    boolean monotonic = true;
    double elapsed_seconds;
    double measured_rate;

    do
    {
        int current_tic = I_GetTime();
        if (current_tic < previous_tic)
            monotonic = false;
        previous_tic = current_tic;
        SDL_Delay(1);
        end_counter = SDL_GetPerformanceCounter();
    } while (end_counter - start_counter < frequency);

    end_tic = I_GetTime();
    elapsed_seconds = (double)(end_counter - start_counter) / (double)frequency;
    measured_rate = (double)(end_tic - start_tic) / elapsed_seconds;

    fprintf(stderr,
            "I_TimerTest: start=%d end=%d elapsed=%.3f s rate=%.2f tics/s monotonic=%s.\n",
            start_tic, end_tic, elapsed_seconds, measured_rate,
            monotonic ? "yes" : "no");

    return monotonic && measured_rate >= 34.0 && measured_rate <= 36.0;
}
#endif

void I_WaitVBL(int count)
{
    if (count > 0)
        SDL_Delay((Uint32)(((Uint64)count * 1000u) / 70u));
}

void I_BeginRead(void)
{
    // unused
}

void I_EndRead(void)
{
    // unused
}

byte *I_AllocLow(int length)
{
    byte *mem;
    mem = static_cast<byte *>(calloc(1, length));
    return mem;
}

// I_Error
void I_Error(char *error, ...)
{
    va_list argptr;

    va_start(argptr, error);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, error, argptr);
    fprintf(stderr, "\n");
    va_end(argptr);

    fflush(stderr);
    I_Shutdown();
    exit(EXIT_FAILURE);
}
