// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	Endianess handling, swapping 16bit and 32bit.
//
//-----------------------------------------------------------------------------

#ifndef __M_SWAP__
#define __M_SWAP__

#include <stdint.h>

#ifdef __GNUG__
#pragma interface
#endif

// Endianess handling.
// WAD files are stored little endian.
int16_t SwapSHORT(int16_t value);
int32_t SwapLONG(int32_t value);

#ifdef __BIG_ENDIAN__
#define SHORT(x) SwapSHORT((int16_t)(x))
#define LONG(x) SwapLONG((int32_t)(x))
#else
#define SHORT(x) (x)
#define LONG(x) (x)
#endif

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
