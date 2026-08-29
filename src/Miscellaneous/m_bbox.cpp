#include "m_bbox.hpp"

void M_ClearBox(BBox *box)
{
    box->top = box->right = MININT;
    box->bottom = box->left = MAXINT;
}

void M_AddToBox(BBox *box, int32_t x, int32_t y)
{
    if (x < box->left)
        box->left = x;
    else if (x > box->right)
        box->right = x;
    if (y < box->bottom)
        box->bottom = y;
    else if (y > box->top)
        box->top = y;
}
