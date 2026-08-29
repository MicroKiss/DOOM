#include "Miscellaneous/swap.hpp"

// Swap 16bit, that is, MSB and LSB byte.
int16_t SwapSHORT(int16_t value)
{
    uint16_t bits = (uint16_t)value;

    return (int16_t)((bits >> 8) | (bits << 8));
}

// Swapping 32bit.
int32_t SwapLONG(int32_t value)
{
    uint32_t bits = (uint32_t)value;

    return (int32_t)((bits >> 24) | ((bits >> 8) & UINT32_C(0x0000ff00)) | ((bits << 8) & UINT32_C(0x00ff0000)) | (bits << 24));
}
