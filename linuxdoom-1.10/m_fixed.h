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
//	Fixed point arithemtics, implementation.
//
//-----------------------------------------------------------------------------

#ifndef __M_FIXED__
#define __M_FIXED__

#include <limits.h>
#include <stdint.h>

#ifdef __GNUG__
#pragma interface
#endif

//
// Fixed point, 32bit as 16.16.
//
#define FRACBITS 16
#define FRACUNIT (1 << FRACBITS)

int32_t FixedMul(int32_t a, int32_t b);
int32_t FixedDiv(int32_t a, int32_t b);
int32_t FixedDiv2(int32_t a, int32_t b);

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
