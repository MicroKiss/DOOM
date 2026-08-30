#include "InputHandler.hpp"
#include "Doom/main.hpp"

static void PostKeyEvent(evtype_t type, INPUTS input)
{
    event_t event = {};
    event.type = type;
    event.data1 = static_cast<int>(input);
    D_PostEvent(&event);
}

void InputHandler::Update()
{
    pressedThisFrame.clear();
}

bool InputHandler::IsPressed(INPUTS input) const
{
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
    default:
        break;
    }

    return inputs.find(input) != inputs.end();
}

void InputHandler::Press(INPUTS input)
{
    pressedThisFrame.insert(input);
    inputs.insert(input);
    PostKeyEvent(ev_keydown, input);
}

void InputHandler::Release(INPUTS input)
{
    inputs.erase(input);
    PostKeyEvent(ev_keyup, input);
}

void InputHandler::ReleaseAll()
{
    while (!inputs.empty())
        Release(*inputs.begin());
}