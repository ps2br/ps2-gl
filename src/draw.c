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

#include "ps2gl/draw.h"

#include <GL/gl.h>
#include <debug.h>
#include <stddef.h>

#include "ps2gl/context.h"
#include "ps2gl/render.h"
#include "ps2gl/texture.h"
#include "ps2gl/types.h"

void
glBegin (GLenum mode)
{
    gl.Draw.VertexCount = 0;
    gl.Draw.CurrentMode = mode;
}

static void
PS2GL_DrawTriangle (PS2GL_Vertex *a, PS2GL_Vertex *b, PS2GL_Vertex *c,
                    GLenum polygon_mode)
{
    if (polygon_mode == GL_FILL)
    {
        if (gl.Caps.Texture2D && gl.Tex.BoundTexture != 0)
        {
            PS2GL_Texture *texture = &gl.Tex.Textures[gl.Tex.BoundTexture];
            gl.Renderer->DrawFilledTexturedTriangle (
                gl.Renderer, gl.Draw.CurrentShadeModel, a, b, c,
                texture->ImplTexture);
            return;
        }
        gl.Renderer->DrawFilledTriangle (gl.Renderer,
                                         gl.Draw.CurrentShadeModel, a, b, c);
        return;
    }

    /** TODO: implement GL_LINES and GL_POINTS */
    if (polygon_mode == GL_LINE)
    {
    }
    if (polygon_mode == GL_POINT)
    {
    }

    scr_printf ("PS2GL: PS2GL_DrawTriangle, Mode %d not implemented.",
                polygon_mode);
}

static void
PS2GL_ApplyLighting (PS2GL_Vertex *v)
{
    float lx = 0.f;
    float ly = 0.f;
    float lz = 1.f;

    float dot = v->Normal[0] * lx + v->Normal[1] * ly + v->Normal[2] * lz;

    if (dot < 0.f)
        dot = 0.f;

    v->Color[0] *= dot;
    v->Color[1] *= dot;
    v->Color[2] *= dot;
}

void
glEnd (void)
{
    /** only Triangles supported now */
    if (gl.Draw.CurrentMode != GL_TRIANGLES)
        return;

    if (gl.Caps.Scissor)
        gl.Renderer->SetScissor (gl.Renderer, gl.Draw.Scissor.X,
                                 gl.Draw.Scissor.Y, gl.Draw.Scissor.Width,
                                 gl.Draw.Scissor.Height);
    else
        gl.Renderer->ResetScissor (gl.Renderer);

    gl.Renderer->SetDepthMask (gl.Renderer, gl.Caps.DepthBufferEnable);

    for (int i = 0; i < gl.Draw.VertexCount; i += 3)
    {
        PS2GL_Vertex va = gl.Draw.Vertices[i + 0];
        PS2GL_Vertex vb = gl.Draw.Vertices[i + 1];
        PS2GL_Vertex vc = gl.Draw.Vertices[i + 2];

        PS2GL_TransformVertex (va.Coords[0], va.Coords[1], va.Coords[2],
                               &va.Coords[0], &va.Coords[1], &va.Coords[2]);

        PS2GL_TransformVertex (vb.Coords[0], vb.Coords[1], vb.Coords[2],
                               &vb.Coords[0], &vb.Coords[1], &vb.Coords[2]);

        PS2GL_TransformVertex (vc.Coords[0], vc.Coords[1], vc.Coords[2],
                               &vc.Coords[0], &vc.Coords[1], &vc.Coords[2]);

        va.Coords[0] = (va.Coords[0] + 1.0f) * 0.5f * gl.ViewportWidth;
        va.Coords[1] = (1.0f - va.Coords[1]) * 0.5f * gl.ViewportHeight;
        va.Coords[2] = (va.Coords[2] + 1.0f) * 0.5f;

        vb.Coords[0] = (vb.Coords[0] + 1.0f) * 0.5f * gl.ViewportWidth;
        vb.Coords[1] = (1.0f - vb.Coords[1]) * 0.5f * gl.ViewportHeight;
        vb.Coords[2] = (vb.Coords[2] + 1.0f) * 0.5f;

        vc.Coords[0] = (vc.Coords[0] + 1.0f) * 0.5f * gl.ViewportWidth;
        vc.Coords[1] = (1.0f - vc.Coords[1]) * 0.5f * gl.ViewportHeight;
        vc.Coords[2] = (vc.Coords[2] + 1.0f) * 0.5f;

        PS2GL_ApplyLighting (&va);
        PS2GL_ApplyLighting (&vb);
        PS2GL_ApplyLighting (&vc);

        float cross
            = (vb.Coords[0] - va.Coords[0]) * (vc.Coords[1] - va.Coords[1])
              - (vb.Coords[1] - va.Coords[1]) * (vc.Coords[0] - va.Coords[0]);

        int front = cross < 0;

        GLenum mode
            = front ? gl.Draw.Polygon.FrontMode : gl.Draw.Polygon.BackMode;

        PS2GL_DrawTriangle (&va, &vb, &vc, mode);
    }
}

