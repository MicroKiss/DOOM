#include "InputHandler.hpp"
#include "Doom/main.hpp"

static bool IsMouseButton(INPUTS input)
{
    return input == INPUTS::MOUSE_LEFT ||
           input == INPUTS::MOUSE_MIDDLE ||
           input == INPUTS::MOUSE_RIGHT;
}

static void PostKeyEvent(evtype_t type, INPUTS input)
{
    event_t event = {};
    event.type = type;
    event.data1 = static_cast<int>(input);
    D_PostEvent(&event);
}

static void PostMouseEvent(const InputHandler &handler, int x, int y)
{
    event_t event = {};
    event.type = ev_mouse;
    event.data1 = (handler.IsDown(INPUTS::MOUSE_LEFT) ? 1 : 0) |
                  (handler.IsDown(INPUTS::MOUSE_MIDDLE) ? 2 : 0) |
                  (handler.IsDown(INPUTS::MOUSE_RIGHT) ? 4 : 0);
    event.data2 = x;
    event.data3 = y;
    D_PostEvent(&event);
}

void InputHandler::Update()
{
    pressedThisFrame.clear();
    mouseMotion = {};
}

bool InputHandler::IsPressed(INPUTS input) const
{
    switch (input)
    {
    case INPUTS::ATTACK:
        return IsPressed(INPUTS::CTRL) || IsPressed(INPUTS::MOUSE_LEFT);
    case INPUTS::USE:
        return IsPressed(static_cast<INPUTS>(' '));
    case INPUTS::NEXT_WEAPON:
        return mouseMotion.wheel != 0;
    case INPUTS::WEAPON_1:
    case INPUTS::WEAPON_2:
    case INPUTS::WEAPON_3:
    case INPUTS::WEAPON_4:
    case INPUTS::WEAPON_5:
    case INPUTS::WEAPON_6:
    case INPUTS::WEAPON_7:
    case INPUTS::WEAPON_8:
        return IsPressed(static_cast<INPUTS>('1' + input - INPUTS::WEAPON_1));
    default:
        break;
    }

    return pressedThisFrame.find(input) != pressedThisFrame.end();
}

bool InputHandler::IsDown(INPUTS input) const
{
    switch (input)
    {
    case INPUTS::MOVE_FORWARD:
        return inputs.find(INPUTS::MOVE_FORWARD) != inputs.end() || inputs.find(INPUTS::W) != inputs.end();
    case INPUTS::MOVE_BACKWARD:
        return inputs.find(INPUTS::MOVE_BACKWARD) != inputs.end() || inputs.find(INPUTS::S) != inputs.end();
    case INPUTS::MOVE_LEFT:
        return inputs.find(INPUTS::MOVE_LEFT) != inputs.end() || inputs.find(INPUTS::A) != inputs.end();
    case INPUTS::MOVE_RIGHT:
        return inputs.find(INPUTS::MOVE_RIGHT) != inputs.end() || inputs.find(INPUTS::D) != inputs.end();
    case INPUTS::ATTACK:
        return IsDown(INPUTS::CTRL) || IsDown(INPUTS::MOUSE_LEFT);
    case INPUTS::USE:
        return IsDown(static_cast<INPUTS>(' '));
    default:
        break;
    }

    return inputs.find(input) != inputs.end();
}

void InputHandler::Press(INPUTS input)
{
    pressedThisFrame.insert(input);
    inputs.insert(input);

    if (IsMouseButton(input))
        PostMouseEvent(*this, 0, 0);
    else
        PostKeyEvent(ev_keydown, input);
}

void InputHandler::Release(INPUTS input)
{
    inputs.erase(input);

    if (IsMouseButton(input))
        PostMouseEvent(*this, 0, 0);
    else
        PostKeyEvent(ev_keyup, input);
}

void InputHandler::ReleaseAll()
{
    while (!inputs.empty())
        Release(*inputs.begin());
}

void InputHandler::AddMouseMotion(int x, int y)
{
    mouseMotion.x += x;
    mouseMotion.y += y;
    PostMouseEvent(*this, x, y);
}

void InputHandler::AddMouseWheel(int amount)
{
    event_t event = {};

    mouseMotion.wheel += amount;
    event.type = ev_mousewheel;
    event.data1 = amount;
    D_PostEvent(&event);
}

const MouseMotion &InputHandler::GetMouseMotion() const
{
    return mouseMotion;
}