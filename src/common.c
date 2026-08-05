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

#include <debug.h>
#include <string.h>

void
glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz)
{
    gl.Draw.CurrentNormal[0] = nx;
    gl.Draw.CurrentNormal[1] = ny;
    gl.Draw.CurrentNormal[2] = nz;
}

/** TODO: implement all the OpenGL Capabilities */
void
glEnable (GLenum cap)
{
    switch (cap)
    {
    case GL_BLEND:
    case GL_COLOR_LOGIC_OP:
    case GL_CULL_FACE:
    case GL_DEPTH_TEST:
    case GL_DITHER:
    case GL_LINE_SMOOTH:
    case GL_POLYGON_OFFSET_FILL:
    case GL_POLYGON_OFFSET_LINE:
    case GL_POLYGON_OFFSET_POINT:
    case GL_POLYGON_SMOOTH:
    case GL_STENCIL_TEST:
        scr_printf ("PS2GL: Capability %d not implemented.\n", cap);
        break;
    case GL_TEXTURE_2D:
        gl.Caps.Texture2D = 1;
        break;
    case GL_SCISSOR_TEST:
        gl.Caps.Scissor = 1;
        break;
    }
}

/** TODO: implement all the OpenGL Capabilities */
void
glDisable (GLenum cap)
{
    switch (cap)
    {
    case GL_BLEND:
    case GL_COLOR_LOGIC_OP:
    case GL_CULL_FACE:
    case GL_DEPTH_TEST:
    case GL_DITHER:
    case GL_LINE_SMOOTH:
    case GL_POLYGON_OFFSET_FILL:
    case GL_POLYGON_OFFSET_LINE:
    case GL_POLYGON_OFFSET_POINT:
    case GL_POLYGON_SMOOTH:
    case GL_STENCIL_TEST:
        scr_printf ("PS2GL: Capability %d not implemented.\n", cap);
        break;
    case GL_TEXTURE_2D:
        gl.Caps.Texture2D = 0;
        break;
    case GL_SCISSOR_TEST:
        gl.Caps.Scissor = 0;
        break;
    }
}

void
glEnableClientState (GLenum cap)
{
    switch (cap)
    {
    case GL_EDGE_FLAG_ARRAY:
    case GL_INDEX_ARRAY:
    case GL_NORMAL_ARRAY:
        scr_printf ("PS2GL: ClientState %d not implemented.\n", cap);
        break;
    case GL_VERTEX_ARRAY:
        gl.Draw.CurrentVertexArray.Enabled = 1;
        break;
    case GL_COLOR_ARRAY:
        gl.Draw.CurrentColorArray.Enabled = 1;
        break;
    case GL_TEXTURE_COORD_ARRAY:
        gl.Tex.CurrentTexCoordsArray.Enabled = 1;
        break;
    }
}

void
glDisableClientState (GLenum cap)
{
    switch (cap)
    {
    case GL_EDGE_FLAG_ARRAY:
    case GL_INDEX_ARRAY:
    case GL_NORMAL_ARRAY:
        scr_printf ("PS2GL: ClientState %d not implemented.\n", cap);
        break;
    case GL_VERTEX_ARRAY:
        gl.Draw.CurrentVertexArray.Enabled = 0;
        break;
    case GL_COLOR_ARRAY:
        gl.Draw.CurrentColorArray.Enabled = 0;
        break;
    case GL_TEXTURE_COORD_ARRAY:
        gl.Tex.CurrentTexCoordsArray.Enabled = 0;
        break;
    }
}

void
glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{
    gl.Draw.CurrentNormalArray.Type = type;
    gl.Draw.CurrentNormalArray.Stride = stride;
    gl.Draw.CurrentNormalArray.Pointer = pointer;
    gl.Draw.CurrentNormalArray.Enabled = 1;
}

const GLubyte *
glGetString (GLenum name)
{
    switch (name)
    {
    case GL_VENDOR:
        return (const GLubyte *)"ps2br-ps2gl";
    case GL_VERSION:
        return (const GLubyte *)"1.1";
    case GL_RENDERER:
        return (const GLubyte *)"PS2-gsKit";
    }
    gl.CurrentError = GL_INVALID_ENUM;
    return NULL;
}

GLenum
glGetError (void)
{
    return gl.CurrentError;
}

void
glHint (GLenum target, GLenum mode)
{
    if (mode != GL_FASTEST && mode != GL_NICEST && mode != GL_DONT_CARE)
    {
        gl.CurrentError = GL_INVALID_ENUM;
        return;
    }

    switch (target)
    {
    case GL_LINE_SMOOTH_HINT:
        gl.Hints.LineSmooth = mode;
        break;
    case GL_POLYGON_SMOOTH_HINT:
        gl.Hints.PolygonSmooth = mode;
        break;
    case GL_POINT_SMOOTH_HINT:
        gl.Hints.PointSmooth = mode;
        break;
    case GL_PERSPECTIVE_CORRECTION_HINT:
        gl.Hints.PerpectiveCorrection = mode;
        break;
    case GL_FOG_HINT:
        gl.Hints.Fog = mode;
        break;
    default:
        gl.CurrentError = GL_INVALID_ENUM;
        return;
    }
}

void
glGetFloatv (GLenum pname, GLfloat *params)
{
    if (!params)
    {
        gl.CurrentError = GL_INVALID_VALUE;
        return;
    }

    switch (pname)
    {
    case GL_LINE_WIDTH:
        *params = gl.Draw.LineWidth;
        break;
    case GL_POINT_SIZE:
        *params = gl.Draw.PointSize;
        break;
    case GL_MODELVIEW_MATRIX:
        memcpy (params, gl.Matrix.ModelView, sizeof (gl.Matrix.ModelView));
        break;
    case GL_PROJECTION_MATRIX:
        memcpy (params, gl.Matrix.Projection, sizeof (gl.Matrix.ModelView));
        break;
    default:
        gl.CurrentError = GL_INVALID_ENUM;
        return;
    }
}

void
glPolygonMode (GLenum face, GLenum mode)
{
    if (mode != GL_POINT && mode != GL_LINE && mode != GL_FILL)
    {
        gl.CurrentError = GL_INVALID_ENUM;
        return;
    }

    switch (face)
    {
    case GL_FRONT:
        gl.Draw.Polygon.FrontMode = mode;
        break;
    case GL_BACK:
        gl.Draw.Polygon.BackMode = mode;
        break;
    case GL_FRONT_AND_BACK:
        gl.Draw.Polygon.FrontMode = mode;
        gl.Draw.Polygon.BackMode = mode;
        break;
    default:
        gl.CurrentError = GL_INVALID_ENUM;
        return;
    };
}

void
glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
    gl.Draw.Scissor.X = x;
    gl.Draw.Scissor.Y = y;
    gl.Draw.Scissor.Width = width;
    gl.Draw.Scissor.Height = height;
}
