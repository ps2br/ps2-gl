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
#include <math.h>
#include <math3d.h>
#include <stddef.h>

#include "ps2gl/context.h"

static void
PS2_ApplyLighting (PS2_Vertex *v)
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
glBegin (GLenum mode)
{
    gl.Draw.VertexCount = 0;
    gl.Draw.CurrentMode = mode;
}

void
glEnd (void)
{
    if (gl.Draw.CurrentMode != GL_TRIANGLES)
        return;

    for (int i = 0; i < gl.Draw.VertexCount; i += 3)
    {
        PS2_Vertex *a = &gl.Draw.Vertices[i + 0];
        PS2_Vertex *b = &gl.Draw.Vertices[i + 1];
        PS2_Vertex *c = &gl.Draw.Vertices[i + 2];

        float ax, ay, az;
        float bx, by, bz;
        float cx, cy, cz;

        PS2_TransformVertex (a->Coords[0], a->Coords[1], a->Coords[2], &ax,
                             &ay, &az);
        PS2_TransformVertex (b->Coords[0], b->Coords[1], b->Coords[2], &bx,
                             &by, &bz);
        PS2_TransformVertex (c->Coords[0], c->Coords[1], c->Coords[2], &cx,
                             &cy, &cz);

        float xa = (ax + 1.0f) * 0.5f * gl.ViewportWidth;
        float ya = (1.0f - ay) * 0.5f * gl.ViewportHeight;

        float xb = (bx + 1.0f) * 0.5f * gl.ViewportWidth;
        float yb = (1.0f - by) * 0.5f * gl.ViewportHeight;

        float xc = (cx + 1.0f) * 0.5f * gl.ViewportWidth;
        float yc = (1.0f - cy) * 0.5f * gl.ViewportHeight;

        /** make a copy to not modify the og vertex*/
        PS2_Vertex va = *a;
        PS2_Vertex vb = *b;
        PS2_Vertex vc = *c;
        PS2_ApplyLighting (&va);
        PS2_ApplyLighting (&vb);
        PS2_ApplyLighting (&vc);

#define ZDEPTH(v) (((v) + 1.f) * .5f)
#define COLOR8(x) ((u8)(fminf (fmaxf ((x), 0.0f), 1.0f) * 255.0f))

        if (gl.Caps.Texture2D && gl.Tex.BoundTexture != 0)
        {
            GSTEXTURE *texture
                = &gl.Tex.Textures[gl.Tex.BoundTexture].GTexture;

            float ua = va.TexCoords[0] * texture->Width;
            float va_uv = va.TexCoords[1] * texture->Height;

            float ub = vb.TexCoords[0] * texture->Width;
            float vb_uv = vb.TexCoords[1] * texture->Height;

            float uc = vc.TexCoords[0] * texture->Width;
            float uc_uv = vc.TexCoords[1] * texture->Height;

            gsKit_prim_triangle_goraud_texture_3d (
                gl.Gs,

                texture,

                xa, ya, (int)ZDEPTH (az), ua, va_uv,

                xb, yb, (int)ZDEPTH (bz), ub, vb_uv,

                xc, yc, (int)ZDEPTH (cz), uc, uc_uv,

                GS_SETREG_RGBAQ (COLOR8 (va.Color[0]), COLOR8 (va.Color[1]),
                                 COLOR8 (va.Color[2]), COLOR8 (va.Color[3]),
                                 0),

                GS_SETREG_RGBAQ (COLOR8 (vb.Color[0]), COLOR8 (vb.Color[1]),
                                 COLOR8 (vb.Color[2]), COLOR8 (vb.Color[3]),
                                 0),

                GS_SETREG_RGBAQ (COLOR8 (vc.Color[0]), COLOR8 (vc.Color[1]),
                                 COLOR8 (vc.Color[2]), COLOR8 (vc.Color[3]),
                                 0));
        }
        else
        {
            gsKit_prim_triangle_gouraud_3d (
                gl.Gs,

                xa, ya, (int)ZDEPTH (az), xb, yb, (int)ZDEPTH (bz), xc, yc,
                (int)ZDEPTH (cz),

                GS_SETREG_RGBAQ (COLOR8 (va.Color[0]), COLOR8 (va.Color[1]),
                                 COLOR8 (va.Color[2]), COLOR8 (va.Color[3]),
                                 0),

                GS_SETREG_RGBAQ (COLOR8 (vb.Color[0]), COLOR8 (vb.Color[1]),
                                 COLOR8 (vb.Color[2]), COLOR8 (vb.Color[3]),
                                 0),

                GS_SETREG_RGBAQ (COLOR8 (vc.Color[0]), COLOR8 (vc.Color[1]),
                                 COLOR8 (vc.Color[2]), COLOR8 (vc.Color[3]),
                                 0));
        }
    }
}

static void
PS2_LoadColorArr (int index)
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
PS2_LoadTexCoordsArr (int index)
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
PS2_LoadNormalArr (int index)
{
    const char *base = gl.Draw.CurrentNormalArray.Pointer;

    size_t stride = gl.Draw.CurrentNormalArray.Stride;
    if (stride == 0)
        stride = 3 * sizeof (GLfloat);

    const GLfloat *n = (const GLfloat *)(base + index * stride);
    glNormal3f (n[0], n[1], n[2]);
}

static void
PS2_LoadVertexArr (int index)
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
            PS2_LoadColorArr (idx);

        if (gl.Tex.CurrentTexCoordsArray.Enabled)
            PS2_LoadTexCoordsArr (idx);

        if (gl.Draw.CurrentNormalArray.Enabled)
            PS2_LoadNormalArr (idx);

        PS2_LoadVertexArr (idx);
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
            PS2_LoadColorArr (idx);

        if (gl.Tex.CurrentTexCoordsArray.Enabled)
            PS2_LoadTexCoordsArr (idx);

        if (gl.Draw.CurrentNormalArray.Enabled)
            PS2_LoadNormalArr (idx);

        PS2_LoadVertexArr (idx);
    }
    glEnd ();
}
