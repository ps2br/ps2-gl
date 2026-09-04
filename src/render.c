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

#include "ps2gl/render.h"

#include <malloc.h>
#include <math.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <dmaKit.h>
#include <gsKit.h>

#include "ps2gl/types.h"

struct ps2gl_renderer_platform_t
{
    GSGLOBAL *Gs;
};

static GLsizei
GSKit_GetWidth (PS2GL_Renderer *self)
{
    return self->Platform->Gs->Width;
}

static GLsizei
GSKit_GetHeight (PS2GL_Renderer *self)
{
    return self->Platform->Gs->Height;
}

static void
GSKit_ClearColor (PS2GL_Renderer *self, PS2GL_Color color)
{
    gsKit_clear (self->Platform->Gs,
                 GS_SETREG_RGBA (color[0] * 255, color[1] * 255,
                                 color[2] * 255, color[3] * 255));
}

static void
GSKit_SwapBuffers (PS2GL_Renderer *self)
{
    gsKit_sync_flip (self->Platform->Gs);
    gsKit_queue_exec (self->Platform->Gs);
}

static void
GSKit_SetScissor (PS2GL_Renderer *self, GLint x, GLint y, GLsizei width,
                  GLsizei height)
{
    gsKit_set_scissor (
        self->Platform->Gs,
        GS_SETREG_SCISSOR (x, y, x + width - 1, y + height - 1));
}

static void
GSKit_ResetScissor (PS2GL_Renderer *self)
{
    gsKit_set_scissor (self->Platform->Gs, GS_SCISSOR_RESET);
}

static void
GSKit_SetDepthMask (PS2GL_Renderer *self, GLboolean flag)
{
    self->Platform->Gs->ZBuffering
        = (flag == GL_TRUE) ? GS_SETTING_ON : GS_SETTING_OFF;
}

static void
GSKit_DeInit (PS2GL_Renderer *self)
{
    gsKit_deinit_global (self->Platform->Gs);
    free (self->Platform);
    free (self);
}

struct ps2gl_impl_texture_t
{
    GSTEXTURE Gs;
};

static inline u32
GSKit_TextureFilterToGsKit (PS2GL_TextureFilter tf)
{
    switch (tf)
    {
    case PS2GL_FILTER_LINEAR:
        return GS_FILTER_LINEAR;
    case PS2GL_FILTER_NEAREST:
        return GS_FILTER_NEAREST;
    default:
        return GS_FILTER_NEAREST; // 0x00, default
    }
}

static inline u8
GSKit_TexturePSMToGsKit (PS2GL_TexturePSM psm)
{
    switch (psm)
    {
    case PS2GL_PSM_24:
        return GS_PSM_CT24;
    case PS2GL_PSM_32:
        return GS_PSM_CT32;
    default:
        return GS_PSM_CT32; // 0x00, // default
    }
}

static PS2GL_ImplTexture *
GSKit_CreateImplTexture (int width, int height, PS2GL_TexturePSM psm,
                         PS2GL_TextureFilter filter)
{
    PS2GL_ImplTexture *self = calloc (1, sizeof *self);
    GSTEXTURE *gs = &self->Gs;
    gs->Width = width;
    gs->Height = height;
    gs->Filter = GSKit_TextureFilterToGsKit (filter);
    gs->PSM = GSKit_TexturePSMToGsKit (psm);
    return self;
}

static u32
GSKit_GetImplTextureSize (PS2GL_ImplTexture *self)
{
    return gsKit_texture_size (self->Gs.Width, self->Gs.Height, self->Gs.PSM);
}

static void *
GSKit_AllocateImplTextureMem (PS2GL_ImplTexture *self)
{
    return (self->Gs.Mem = memalign (128, GSKit_GetImplTextureSize (self)));
}

extern int gsKit_texture_finish (GSGLOBAL *, GSTEXTURE *);

static void
GSKit_FinishImplTextureCreation (PS2GL_Renderer *self,
                                 PS2GL_ImplTexture *texture)
{
    gsKit_texture_finish (self->Platform->Gs, &texture->Gs);
}

static void
GSKit_SetImplTextureFilter (PS2GL_ImplTexture *self,
                            PS2GL_TextureFilter filter)
{
    self->Gs.Filter = GSKit_TextureFilterToGsKit (filter);
}

#define COLOR8(x) ((u8)(fminf (fmaxf ((x), 0.0f), 1.0f) * 255.0f))

