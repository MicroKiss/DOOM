#include "doomdef.hpp"

#include <SDL.h>
#include <string.h>

#include "Doom/main.hpp"
#include "Miscellaneous/argv.hpp"
#include "Renderer/video.hpp"
#include "SystemInterface/main_test.hpp"
#include "SystemInterface/system.hpp"
#include "SystemInterface/video.hpp"

int TimerTest()
{
    bool passed;
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
        { SDLK_ESCAPE, KEY_ESCAPE },
        { SDLK_RETURN, KEY_ENTER },
        { SDLK_LEFT, KEY_LEFTARROW },
        { SDLK_RIGHT, KEY_RIGHTARROW },
        { SDLK_UP, KEY_UPARROW },
        { SDLK_DOWN, KEY_DOWNARROW },
        { SDLK_F1, KEY_F1 },
        { SDLK_F2, KEY_F2 },
        { SDLK_F3, KEY_F3 },
        { SDLK_F4, KEY_F4 },
        { SDLK_F5, KEY_F5 },
        { SDLK_F6, KEY_F6 },
        { SDLK_F7, KEY_F7 },
        { SDLK_F8, KEY_F8 },
        { SDLK_F9, KEY_F9 },
        { SDLK_F10, KEY_F10 },
        { SDLK_F11, KEY_F11 },
        { SDLK_F12, KEY_F12 },
        { SDLK_LCTRL, KEY_RCTRL },
        { SDLK_LALT, KEY_RALT },
        { SDLK_LSHIFT, KEY_RSHIFT },
        { SDLK_w, 'w' },
        { SDLK_a, 'a' },
        { SDLK_s, 's' },
        { SDLK_d, 'd' },
    };
    int key_index;
    I_Init();
    V_Init();
    I_InitGraphics();

    if (!SDL_GetRelativeMouseMode())
        I_Error("KeyboardTest: relative mouse mode is disabled");

    for (key_index = 0; key_index < sizeof(keys) / sizeof(keys[0]); ++key_index)
    {
        SDL_Event key_event = { 0 };

        key_event.type = SDL_KEYDOWN;
        key_event.key.keysym.sym = keys[key_index].sdl_key;
        if (SDL_PushEvent(&key_event) < 0)
            I_Error("KeyboardTest: SDL key-down push failed: %s", SDL_GetError());

        key_event.type = SDL_KEYUP;
        if (SDL_PushEvent(&key_event) < 0)
            I_Error("KeyboardTest: SDL key-up push failed: %s", SDL_GetError());
    }

    I_StartTic();

    const std::vector<INPUTS> &pressedInputs = inputHandler.GetPressedInputs();
    if (pressedInputs.size() != sizeof(keys) / sizeof(keys[0]))
        I_Error("KeyboardTest: unexpected number of pressed inputs");

    for (key_index = 0; key_index < sizeof(keys) / sizeof(keys[0]); ++key_index)
    {
        if (static_cast<int>(pressedInputs[key_index]) != keys[key_index].doom_key ||
            inputHandler.IsDown(static_cast<INPUTS>(keys[key_index].doom_key)))
            I_Error("KeyboardTest: incorrect input for SDL key %d",
                    keys[key_index].sdl_key);
    }

    {
        SDL_Event mouse_event = { 0 };

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

        mouse_event.type = SDL_MOUSEWHEEL;
        mouse_event.wheel.y = -2;
        mouse_event.wheel.direction = SDL_MOUSEWHEEL_NORMAL;
        SDL_PushEvent(&mouse_event);

        I_StartTic();

        if (inputHandler.IsDown(INPUTS::MOUSE_LEFT) ||
            !inputHandler.IsPressed(INPUTS::MOUSE_LEFT) ||
            inputHandler.GetMouseMotion().x != 3 ||
            inputHandler.GetMouseMotion().y != 2 ||
            inputHandler.GetMouseMotion().wheel != -2)
            I_Error("KeyboardTest: incorrect mouse input");
    }

    {
        SDL_Event input_event = { 0 };

        input_event.type = SDL_KEYDOWN;
        input_event.key.keysym.sym = SDLK_LCTRL;
        SDL_PushEvent(&input_event);
        input_event.type = SDL_MOUSEBUTTONDOWN;
        input_event.button.button = SDL_BUTTON_RIGHT;
        SDL_PushEvent(&input_event);
        I_StartTic();

        if (!inputHandler.IsDown(INPUTS::CTRL) ||
            !inputHandler.IsDown(INPUTS::MOUSE_RIGHT))
            I_Error("KeyboardTest: input press state was not retained");

        input_event.type = SDL_WINDOWEVENT;
        input_event.window.event = SDL_WINDOWEVENT_FOCUS_LOST;
        SDL_PushEvent(&input_event);
        I_StartTic();

        if (inputHandler.IsDown(INPUTS::CTRL) ||
            inputHandler.IsDown(INPUTS::MOUSE_RIGHT) ||
            SDL_GetRelativeMouseMode())
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
