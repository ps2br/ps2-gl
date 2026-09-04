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

#ifndef ps2gl_draw_h
#define ps2gl_draw_h

#include <GL/gl.h>

#include "ps2gl/types.h"

typedef struct ps2_polygon_t
{
    GLenum FrontMode;
    GLenum BackMode;
} PS2GL_Polygon;

typedef struct ps2_scissor
{
    GLint X, Y;
    GLsizei Width, Height;
} PS2GL_Scissor;

#define PS2GL_DRAW_MAX_VERTICES 4096

typedef struct ps2_draw_context_t
{
    PS2GL_Vertex Vertices[PS2GL_DRAW_MAX_VERTICES];
    int VertexCount;

    PS2GL_Color CurrentColor;
    PS2GL_3DCoords CurrentNormal;

    GLenum CurrentMode;
    GLenum CurrentShadeModel;

    GLfloat LineWidth;
    GLfloat PointSize;

    PS2GL_Polygon Polygon;
    PS2GL_Scissor Scissor;

    /** used for glDrawArrays */
    PS2GL_Array CurrentVertexArray;
    PS2GL_Array_NoSize CurrentNormalArray;
    PS2GL_Array CurrentColorArray;
} PS2GL_DrawContext;

#endif
