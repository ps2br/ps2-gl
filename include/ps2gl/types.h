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

#ifndef ps2gl_types_h
#define ps2gl_types_h

#include <GL/gl.h>

typedef GLfloat PS2_3DCoords[3];
typedef GLfloat PS2_2DCoords[2];
typedef GLfloat PS2_Color[4];

typedef struct ps2_gl_array_t
{
    GLint Size;
    GLenum Type;
    GLsizei Stride;
    const void *Pointer;
    GLboolean Enabled : 1;
} __attribute__ ((packed)) PS2_GLArray;

typedef struct ps2_gl_array_no_size_t
{
    GLenum Type;
    GLsizei Stride;
    const void *Pointer;
    GLboolean Enabled : 1;
} __attribute__ ((packed)) PS2_GLArray_NoSize;


#endif