static void
GSKit_DrawFilledTriangle (PS2GL_Renderer *self, GLenum shadeModel,
                          PS2GL_Vertex *a, PS2GL_Vertex *b, PS2GL_Vertex *c)
{
    if (shadeModel == GL_SMOOTH)
    {
        gsKit_prim_triangle_gouraud_3d (
            self->Platform->Gs,

            a->Coords[0], a->Coords[1], a->Coords[2], b->Coords[0],
            b->Coords[1], b->Coords[2], c->Coords[0], c->Coords[1],
            c->Coords[2],

            GS_SETREG_RGBAQ (COLOR8 (a->Color[0]), COLOR8 (a->Color[1]),
                             COLOR8 (a->Color[2]), COLOR8 (a->Color[3]), 0),

            GS_SETREG_RGBAQ (COLOR8 (b->Color[0]), COLOR8 (b->Color[1]),
                             COLOR8 (b->Color[2]), COLOR8 (b->Color[3]), 0),

            GS_SETREG_RGBAQ (COLOR8 (c->Color[0]), COLOR8 (c->Color[1]),
                             COLOR8 (c->Color[2]), COLOR8 (c->Color[3]), 0));
    }
    else if (shadeModel == GL_FLAT)
    {
        gsKit_prim_triangle_3d (
            self->Platform->Gs,

            a->Coords[0], a->Coords[1], a->Coords[2], b->Coords[0],
            b->Coords[1], b->Coords[2], c->Coords[0], c->Coords[1],
            c->Coords[2],

            GS_SETREG_RGBAQ (COLOR8 (a->Color[0]), COLOR8 (a->Color[1]),
                             COLOR8 (a->Color[2]), COLOR8 (a->Color[3]), 0));
    }
}

static void
GSKit_DrawFilledTexturedTriangle (PS2GL_Renderer *self, GLenum shadeModel,
                                  PS2GL_Vertex *a, PS2GL_Vertex *b,
                                  PS2GL_Vertex *c, PS2GL_ImplTexture *texture)
{
    float au = a->TexCoords[0] * texture->Gs.Width;
    float av = a->TexCoords[1] * texture->Gs.Height;

    float bu = b->TexCoords[0] * texture->Gs.Width;
    float bv = b->TexCoords[1] * texture->Gs.Height;

    float cu = c->TexCoords[0] * texture->Gs.Width;
    float cv = c->TexCoords[1] * texture->Gs.Height;

    if (shadeModel == GL_FLAT)
    {
        a->Color[0] = c->Color[0];
        a->Color[1] = c->Color[1];
        a->Color[2] = c->Color[2];
        a->Color[3] = c->Color[3];

        b->Color[0] = c->Color[0];
        b->Color[1] = c->Color[1];
        b->Color[2] = c->Color[2];
        b->Color[3] = c->Color[3];
    }

    gsKit_prim_triangle_goraud_texture_3d (
        self->Platform->Gs,

        &texture->Gs,

        a->Coords[0], a->Coords[1], a->Coords[2], au, av,

        b->Coords[0], b->Coords[1], b->Coords[2], bu, bv,

        c->Coords[0], c->Coords[1], c->Coords[2], cu, cv,

        GS_SETREG_RGBAQ (COLOR8 (a->Color[0]), COLOR8 (a->Color[1]),
                         COLOR8 (a->Color[2]), COLOR8 (a->Color[3]), 0),

        GS_SETREG_RGBAQ (COLOR8 (b->Color[0]), COLOR8 (b->Color[1]),
                         COLOR8 (b->Color[2]), COLOR8 (b->Color[3]), 0),

        GS_SETREG_RGBAQ (COLOR8 (c->Color[0]), COLOR8 (c->Color[1]),
                         COLOR8 (c->Color[2]), COLOR8 (c->Color[3]), 0));
}

PS2GL_Renderer *
PS2GL_InitGSKitRenderer (void)
{
    PS2GL_Renderer *self = calloc (1, sizeof *self);
    self->Platform = calloc (1, sizeof *self->Platform);

    self->Platform->Gs = gsKit_init_global ();
    gsKit_init_screen (self->Platform->Gs);
    dmaKit_init (D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                 D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init (DMA_CHANNEL_GIF);

    self->GetWidth = GSKit_GetWidth;
    self->GetHeight = GSKit_GetHeight;
    self->SwapBuffers = GSKit_SwapBuffers;
    self->ClearColor = GSKit_ClearColor;
    self->SetScissor = GSKit_SetScissor;
    self->ResetScissor = GSKit_ResetScissor;
    self->SetDepthMask = GSKit_SetDepthMask;
    self->DeInit = GSKit_DeInit;

    self->CreateImplTexture = GSKit_CreateImplTexture;
    self->AllocateImplTextureMem = GSKit_AllocateImplTextureMem;
    self->GetImplTextureSize = GSKit_GetImplTextureSize;
    self->FinishImplTextureCreation = GSKit_FinishImplTextureCreation;
    self->SetImplTextureFilter = GSKit_SetImplTextureFilter;

    self->DrawFilledTriangle = GSKit_DrawFilledTriangle;
    self->DrawFilledTexturedTriangle = GSKit_DrawFilledTexturedTriangle;

    return self;
}
