#pragma once

#include "doomtype.hpp"

extern int rndindex;

int M_Random(void);

// As M_Random, but used only by the play simulation.
int P_Random(void);

// Deterministic random sequence.
void M_ClearRandom(void);
