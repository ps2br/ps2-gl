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
#include <tamtypes.h>

#include "ps2gl/context.h"
#include "ps2gl/render.h"

void
glClearColor (GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
    if (r < 0.f)
        r = 0.f;
    if (r > 1.f)
        r = 1.f;

    if (g < 0.f)
        g = 0.f;
    if (g > 1.f)
        g = 1.f;

    if (b < 0.f)
        b = 0.f;
    if (b > 1.f)
        b = 1.f;

    if (a < 0.f)
        a = 0.f;
    if (a > 1.f)
        a = 1.f;

    gl.Draw.CurrentColor[0] = r;
    gl.Draw.CurrentColor[1] = g;
    gl.Draw.CurrentColor[2] = b;
    gl.Draw.CurrentColor[3] = a;
}

void
glClear (GLbitfield mask)
{
    if (mask & GL_COLOR_BUFFER_BIT)
        PS2GL_ClearRenderColor (gl.Renderer, gl.Draw.CurrentColor);
}
