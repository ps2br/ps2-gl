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
#include <gsKit.h>

#include "ps2gl/draw.h"
#include "ps2gl/matrix.h"
#include "ps2gl/texture.h"

typedef struct ps2_gl_capabilities_t
{
    GLboolean Texture2D : 1;

    /** TODO: implement all the OpenGL Capabilities */
} __attribute__ ((packed)) PS2_GLCapabilities;

typedef struct ps2_gl_context_t
{
    GSGLOBAL *Gs;

    GLint ViewportX;
    GLint ViewportY;
    GLsizei ViewportWidth;
    GLsizei ViewportHeight;

    PS2_GLCapabilities Caps;
    PS2_DrawContext Draw;
    PS2_MatrixContext Matrix;
    PS2_TextureContext Tex;
    
    GLenum CurrentError;
} PS2_GLContext;

extern PS2_GLContext gl;

/**
 * Initilazes the GL State.
 * this function MUST be called after @gsKit_init_global.
 * this function initialzes @dmaKit and @dmaKit_chan.
 * Every call to @PS2_GLInit should have it PS2_GLShutdown at end of usage.
 */
extern void PS2_GLInit (GSGLOBAL *);
extern void PS2_GLShutdown (void);

#endif