static void
PS2GL_LoadColorArr (int index)
{
    const char *base = gl.Draw.CurrentColorArray.Pointer;

    size_t stride = gl.Draw.CurrentColorArray.Stride;
    if (stride == 0)
        stride = gl.Draw.CurrentColorArray.Size * sizeof (GLfloat);

    const GLfloat *c = (const GLfloat *)(base + index * stride);
    switch (gl.Draw.CurrentColorArray.Size)
    {
    case 3:
        glColor3f (c[0], c[1], c[2]);
        break;
    case 4:
        glColor4f (c[0], c[1], c[2], c[3]);
        break;
    }
}

static void
PS2GL_LoadTexCoordsArr (int index)
{
    const char *base = gl.Tex.CurrentTexCoordsArray.Pointer;

    size_t stride = gl.Tex.CurrentTexCoordsArray.Stride;
    if (stride == 0)
        stride = gl.Tex.CurrentTexCoordsArray.Size * sizeof (GLfloat);

    const GLfloat *tc = (const GLfloat *)(base + index * stride);
    switch (gl.Tex.CurrentTexCoordsArray.Size)
    {
    case 2:
        glTexCoord2f (tc[0], tc[1]);
        break;
    }
}

static void
PS2GL_LoadNormalArr (int index)
{
    const char *base = gl.Draw.CurrentNormalArray.Pointer;

    size_t stride = gl.Draw.CurrentNormalArray.Stride;
    if (stride == 0)
        stride = 3 * sizeof (GLfloat);

    const GLfloat *n = (const GLfloat *)(base + index * stride);
    glNormal3f (n[0], n[1], n[2]);
}

static void
PS2GL_LoadVertexArr (int index)
{
    const char *base = gl.Draw.CurrentVertexArray.Pointer;

    size_t stride = gl.Draw.CurrentVertexArray.Stride;
    if (stride == 0)
        stride = gl.Draw.CurrentVertexArray.Size * sizeof (GLfloat);

    const GLfloat *v = (const GLfloat *)(base + index * stride);
    switch (gl.Draw.CurrentVertexArray.Size)
    {
    case 2:
        glVertex3f (v[0], v[1], 0.f);
        break;
    case 3:
        glVertex3f (v[0], v[1], v[2]);
        break;
    }
}

void
glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
    if (!gl.Draw.CurrentVertexArray.Enabled)
    {
        gl.CurrentError = GL_INVALID_OPERATION;
        return;
    }

    glBegin (mode);
    for (GLsizei i = 0; i < count; i++)
    {
        int idx = first + i;
        if (gl.Draw.CurrentColorArray.Enabled)
            PS2GL_LoadColorArr (idx);

        if (gl.Tex.CurrentTexCoordsArray.Enabled)
            PS2GL_LoadTexCoordsArr (idx);

        if (gl.Draw.CurrentNormalArray.Enabled)
            PS2GL_LoadNormalArr (idx);

        PS2GL_LoadVertexArr (idx);
    }
    glEnd ();
}

void
glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    if (!gl.Draw.CurrentVertexArray.Enabled)
    {
        gl.CurrentError = GL_INVALID_OPERATION;
        return;
    }

    glBegin (mode);
    for (GLsizei i = 0; i < count; i++)
    {
        GLuint idx;
        switch (type)
        {
        case GL_UNSIGNED_BYTE:
            idx = ((const GLubyte *)indices)[i];
            break;
        case GL_UNSIGNED_SHORT:
            idx = ((const GLushort *)indices)[i];
            break;
        case GL_UNSIGNED_INT:
            idx = ((const GLuint *)indices)[i];
            break;
        default:
            gl.CurrentError = GL_INVALID_ENUM;
            return;
        }

        if (gl.Draw.CurrentColorArray.Enabled)
            PS2GL_LoadColorArr (idx);

        if (gl.Tex.CurrentTexCoordsArray.Enabled)
            PS2GL_LoadTexCoordsArr (idx);

        if (gl.Draw.CurrentNormalArray.Enabled)
            PS2GL_LoadNormalArr (idx);

        PS2GL_LoadVertexArr (idx);
    }
    glEnd ();
}
