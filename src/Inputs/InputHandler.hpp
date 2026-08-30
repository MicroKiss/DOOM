#pragma once

#include "Inputs.hpp"
#include <set>

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

  private:
    std::set<INPUTS> inputs;
    std::set<INPUTS> pressedThisFrame;
};

inline InputHandler inputHandler;