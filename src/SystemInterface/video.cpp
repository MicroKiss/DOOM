// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
// $Id:$
// Copyright (C) 1993-1996 by id Software, Inc.
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
// This source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
// DESCRIPTION:
//     SDL video platform layer.
//-----------------------------------------------------------------------------

#include "Renderer/video.hpp"
#include "Doom/main.hpp"
#include "Inputs/InputHandler.hpp"
#include "SystemInterface/system.hpp"
#include "SystemInterface/video.hpp"
#include "doomdef.hpp"
#include <SDL.h>
#include <string.h>

#define DISPLAY_WIDTH SCREENWIDTH
#define DISPLAY_HEIGHT (SCREENHEIGHT * 6 / 5)

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static Uint32 rgba_palette[256];
static Uint32 rgba_framebuffer[SCREENWIDTH * SCREENHEIGHT];

static void I_ReleaseInput(void)
{
    inputHandler.ReleaseAll();
}

static INPUTS TranslateMouseButton(Uint8 button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT:
        return INPUTS::MOUSE_LEFT;
    case SDL_BUTTON_MIDDLE:
        return INPUTS::MOUSE_MIDDLE;
    case SDL_BUTTON_RIGHT:
        return INPUTS::MOUSE_RIGHT;
    default:
        return INPUTS::NOTHING;
    }
}

static INPUTS TranslateKey(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_LEFT:
        return INPUTS::MOVE_LEFT;
    case SDLK_RIGHT:
        return INPUTS::MOVE_RIGHT;
    case SDLK_DOWN:
        return INPUTS::MOVE_BACKWARD;
    case SDLK_UP:
        return INPUTS::MOVE_FORWARD;
    case SDLK_ESCAPE:
        return INPUTS::MENU;
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
        return INPUTS::RETURN;
    case SDLK_TAB:
        return INPUTS::TAB;
    case SDLK_F1:
        return INPUTS::F1;
    case SDLK_F2:
        return INPUTS::F2;
    case SDLK_F3:
        return INPUTS::F3;
    case SDLK_F4:
        return INPUTS::F4;
    case SDLK_F5:
        return INPUTS::F5;
    case SDLK_F6:
        return INPUTS::F6;
    case SDLK_F7:
        return INPUTS::F7;
    case SDLK_F8:
        return INPUTS::F8;
    case SDLK_F9:
        return INPUTS::F9;
    case SDLK_F10:
        return INPUTS::F10;
    case SDLK_F11:
        return INPUTS::F11;
    case SDLK_F12:
        return INPUTS::F12;
    case SDLK_BACKSPACE:
    case SDLK_DELETE:
        return INPUTS::BACKSPACE;
    case SDLK_PAUSE:
        return INPUTS::PAUSE;
    case SDLK_EQUALS:
    case SDLK_KP_EQUALS:
        return INPUTS::EQUALS;
    case SDLK_MINUS:
    case SDLK_KP_MINUS:
        return INPUTS::MINUS;
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
        return INPUTS::SHIFT;
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        return INPUTS::CTRL;
    case SDLK_LALT:
    case SDLK_RALT:
        return INPUTS::ALT;
    case SDLK_KP_0:
        return INPUTS::KP_0;
    case SDLK_KP_1:
        return INPUTS::KP_1;
    case SDLK_KP_2:
        return INPUTS::KP_2;
    case SDLK_KP_3:
        return INPUTS::KP_3;
    case SDLK_KP_4:
        return INPUTS::KP_4;
    case SDLK_KP_5:
        return INPUTS::KP_5;
    case SDLK_KP_6:
        return INPUTS::KP_6;
    case SDLK_KP_7:
        return INPUTS::KP_7;
    case SDLK_KP_8:
        return INPUTS::KP_8;
    case SDLK_KP_9:
        return INPUTS::KP_9;
    default:
        if (key >= SDLK_SPACE && key <= SDLK_z)
            return static_cast<INPUTS>(key);
        return INPUTS::NOTHING;
    }
}

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
                              DISPLAY_WIDTH * 3,
                              DISPLAY_HEIGHT * 3,
                              SDL_WINDOW_RESIZABLE);
    if (!window)
        I_Error("I_InitGraphics: window creation failed: %s", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

    if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
    {
#ifdef DOOM_DEBUG
        fprintf(stderr, "I_InitGraphics: relative mouse mode unavailable: %s.\n", SDL_GetError());
#endif
    }

#ifdef DOOM_DEBUG
    {
        SDL_RendererInfo info;
        SDL_GetRendererInfo(renderer, &info);
        fprintf(stderr,
                "I_InitGraphics: %dx%d indexed framebuffer, %dx%d window, renderer=%s.\n",
                SCREENWIDTH,
                SCREENHEIGHT,
                DISPLAY_WIDTH * 3,
                DISPLAY_HEIGHT * 3,
                info.name ? info.name : "unknown");
    }
#endif
}

void I_ShutdownGraphics(void)
{
    SDL_SetRelativeMouseMode(SDL_FALSE);

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

void I_StartTic()
{
    SDL_Event sdl_event;
    inputHandler.Update();

    while (SDL_PollEvent(&sdl_event))
    {
        switch (sdl_event.type)
        {
        case SDL_KEYDOWN: {
            INPUTS input = TranslateKey(sdl_event.key.keysym.sym);
            if (input != INPUTS::NOTHING)
                inputHandler.Press(input);
            break;
        }
        case SDL_KEYUP: {
            INPUTS input = TranslateKey(sdl_event.key.keysym.sym);
            if (input != INPUTS::NOTHING)
                inputHandler.Release(input);
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            INPUTS button = TranslateMouseButton(sdl_event.button.button);

            if (button != INPUTS::NOTHING)
            {
                if (sdl_event.type == SDL_MOUSEBUTTONDOWN)
                    inputHandler.Press(button);
                else
                    inputHandler.Release(button);
            }
            break;
        }

        case SDL_MOUSEMOTION:
            if (sdl_event.motion.xrel || sdl_event.motion.yrel)
                inputHandler.AddMouseMotion(sdl_event.motion.xrel,
                                            -sdl_event.motion.yrel);
            break;
        case SDL_MOUSEWHEEL:
            if (sdl_event.wheel.y)
            {
                int y = sdl_event.wheel.y;
                if (sdl_event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
                    y = -y;
                inputHandler.AddMouseWheel(y);
            }
            break;

        case SDL_WINDOWEVENT:
            if (sdl_event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
            {
                I_ReleaseInput();
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            else if (sdl_event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED &&
                     SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
            {
#ifdef DOOM_DEBUG
                fprintf(stderr,
                        "I_StartTic: relative mouse mode unavailable: %s.\n",
                        SDL_GetError());
#endif
            }
            break;

        case SDL_QUIT:
            I_Quit();
            break;
        }
    }
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

    if (SDL_UpdateTexture(texture, NULL, rgba_framebuffer, SCREENWIDTH * sizeof(*rgba_framebuffer)) < 0)
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
