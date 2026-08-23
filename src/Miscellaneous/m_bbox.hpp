#pragma once

#include "doomtype.h"

#include "m_fixed.hpp"

// Bounding box coordinate storage.
enum
{
    BOXTOP,
    BOXBOTTOM,
    BOXLEFT,
    BOXRIGHT
}; // bbox coordinates

typedef struct
{
    int32_t top;
    int32_t bottom;
    int32_t left;
    int32_t right;
} BBox;

#ifdef __cplusplus
extern "C"
{
#endif

    void M_ClearBox(BBox *box);

    void M_AddToBox(BBox *box,
                    int32_t x,
                    int32_t y);

#ifdef __cplusplus
}
#endif
