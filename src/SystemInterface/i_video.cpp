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

extern "C"
{
#include "d_main.hpp"
#include "doomdef.hpp"
#include "i_system.hpp"
#include "i_video.hpp"
#include "v_video.hpp"

#define DISPLAY_WIDTH SCREENWIDTH
#define DISPLAY_HEIGHT (SCREENHEIGHT * 6 / 5)

    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_Texture *texture;
    static Uint32 rgba_palette[256];
    static Uint32 rgba_framebuffer[SCREENWIDTH * SCREENHEIGHT];
    static boolean keys_down[256];
    static int mouse_buttons;

    static void I_PostKeyEvent(evtype_t type, int key)
    {
        event_t event = {0};

        event.type = type;
        event.data1 = key;
        D_PostEvent(&event);

        if ((unsigned)key < sizeof(keys_down) / sizeof(keys_down[0]))
            keys_down[key] = type == ev_keydown;
    }

    static void I_PostMouseEvent(int x, int y)
    {
        event_t event = {0};

        event.type = ev_mouse;
        event.data1 = mouse_buttons;
        event.data2 = x * 4;
        event.data3 = -y * 4;
        D_PostEvent(&event);
    }

    static void I_ReleaseInput(void)
    {
        int key;

        for (key = 0; key < sizeof(keys_down) / sizeof(keys_down[0]); ++key)
        {
            if (keys_down[key])
                I_PostKeyEvent(ev_keyup, key);
        }

        if (mouse_buttons)
        {
            mouse_buttons = 0;
            I_PostMouseEvent(0, 0);
        }
    }

    static int I_TranslateKey(SDL_Keycode key)
    {
        switch (key)
        {
        case SDLK_LEFT:
            return KEY_LEFTARROW;
        case SDLK_RIGHT:
            return KEY_RIGHTARROW;
        case SDLK_DOWN:
            return KEY_DOWNARROW;
        case SDLK_UP:
            return KEY_UPARROW;
        case SDLK_ESCAPE:
            return KEY_ESCAPE;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            return KEY_ENTER;
        case SDLK_TAB:
            return KEY_TAB;
        case SDLK_F1:
            return KEY_F1;
        case SDLK_F2:
            return KEY_F2;
        case SDLK_F3:
            return KEY_F3;
        case SDLK_F4:
            return KEY_F4;
        case SDLK_F5:
            return KEY_F5;
        case SDLK_F6:
            return KEY_F6;
        case SDLK_F7:
            return KEY_F7;
        case SDLK_F8:
            return KEY_F8;
        case SDLK_F9:
            return KEY_F9;
        case SDLK_F10:
            return KEY_F10;
        case SDLK_F11:
            return KEY_F11;
        case SDLK_F12:
            return KEY_F12;
        case SDLK_BACKSPACE:
        case SDLK_DELETE:
            return KEY_BACKSPACE;
        case SDLK_PAUSE:
            return KEY_PAUSE;
        case SDLK_EQUALS:
        case SDLK_KP_EQUALS:
            return KEY_EQUALS;
        case SDLK_MINUS:
        case SDLK_KP_MINUS:
            return KEY_MINUS;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            return KEY_RSHIFT;
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            return KEY_RCTRL;
        case SDLK_LALT:
        case SDLK_RALT:
            return KEY_RALT;
        case SDLK_KP_0:
            return '0';
        case SDLK_KP_1:
            return '1';
        case SDLK_KP_2:
            return '2';
        case SDLK_KP_3:
            return '3';
        case SDLK_KP_4:
            return '4';
        case SDLK_KP_5:
            return '5';
        case SDLK_KP_6:
            return '6';
        case SDLK_KP_7:
            return '7';
        case SDLK_KP_8:
            return '8';
        case SDLK_KP_9:
            return '9';
        default:
            if (key >= SDLK_SPACE && key <= SDLK_z)
                return (int)key;
            return 0;
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

        if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
        {
#ifdef DOOM_DEBUG
            fprintf(stderr, "I_InitGraphics: relative mouse mode unavailable: %s.\n",
                    SDL_GetError());
#endif
        }

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

    void I_StartTic(void)
    {
        SDL_Event sdl_event;

        while (SDL_PollEvent(&sdl_event))
        {
            switch (sdl_event.type)
            {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                int key = I_TranslateKey(sdl_event.key.keysym.sym);

                if (key)
                    I_PostKeyEvent(sdl_event.type == SDL_KEYDOWN
                                       ? ev_keydown
                                       : ev_keyup,
                                   key);
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                int button = 0;

                if (sdl_event.button.button == SDL_BUTTON_LEFT)
                    button = 1;
                else if (sdl_event.button.button == SDL_BUTTON_MIDDLE)
                    button = 2;
                else if (sdl_event.button.button == SDL_BUTTON_RIGHT)
                    button = 4;

                if (button)
                {
                    if (sdl_event.type == SDL_MOUSEBUTTONDOWN)
                        mouse_buttons |= button;
                    else
                        mouse_buttons &= ~button;
                    I_PostMouseEvent(0, 0);
                }
                break;
            }

            case SDL_MOUSEMOTION:
                if (sdl_event.motion.xrel || sdl_event.motion.yrel)
                    I_PostMouseEvent(sdl_event.motion.xrel,
                                     sdl_event.motion.yrel);
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
}