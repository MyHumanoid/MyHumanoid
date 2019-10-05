/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005-2007  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  3  of
 *  the License, or (at your option) any later version.
 *
 *  This  program  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the  implied  warranty  of
 *  MERCHANTABILITY  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foun-
 *  dation, Inc., 59 Temple Place, Suite 330, Boston,  MA  02111-1307
 *  USA
 *
 *  File   : ComponentID.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include <cstdint>


// Construct a integer composed from a four char literal.
// FOUR_CHAR_CONST('D','U','M','Y'); is a substitute of the uint32_t literal
// 'DUMY'
#ifndef FOUR_CHAR_CONST
#define FOUR_CHAR_CONST(a, b, c, d)                                                                \
	(((char)(a)) << 24 | ((char)(b)) << 16 | ((char)(c)) << 8 | ((char)(d)))
#endif

static const uint32_t kComponentID_Dummy = FOUR_CHAR_CONST('D', 'M', 'M', 'Y');

// ------------------------------------------------------------------
// The Character Setting Panel
// ------------------------------------------------------------------
static const uint32_t kComponentID_CharacterSettingPanel = FOUR_CHAR_CONST('p', 'a', 'C', 'A');

static const uint32_t kAge = FOUR_CHAR_CONST('A', 'g', 'e', 'X');
static const uint32_t kMuscleSize =
        FOUR_CHAR_CONST('M', 's', 'S', 'z');
static const uint32_t kBreast =
        FOUR_CHAR_CONST('B', 'r', 's', 't');
static const uint32_t kShape =
        FOUR_CHAR_CONST('S', 'h', 'p', 'a');
