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
//     SDL video platform layer.
//
//-----------------------------------------------------------------------------

#include <SDL.h>
#include <string.h>

#include "doomdef.h"
#include "i_system.h"
#include "i_video.h"
#include "v_video.h"

#define DISPLAY_WIDTH SCREENWIDTH
#define DISPLAY_HEIGHT (SCREENHEIGHT * 6 / 5)

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static Uint32 rgba_palette[256];
static Uint32 rgba_framebuffer[SCREENWIDTH * SCREENHEIGHT];

void I_InitGraphics(void)
{
    if (window)
        return;

    if (!screens[0])
        I_Error("I_InitGraphics: screens[0] is not allocated");

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        I_Error("I_InitGraphics: SDL video initialization failed: %s",
                SDL_GetError());

    window = SDL_CreateWindow("DOOM",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              DISPLAY_WIDTH * 2,
                              DISPLAY_HEIGHT * 2,
                              SDL_WINDOW_RESIZABLE);
    if (!window)
        I_Error("I_InitGraphics: window creation failed: %s", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED);
    if (!renderer)
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (!renderer)
        I_Error("I_InitGraphics: renderer creation failed: %s", SDL_GetError());

    if (SDL_RenderSetLogicalSize(renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT) < 0)
        I_Error("I_InitGraphics: logical size setup failed: %s", SDL_GetError());

    if (SDL_RenderSetIntegerScale(renderer, SDL_TRUE) < 0)
        I_Error("I_InitGraphics: integer scaling setup failed: %s",
                SDL_GetError());

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                SCREENWIDTH,
                                SCREENHEIGHT);
    if (!texture)
        I_Error("I_InitGraphics: texture creation failed: %s", SDL_GetError());

#ifdef DOOM_DEBUG
    {
        SDL_RendererInfo info;
        SDL_GetRendererInfo(renderer, &info);
        fprintf(stderr,
                "I_InitGraphics: %dx%d indexed framebuffer, %dx%d window, renderer=%s.\n",
                SCREENWIDTH, SCREENHEIGHT, DISPLAY_WIDTH * 2, DISPLAY_HEIGHT * 2,
                info.name ? info.name : "unknown");
    }
#endif
}

void I_ShutdownGraphics(void)
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    if (SDL_WasInit(SDL_INIT_VIDEO))
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void I_StartFrame(void)
{
}

void I_StartTic(void)
{
    SDL_PumpEvents();
}

void I_UpdateNoBlit(void)
{
}

void I_FinishUpdate(void)
{
    int pixel;

    if (!renderer || !texture || !screens[0])
        I_Error("I_FinishUpdate: graphics are not initialized");

    for (pixel = 0; pixel < SCREENWIDTH * SCREENHEIGHT; ++pixel)
        rgba_framebuffer[pixel] = rgba_palette[screens[0][pixel]];

    if (SDL_UpdateTexture(texture, NULL, rgba_framebuffer,
                          SCREENWIDTH * sizeof(*rgba_framebuffer)) < 0)
        I_Error("I_FinishUpdate: texture upload failed: %s", SDL_GetError());

    if (SDL_RenderClear(renderer) < 0)
        I_Error("I_FinishUpdate: renderer clear failed: %s", SDL_GetError());

    if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0)
        I_Error("I_FinishUpdate: texture copy failed: %s", SDL_GetError());

    SDL_RenderPresent(renderer);
}

void I_ReadScreen(byte *screen)
{
    if (!screen || !screens[0])
        I_Error("I_ReadScreen: screen buffer is NULL");

    memcpy(screen, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

void I_SetPalette(byte *palette)
{
    int i;

    if (!palette)
        I_Error("I_SetPalette: palette is NULL");

    for (i = 0; i < 256; ++i)
    {
        Uint32 red = gammatable[usegamma][palette[i * 3]];
        Uint32 green = gammatable[usegamma][palette[i * 3 + 1]];
        Uint32 blue = gammatable[usegamma][palette[i * 3 + 2]];

        rgba_palette[i] = (red << 24) | (green << 16) | (blue << 8) | 0xff;
    }
}