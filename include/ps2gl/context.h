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

#ifndef ps2gl_context_h
#define ps2gl_context_h

#include <GL/gl.h>

#include "ps2gl/draw.h"
#include "ps2gl/matrix.h"
#include "ps2gl/render.h"
#include "ps2gl/texture.h"

typedef struct ps2_gl_hints_t
{
    GLenum LineSmooth;
    GLenum PolygonSmooth;
    GLenum PointSmooth;
    GLenum Fog;
    GLenum PerpectiveCorrection;
} PS2GL_Hints;

typedef struct ps2gl_capabilities_t
{
    GLboolean Texture2D : 1;
    GLboolean Scissor : 1;
    GLboolean DepthBufferEnable : 1;

    /** TODO: implement all the OpenGL Capabilities */
} __attribute__ ((packed)) PS2GL_Capabilities;

typedef struct ps2gl_context_t
{
    PS2GL_Renderer *Renderer;

    GLint ViewportX;
    GLint ViewportY;
    GLsizei ViewportWidth;
    GLsizei ViewportHeight;
    GLenum CurrentError;

    PS2GL_Capabilities Caps;
    PS2GL_Hints Hints;
    PS2GL_DrawContext Draw;
    PS2GL_MatrixContext Matrix;
    PS2GL_TextureContext Tex;
} PS2GL_Context;

extern PS2GL_Context gl;

/**
 * Initilazes the GL State.
 * Should be initialized with an Renderer.
 * Every call to @PS2GL_Init should have it PS2GL_Shutdown at end of usage.
 */
extern void PS2GL_Init (PS2GL_Renderer *);
extern void PS2GL_Shutdown (void);

#endif
