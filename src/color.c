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

void
glColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
    gl.Draw.CurrentColor[0] = red;
    gl.Draw.CurrentColor[1] = green;
    gl.Draw.CurrentColor[2] = blue;
}

void
glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    gl.Draw.CurrentColor[0] = red;
    gl.Draw.CurrentColor[1] = green;
    gl.Draw.CurrentColor[2] = blue;
    gl.Draw.CurrentColor[3] = alpha;
}

void
glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    gl.Draw.CurrentColor[0] = red;
    gl.Draw.CurrentColor[1] = green;
    gl.Draw.CurrentColor[2] = blue;
    gl.Draw.CurrentColor[3] = alpha;
}

void
glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    gl.Draw.CurrentColorArray.Size = size;
    gl.Draw.CurrentColorArray.Type = type;
    gl.Draw.CurrentColorArray.Stride = stride;
    gl.Draw.CurrentColorArray.Enabled = 1;
}
