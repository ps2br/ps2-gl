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
#include <gsKit.h>
#include <math.h>
#include <math3d.h>
#include <stddef.h>

#include "ps2gl/context.h"
#include "ps2gl/types.h"

void
glBegin (GLenum mode)
{
    gl.Draw.VertexCount = 0;
    gl.Draw.CurrentMode = mode;
}

#define COLOR8(x) ((u8)(fminf (fmaxf ((x), 0.0f), 1.0f) * 255.0f))

static void
PS2GL_DrawFilledTriangleWithTexture (PS2GL_Vertex *va, PS2GL_Vertex *vb,
                                   PS2GL_Vertex *vc, PS2GL_3DCoords *a,
                                   PS2GL_3DCoords *b, PS2GL_3DCoords *c)
{
    GSTEXTURE *texture = &gl.Tex.Textures[gl.Tex.BoundTexture].GTexture;

    PS2GL_2DCoords tex_a;
    tex_a[0] = va->TexCoords[0] * texture->Width;
    tex_a[1] = va->TexCoords[1] * texture->Height;

    PS2GL_2DCoords tex_b;
    tex_b[0] = vb->TexCoords[0] * texture->Width;
    tex_b[1] = vb->TexCoords[1] * texture->Height;

    PS2GL_2DCoords tex_c;
    tex_c[0] = vc->TexCoords[0] * texture->Width;
    tex_c[1] = vc->TexCoords[1] * texture->Height;

    gsKit_prim_triangle_goraud_texture_3d (
        gl.Gs,

        texture,

        *a[0], *a[1], *a[2], tex_a[0], tex_a[1],

        *b[0], *b[1], *b[2], tex_b[0], tex_b[1],

        *c[0], *c[1], *c[2], tex_c[0], tex_c[1],

        GS_SETREG_RGBAQ (COLOR8 (va->Color[0]), COLOR8 (va->Color[1]),
                         COLOR8 (va->Color[2]), COLOR8 (va->Color[3]), 0),

        GS_SETREG_RGBAQ (COLOR8 (vb->Color[0]), COLOR8 (vb->Color[1]),
                         COLOR8 (vb->Color[2]), COLOR8 (vb->Color[3]), 0),

        GS_SETREG_RGBAQ (COLOR8 (vc->Color[0]), COLOR8 (vc->Color[1]),
                         COLOR8 (vc->Color[2]), COLOR8 (vc->Color[3]), 0));
}

static void
PS2GL_DrawFilledTriangle (PS2GL_Vertex *va, PS2GL_Vertex *vb, PS2GL_Vertex *vc,
                        PS2GL_3DCoords *a, PS2GL_3DCoords *b, PS2GL_3DCoords *c)
{
    /** with texture */
    if (gl.Caps.Texture2D && gl.Tex.BoundTexture != 0)
    {
        PS2GL_DrawFilledTriangleWithTexture (va, vb, vc, a, b, c);
        return;
    }

    /** common colored */
    gsKit_prim_triangle_gouraud_3d (
        gl.Gs,

        *a[0], *a[1], *a[2], *b[0], *b[2], *b[2], *c[0], *c[1], *c[2],

        GS_SETREG_RGBAQ (COLOR8 (va->Color[0]), COLOR8 (va->Color[1]),
                         COLOR8 (va->Color[2]), COLOR8 (va->Color[3]), 0),

        GS_SETREG_RGBAQ (COLOR8 (vb->Color[0]), COLOR8 (vb->Color[1]),
                         COLOR8 (vb->Color[2]), COLOR8 (vb->Color[3]), 0),

        GS_SETREG_RGBAQ (COLOR8 (vc->Color[0]), COLOR8 (vc->Color[1]),
                         COLOR8 (vc->Color[2]), COLOR8 (vc->Color[3]), 0));
}

static void
PS2GL_DrawTriangle (PS2GL_Vertex *va, PS2GL_Vertex *vb, PS2GL_Vertex *vc,
                  PS2GL_3DCoords *a, PS2GL_3DCoords *b, PS2GL_3DCoords *c,
                  GLenum polygon_mode)
{
    if (polygon_mode == GL_FILL)
    {
        PS2GL_DrawFilledTriangle (va, vb, vc, a, b, c);
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
    {
        gsKit_set_scissor (
            gl.Gs, GS_SETREG_SCISSOR (
                       gl.Draw.Scissor.X,
                       gl.Draw.Scissor.X + gl.Draw.Scissor.Width - 1,
                       gl.Draw.Scissor.Y,
                       gl.Draw.Scissor.Y + gl.Draw.Scissor.Height - 1));
    }
    else
    {
        gsKit_set_scissor (gl.Gs, GS_SCISSOR_RESET);
    }

    for (int i = 0; i < gl.Draw.VertexCount; i += 3)
    {
        PS2GL_Vertex *a = &gl.Draw.Vertices[i + 0];
        PS2GL_Vertex *b = &gl.Draw.Vertices[i + 1];
        PS2GL_Vertex *c = &gl.Draw.Vertices[i + 2];

        PS2GL_3DCoords _a;
        PS2GL_3DCoords _b;
        PS2GL_3DCoords _c;

        PS2GL_TransformVertex (a->Coords[0], a->Coords[1], a->Coords[2], &_a[0],
                             &_a[2], &_a[2]);
        PS2GL_TransformVertex (b->Coords[0], b->Coords[1], b->Coords[2], &_b[0],
                             &_b[1], &_b[2]);
        PS2GL_TransformVertex (c->Coords[0], c->Coords[1], c->Coords[2], &_c[0],
                             &_c[1], &_c[2]);

        _a[0] = (_a[0] + 1.0f) * 0.5f * gl.ViewportWidth;
        _a[1] = (1.0f - _a[1]) * 0.5f * gl.ViewportHeight;
        _a[2] = ((_a[2]) + 1.f) * .5f;

        _b[0] = (_b[0] + 1.0f) * 0.5f * gl.ViewportWidth;
        _b[1] = (1.0f - _b[1]) * 0.5f * gl.ViewportHeight;
        _b[2] = ((_b[2]) + 1.f) * .5f;

        _c[0] = (_c[0] + 1.0f) * 0.5f * gl.ViewportWidth;
        _c[1] = (1.0f - _c[1]) * 0.5f * gl.ViewportHeight;
        _c[2] = ((_c[2]) + 1.f) * .5f;

        /** make a copy to not modify the og vertex*/
        PS2GL_Vertex va = *a;
        PS2GL_Vertex vb = *b;
        PS2GL_Vertex vc = *c;
        PS2GL_ApplyLighting (&va);
        PS2GL_ApplyLighting (&vb);
        PS2GL_ApplyLighting (&vc);

        float cross = (_b[0] - _a[0]) * (_c[1] - _a[1])
                      - (_b[1] - _a[1]) * (_c[0] - _a[0]);
        int front = cross < 0;
        GLenum mode
            = front ? gl.Draw.Polygon.FrontMode : gl.Draw.Polygon.BackMode;

        PS2GL_DrawTriangle (&va, &vb, &vc, &_a, &_b, &_c, mode);
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
