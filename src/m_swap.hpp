#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int16_t SwapSHORT(int16_t value);
    int32_t SwapLONG(int32_t value);

#ifdef __cplusplus
}
#endif

#ifdef __BIG_ENDIAN__
#define SHORT(x) SwapSHORT((int16_t)(x))
#define LONG(x) SwapLONG((int32_t)(x))
#else
#define SHORT(x) (x)
#define LONG(x) (x)
#endif
