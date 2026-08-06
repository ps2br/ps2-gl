/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2026 ps2br.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ps2gl_texture_h
#define ps2gl_texture_h

#include <GL/gl.h>
#include <tamtypes.h>

#include "ps2gl/types.h"

#define PS2_TEXTURES_MAX 256

typedef u8 GSTEXTURE; /** FIXME!!! */
typedef struct ps2_texture_t
{
    GSTEXTURE GTexture;
    int Width, Height;
    int Used;

    GLint MinFilter;
    GLint MagFilter;
} PS2_Texture;

typedef struct ps2_texture_context_t
{
    PS2_Texture Textures[PS2_TEXTURES_MAX];

    PS2_2DCoords CurrentTexCoords; // for immediate mode
    GLuint BoundTexture;

    PS2_GLArray CurrentTexCoordsArray;
} PS2_TextureContext;

#endif
