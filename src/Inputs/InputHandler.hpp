#pragma once

#include "Inputs.hpp"
#include <set>
#include <vector>

struct MouseMotion
{
    int x = 0;
    int y = 0;
    int wheel = 0;
};

class InputHandler
{
  public:
    InputHandler() = default;
    ~InputHandler() = default;
    void Update();
    bool IsPressed(INPUTS input) const;
    bool IsDown(INPUTS input) const;
    void Press(INPUTS input);
    void Release(INPUTS input);
    void ReleaseAll();
    void AddMouseMotion(int x, int y);
    void AddMouseWheel(int amount);
    const std::vector<INPUTS> &GetPressedInputs() const;
    const MouseMotion &GetMouseMotion() const;

  private:
    std::set<INPUTS> inputs;
    std::set<INPUTS> pressedThisFrame;
    std::vector<INPUTS> pressedInputs;
    MouseMotion mouseMotion;
};

inline InputHandler inputHandler;