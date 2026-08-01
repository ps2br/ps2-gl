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

#include <GL/gl.h>

#include "ps2gl/context.h"

static void
PS2_SetVertexState (PS2_Vertex *v)
{
    v->Color[0] = gl.Draw.CurrentColor[0];
    v->Color[1] = gl.Draw.CurrentColor[1];
    v->Color[2] = gl.Draw.CurrentColor[2];
    v->Color[3] = gl.Draw.CurrentColor[3];

    v->Normal[0] = gl.Draw.CurrentNormal[0];
    v->Normal[1] = gl.Draw.CurrentNormal[1];
    v->Normal[2] = gl.Draw.CurrentNormal[2];

    v->TexCoords[0] = gl.Tex.CurrentTexCoords[0];
    v->TexCoords[1] = gl.Tex.CurrentTexCoords[1];
}

void
glVertex2i (int x, int y)
{
    if (gl.Draw.VertexCount >= PS2_DRAW_MAX_VERTICES)
        return;

    PS2_Vertex *v = &gl.Draw.Vertices[gl.Draw.VertexCount++];
    v->Coords[0] = x;
    v->Coords[1] = y;
    v->Coords[2] = 0;

    PS2_SetVertexState (v);
}

void
glVertex2f (GLfloat x, GLfloat y)
{
    glVertex3f (x, y, 0.f);
}

void
glVertex3f (GLfloat x, GLfloat y, GLfloat z)
{
    if (gl.Draw.VertexCount >= PS2_DRAW_MAX_VERTICES)
        return;

    PS2_Vertex *v = &gl.Draw.Vertices[gl.Draw.VertexCount++];
    v->Coords[0] = x;
    v->Coords[1] = y;
    v->Coords[2] = z;

    PS2_SetVertexState (v);
}

void
glVertexPointer (GLint size, GLenum type, GLsizei stride,
                 const GLvoid *pointer)
{
    gl.Draw.CurrentVertexArray.Size = size;
    gl.Draw.CurrentVertexArray.Type = type;
    gl.Draw.CurrentVertexArray.Stride = stride;
    gl.Draw.CurrentVertexArray.Pointer = pointer;
}
