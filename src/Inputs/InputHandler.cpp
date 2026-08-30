#include "InputHandler.hpp"

void InputHandler::Update()
{
    pressedThisFrame.clear();
    pressedInputs.clear();
    mouseMotion = {};
}

bool InputHandler::IsPressed(INPUTS input) const
{
    switch (input)
    {
    case INPUTS::ATTACK:
        return IsPressed(INPUTS::MOUSE_LEFT);
    case INPUTS::USE:
        return IsPressed(static_cast<INPUTS>('e'));
    case INPUTS::JUMP:
        return IsPressed(static_cast<INPUTS>(' '));
    case INPUTS::NEXT_WEAPON:
        return mouseMotion.wheel > 0;
    case INPUTS::PREV_WEAPON:
        return mouseMotion.wheel < 0;
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
        return IsDown(INPUTS::MOUSE_LEFT);
    case INPUTS::USE:
        return IsDown(static_cast<INPUTS>('e'));
    case INPUTS::JUMP:
        return IsDown(static_cast<INPUTS>(' '));
    default:
        break;
    }

    return inputs.find(input) != inputs.end();
}

void InputHandler::Press(INPUTS input)
{
    pressedThisFrame.insert(input);
    pressedInputs.push_back(input);
    inputs.insert(input);
}

void InputHandler::Release(INPUTS input)
{
    inputs.erase(input);
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
}

void InputHandler::AddMouseWheel(int amount)
{
    mouseMotion.wheel += amount;
}

const std::vector<INPUTS> &InputHandler::GetPressedInputs() const
{
    return pressedInputs;
}

const MouseMotion &InputHandler::GetMouseMotion() const
{
    return mouseMotion;
}