#include <stdlib.h>

#include "doomtype.hpp"
#include "i_system.hpp"
#include "m_fixed.hpp"

int32_t FixedMul(int32_t a,
                 int32_t b)
{
    return ((long long)a * (long long)b) >> FRACBITS;
}

//
// FixedDiv, C version.
//

int32_t FixedDiv(int32_t a, int32_t b)
{
    if ((abs(a) >> 14) >= abs(b))
        return (a ^ b) < 0 ? MININT : MAXINT;
    return FixedDiv2(a, b);
}

int32_t
FixedDiv2(int32_t a,
          int32_t b)
{
    double c;

    c = ((double)a) / ((double)b) * FRACUNIT;

    if (c >= 2147483648.0 || c < -2147483648.0)
        I_Error("FixedDiv: divide by zero");
    return (int32_t)c;
}
