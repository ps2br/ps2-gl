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

/** PS2GL Renderer Interface */

#ifndef ps2gl_render_h
#define ps2gl_render_h

#include <GL/gl.h>
#include <tamtypes.h>

#include "ps2gl/types.h"

/** Renderer, Implementation specific */
typedef struct ps2gl_renderer_t PS2GL_Renderer;

/** Renderer platform, Implementation specific */
typedef struct ps2gl_renderer_platform_t PS2GL_RendererPlatform;

/** Texture, Implementation-specific */
/** ex: in gsKit it may be GSTEXTURE */
typedef struct ps2gl_impl_texture_t PS2GL_ImplTexture;

typedef enum ps2gl_texture_psm_t
{
    PS2GL_PSM_24,
    PS2GL_PSM_32
} PS2GL_TexturePSM;

typedef enum ps2gl_texture_filter_t
{
    PS2GL_FILTER_NEAREST,
    PS2GL_FILTER_LINEAR
} PS2GL_TextureFilter;

struct ps2gl_renderer_t
{
    PS2GL_RendererPlatform *platform;

    void (*ClearColor) (PS2GL_Renderer *, PS2GL_Color);
    GLsizei (*GetWidth) (PS2GL_Renderer *);
    GLsizei (*GetHeight) (PS2GL_Renderer *);
    void (*SetScissor) (PS2GL_Renderer *, GLint x, GLint y, GLsizei width,
                        GLsizei height);
    void (*SwapBuffers) (PS2GL_Renderer *);

    void (*ResetScissor) (PS2GL_Renderer *);
    void (*DeInit) (PS2GL_Renderer *);

    /** Allocates the ImplTexture and fills Implementation-Specic data */
    /** NOTE: this don't fills the image data (pixels) */
    PS2GL_ImplTexture *(*CreateImplTexture) (int width, int height,
                                             PS2GL_TexturePSM psm,
                                             PS2GL_TextureFilter filter);

    /** Returns a pointer of allocate memory of Texture image
     * Should be filled by user
     * if PSM is 32, width is 200 and Heighr is 300
     * the allocated size is (200 * height * 4) => 240000 (bytes)
     */
    void *(*AllocateImplTextureMem) (PS2GL_ImplTexture *);

    u32 (*GetImplTextureSize) (PS2GL_ImplTexture *);

    /** Send the Texture into PS2 VRAM and Frees from RAM */
    void (*FinishImplTextureCreation) (PS2GL_Renderer *, PS2GL_ImplTexture *);

    void (*SetImplTextureFilter) (PS2GL_ImplTexture *, PS2GL_TextureFilter);

    void (*DrawFilledTriangle) (PS2GL_Renderer *, PS2GL_Vertex *a,
                                PS2GL_Vertex *b, PS2GL_Vertex *c);

    void (*DrawFilledTexturedTriangle) (PS2GL_Renderer *, PS2GL_Vertex *a,
                                        PS2GL_Vertex *b, PS2GL_Vertex *c,
                                        PS2GL_ImplTexture *texture);
};

/** Inits a gsKit implementation of PS2GL Renderer */
PS2GL_Renderer *PS2GL_InitGSKitRenderer (void);

#endif
