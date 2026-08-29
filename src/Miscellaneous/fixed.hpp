#pragma once
#include <limits.h>
#include <stdint.h>


// Fixed point, 32bit as 16.16.
#define FRACBITS 16
#define FRACUNIT (1 << FRACBITS)

    int32_t FixedMul(int32_t a, int32_t b);
    int32_t FixedDiv(int32_t a, int32_t b);
    int32_t FixedDiv2(int32_t a, int32_t b);
