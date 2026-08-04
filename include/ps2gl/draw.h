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

typedef struct ps2_vertex_t
{
    PS2_3DCoords Coords;
    PS2_3DCoords Normal;
    PS2_2DCoords TexCoords;
    PS2_Color Color;
} PS2_Vertex;

typedef struct ps2_polygon_t
{
    GLenum FrontMode;
    GLenum BackMode;
} PS2_Polygon;

#define PS2_DRAW_MAX_VERTICES 4096

typedef struct ps2_draw_context_t
{
    PS2_Vertex Vertices[PS2_DRAW_MAX_VERTICES];
    int VertexCount;

    PS2_Color CurrentColor;
    PS2_3DCoords CurrentNormal;

    GLenum CurrentMode;

    GLfloat LineWidth;
    GLfloat PointSize;

    PS2_Polygon Polygon;

    /** used for glDrawArrays */
    PS2_GLArray CurrentVertexArray;
    PS2_GLArray_NoSize CurrentNormalArray;
    PS2_GLArray CurrentColorArray;
} PS2_DrawContext;

#endif
