#include "doomdef.h"

#include <SDL.h>
#include <string.h>

#include "d_event.h"
#include "m_argv.hpp"
#include "d_main.h"
#include "i_system.h"
#include "i_video.h"
#include "v_video.hpp"

int TimerTest()
{
    boolean passed;
    I_Init();
    passed = I_RunTimerTest();
    I_Shutdown();
    return passed ? 0 : 1;
}

int VideoTest()
{
    byte palette[256 * 3];
    int color;
    int pixel;

    for (color = 0; color < 256; ++color)
    {
        palette[color * 3] = (byte)color;
        palette[color * 3 + 1] = (byte)(255 - color);
        palette[color * 3 + 2] = (byte)(color / 2);
    }

    I_Init();
    V_Init();
    I_InitGraphics();
    I_SetPalette(palette);

    for (pixel = 0; pixel < SCREENWIDTH * SCREENHEIGHT; ++pixel)
        screens[0][pixel] = (byte)(pixel % 256);

    I_FinishUpdate();
    I_Shutdown();
    return 0;
}

int KeyboardTest()
{
    static const struct
    {
        SDL_Keycode sdl_key;
        int doom_key;
    } keys[] = {
        {SDLK_ESCAPE, KEY_ESCAPE},
        {SDLK_RETURN, KEY_ENTER},
        {SDLK_LEFT, KEY_LEFTARROW},
        {SDLK_RIGHT, KEY_RIGHTARROW},
        {SDLK_UP, KEY_UPARROW},
        {SDLK_DOWN, KEY_DOWNARROW},
        {SDLK_F1, KEY_F1},
        {SDLK_F2, KEY_F2},
        {SDLK_F3, KEY_F3},
        {SDLK_F4, KEY_F4},
        {SDLK_F5, KEY_F5},
        {SDLK_F6, KEY_F6},
        {SDLK_F7, KEY_F7},
        {SDLK_F8, KEY_F8},
        {SDLK_F9, KEY_F9},
        {SDLK_F10, KEY_F10},
        {SDLK_F11, KEY_F11},
        {SDLK_F12, KEY_F12},
        {SDLK_LCTRL, KEY_RCTRL},
        {SDLK_LALT, KEY_RALT},
        {SDLK_LSHIFT, KEY_RSHIFT},
        {SDLK_w, 'w'},
        {SDLK_a, 'a'},
        {SDLK_s, 's'},
        {SDLK_d, 'd'},
    };
    int key_index;
    int event_index = eventhead;

    I_Init();
    V_Init();
    I_InitGraphics();

    if (!SDL_GetRelativeMouseMode())
        I_Error("KeyboardTest: relative mouse mode is disabled");

    for (key_index = 0; key_index < sizeof(keys) / sizeof(keys[0]); ++key_index)
    {
        SDL_Event key_event = {0};

        key_event.type = SDL_KEYDOWN;
        key_event.key.keysym.sym = keys[key_index].sdl_key;
        if (SDL_PushEvent(&key_event) < 0)
            I_Error("KeyboardTest: SDL key-down push failed: %s", SDL_GetError());

        key_event.type = SDL_KEYUP;
        if (SDL_PushEvent(&key_event) < 0)
            I_Error("KeyboardTest: SDL key-up push failed: %s", SDL_GetError());
    }

    I_StartTic();

    for (key_index = 0; key_index < sizeof(keys) / sizeof(keys[0]); ++key_index)
    {
        event_t *key_down = &events[event_index];
        event_index = (event_index + 1) & (MAXEVENTS - 1);
        event_t *key_up = &events[event_index];
        event_index = (event_index + 1) & (MAXEVENTS - 1);

        if (key_down->type != ev_keydown || key_up->type != ev_keyup ||
            key_down->data1 != keys[key_index].doom_key ||
            key_up->data1 != keys[key_index].doom_key)
            I_Error("KeyboardTest: incorrect event for SDL key %d",
                    keys[key_index].sdl_key);
    }

    if (eventhead != event_index)
        I_Error("KeyboardTest: unexpected number of posted events");

    {
        SDL_Event mouse_event = {0};
        event_t *mouse_down;
        event_t *mouse_motion;
        event_t *mouse_up;

        mouse_event.type = SDL_MOUSEBUTTONDOWN;
        mouse_event.button.button = SDL_BUTTON_LEFT;
        SDL_PushEvent(&mouse_event);

        mouse_event.type = SDL_MOUSEMOTION;
        mouse_event.motion.xrel = 3;
        mouse_event.motion.yrel = -2;
        SDL_PushEvent(&mouse_event);

        mouse_event.type = SDL_MOUSEBUTTONUP;
        mouse_event.button.button = SDL_BUTTON_LEFT;
        SDL_PushEvent(&mouse_event);

        I_StartTic();

        mouse_down = &events[event_index];
        event_index = (event_index + 1) & (MAXEVENTS - 1);
        mouse_motion = &events[event_index];
        event_index = (event_index + 1) & (MAXEVENTS - 1);
        mouse_up = &events[event_index];
        event_index = (event_index + 1) & (MAXEVENTS - 1);

        if (mouse_down->type != ev_mouse || mouse_down->data1 != 1 ||
            mouse_motion->type != ev_mouse || mouse_motion->data1 != 1 ||
            mouse_motion->data2 != 12 || mouse_motion->data3 != 8 ||
            mouse_up->type != ev_mouse || mouse_up->data1 != 0 ||
            eventhead != event_index)
            I_Error("KeyboardTest: incorrect mouse events");
    }

    {
        SDL_Event input_event = {0};
        event_t *key_up;
        event_t *mouse_up;

        input_event.type = SDL_KEYDOWN;
        input_event.key.keysym.sym = SDLK_LCTRL;
        SDL_PushEvent(&input_event);
        input_event.type = SDL_MOUSEBUTTONDOWN;
        input_event.button.button = SDL_BUTTON_RIGHT;
        SDL_PushEvent(&input_event);
        I_StartTic();
        event_index = eventhead;

        input_event.type = SDL_WINDOWEVENT;
        input_event.window.event = SDL_WINDOWEVENT_FOCUS_LOST;
        SDL_PushEvent(&input_event);
        I_StartTic();

        key_up = &events[event_index];
        event_index = (event_index + 1) & (MAXEVENTS - 1);
        mouse_up = &events[event_index];
        event_index = (event_index + 1) & (MAXEVENTS - 1);

        if (key_up->type != ev_keyup || key_up->data1 != KEY_RCTRL ||
            mouse_up->type != ev_mouse || mouse_up->data1 != 0 ||
            eventhead != event_index || SDL_GetRelativeMouseMode())
            I_Error("KeyboardTest: focus loss did not release input");

        input_event.window.event = SDL_WINDOWEVENT_FOCUS_GAINED;
        SDL_PushEvent(&input_event);
        I_StartTic();

        if (!SDL_GetRelativeMouseMode())
            I_Error("KeyboardTest: focus gain did not restore relative mouse mode");
    }

    I_Shutdown();
    return 0;
}

int TestMain()
{

    for (int i = 1; i < myargc; ++i)
    {
        if (!strcmp(myargv[i], "-timertest"))
        {
            return TimerTest();
        }

        if (!strcmp(myargv[i], "-videotest"))
            return VideoTest();

        if (!strcmp(myargv[i], "-keyboardtest"))
            return KeyboardTest();
    }
    return -1;
}
