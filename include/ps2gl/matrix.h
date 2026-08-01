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

#ifndef ps2gl_matrix_h
#define ps2gl_matrix_h

#include <GL/gl.h>
#include <linmath.h>

#define PS2_MATRIX_STACK_MAX 32

typedef struct ps2_matrix_context
{
    mat4x4 ModelView;
    mat4x4 Projection;

    mat4x4 ModelViewStack[PS2_MATRIX_STACK_MAX];
    mat4x4 ProjectionStack[PS2_MATRIX_STACK_MAX];

    int ModelViewTop;
    int ProjectionTop;

    GLenum Mode;
} PS2_MatrixContext;

mat4x4 *PS2_CurrentMatrix (void);
void PS2_MatrixIdentity (mat4x4);
void PS2_MatrixMultiply (mat4x4, const mat4x4, const mat4x4);
void PS2_TransformVertex (float, float, float, float *, float *, float *);

#endif
