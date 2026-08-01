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

#include "ps2gl/matrix.h"

#include <GL/gl.h>
#include <math.h>
#include <string.h>
#include <linmath.h>

#include "ps2gl/context.h"

void
PS2_MatrixIdentity (mat4x4 m)
{
    mat4x4_identity(m);
}

void
PS2_TransformVertex(float x, float y, float z,
                    float *outX, float *outY, float *outZ)
{
    vec4 v = { x, y, z, 1.0f };
    vec4 mv;
    vec4 clip;

    mat4x4_mul_vec4(mv, gl.Matrix.ModelView, v);
    mat4x4_mul_vec4(clip, gl.Matrix.Projection, mv);

    if (clip[3] != 0.0f)
    {
        clip[0] /= clip[3];
        clip[1] /= clip[3];
        clip[2] /= clip[3];
    }

    *outX = clip[0];
    *outY = clip[1];
    *outZ = clip[2];
}

mat4x4 *PS2_CurrentMatrix (void)
{
    if (gl.Matrix.Mode == GL_PROJECTION)
        return &gl.Matrix.Projection;

    return &gl.Matrix.ModelView;
}

void
PS2_MatrixMultiply (mat4x4 out, const mat4x4 a, const mat4x4 b)
{
    mat4x4_mul(out, a, b);
}

void
glMatrixMode (GLenum mode)
{
    switch (mode)
    {
    case GL_MODELVIEW:
    case GL_PROJECTION:
        gl.Matrix.Mode = mode;
    }
}

void
glLoadIdentity (void)
{
    if (gl.Matrix.Mode == GL_MODELVIEW)
    {
        PS2_MatrixIdentity (gl.Matrix.ModelView);
    }
    else if (gl.Matrix.Mode == GL_PROJECTION)
    {
        PS2_MatrixIdentity (gl.Matrix.Projection);
    }
}

void
glPushMatrix (void)
{

    if (gl.Matrix.Mode == GL_MODELVIEW)
    {
        memcpy (gl.Matrix.ModelViewStack[gl.Matrix.ModelViewTop++],
                gl.Matrix.ModelView, sizeof (GLfloat) * 16);
    }
    else if (gl.Matrix.Mode == GL_PROJECTION)
    {
        memcpy (gl.Matrix.ProjectionStack[gl.Matrix.ProjectionTop++],
                gl.Matrix.Projection, sizeof (GLfloat) * 16);
    }
}

void
glPopMatrix (void)
{
    if (gl.Matrix.Mode == GL_MODELVIEW)
    {
        memcpy (gl.Matrix.ModelView,
                gl.Matrix.ModelViewStack[--gl.Matrix.ModelViewTop],
                sizeof (GLfloat) * 16);
    }
    else if (gl.Matrix.Mode == GL_PROJECTION)
    {
        memcpy (gl.Matrix.Projection,
                gl.Matrix.ProjectionStack[--gl.Matrix.ProjectionTop],
                sizeof (GLfloat) * 16);
    }
}

void
glMultMatrixf (const GLfloat *m)
{
    mat4x4 temp;
    memcpy(temp, m, sizeof(temp));

    mat4x4 *current = PS2_CurrentMatrix();

    mat4x4 result;
    mat4x4_mul(result, *current, temp);

    memcpy(*current, result, sizeof(result));
}

void
glTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1 };

    glMultMatrixf (m);
}

void
glScalef (GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat m[16] = { x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1 };

    glMultMatrixf (m);
}

void
glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat rad = angle * 0.0174532925f;
    GLfloat c = cosf (rad);
    GLfloat s = sinf (rad);

    GLfloat m[16] = { x * x * (1 - c) + c,
                      y * x * (1 - c) + z * s,
                      x * z * (1 - c) - y * s,
                      0,

                      x * y * (1 - c) - z * s,
                      y * y * (1 - c) + c,
                      y * z * (1 - c) + x * s,
                      0,

                      x * z * (1 - c) + y * s,
                      y * z * (1 - c) - x * s,
                      z * z * (1 - c) + c,
                      0,

                      0,
                      0,
                      0,
                      1 };

    glMultMatrixf (m);
}

void
glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
         GLdouble znear, GLdouble zfar)
{
    GLfloat m[16] = { 0 };

    m[0] = 2.0f / (right - left);
    m[5] = 2.0f / (top - bottom);
    m[10] = -2.0f / (zfar - znear);

    m[12] = -(right + left) / (right - left);
    m[13] = -(top + bottom) / (top - bottom);
    m[14] = -(zfar + znear) / (zfar - znear);

    m[15] = 1.0f;

    glMultMatrixf (m);
}

void
glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
           GLdouble znear, GLdouble zfar)
{
    GLfloat m[16] = { 0 };

    m[0] = (2.0f * znear) / (right - left);

    m[5] = (2.0f * znear) / (top - bottom);

    m[8] = (right + left) / (right - left);

    m[9] = (top + bottom) / (top - bottom);

    m[10] = -(zfar + znear) / (zfar - znear);

    m[11] = -1.0f;

    m[14] = -(2.0f * zfar * znear) / (zfar - znear);

    glMultMatrixf (m);
}
