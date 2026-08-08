#include "ps2gl/render.h"

#include <dmaKit.h>
#include <gsKit.h>
#include <gsTexture.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <tamtypes.h>

#include "ps2gl/types.h"

struct ps2gl_renderer_t
{
    GSGLOBAL *Gs;
};

PS2GL_Renderer *
PS2GL_InitRenderer (void)
{
    PS2GL_Renderer *self = calloc (1, sizeof *self);

    self->Gs = gsKit_init_global ();
    gsKit_init_screen (self->Gs);
    dmaKit_init (D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                 D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init (DMA_CHANNEL_GIF);

    return self;
}

GLsizei
PS2GL_GetRendererWidth (PS2GL_Renderer *self)
{
    return self->Gs->Width;
}

GLsizei
PS2GL_GetRendererHeight (PS2GL_Renderer *self)
{
    return self->Gs->Height;
}

void
PS2GL_SetRendererScissor (PS2GL_Renderer *self, GLint x, GLint y,
                          GLsizei width, GLsizei height)
{
    gsKit_set_scissor (
        self->Gs, GS_SETREG_SCISSOR (x, y, x + width - 1, y + height - 1));
}

void
PS2GL_ResetRendererScissor (PS2GL_Renderer *self)
{
    gsKit_set_scissor (self->Gs, GS_SCISSOR_RESET);
}

void
PS2GL_ClearRenderColor (PS2GL_Renderer *self, PS2GL_Color color)
{
    gsKit_clear (self->Gs, GS_SETREG_RGBA (color[0] * 255, color[1] * 255,
                                           color[2] * 255, color[3] * 255));
}

void
PS2GL_DeInitRenderer (PS2GL_Renderer *self)
{
    gsKit_deinit_global (self->Gs);
    free (self);
}

struct ps2gl_impl_texture_t
{
    GSTEXTURE Gs;
};

static inline u32
PS2GL_TextureFilterToGsKit (PS2GL_TextureFilter tf)
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
PS2GL_TexturePSMToGsKit (PS2GL_TexturePSM psm)
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

PS2GL_ImplTexture *
PS2GL_CreateImplTexture (int width, int height, PS2GL_TexturePSM psm,
                         PS2GL_TextureFilter filter)
{
    PS2GL_ImplTexture *self = calloc (1, sizeof *self);
    GSTEXTURE *gs = &self->Gs;
    gs->Width = width;
    gs->Height = height;
    gs->Filter = PS2GL_TextureFilterToGsKit (filter);
    gs->PSM = PS2GL_TexturePSMToGsKit (psm);
    return self;
}

u32
PS2GL_GetImplTextureSize (PS2GL_ImplTexture *self)
{
    return gsKit_texture_size (self->Gs.Width, self->Gs.Height, self->Gs.PSM);
}

void *
PS2GL_AllocateImplTextureMem (PS2GL_ImplTexture *self)
{
    return (self->Gs.Mem = memalign (128, PS2GL_GetImplTextureSize (self)));
}

extern int gsKit_texture_finish (GSGLOBAL *, GSTEXTURE *);

void
PS2GL_FinishImplTextureCreation (PS2GL_Renderer *renderer,
                                 PS2GL_ImplTexture *self)
{
    gsKit_texture_finish (renderer->Gs, &self->Gs);
}

void
PS2GL_SetImplTextureFilter (PS2GL_ImplTexture *self,
                            PS2GL_TextureFilter filter)
{
    self->Gs.Filter = PS2GL_TextureFilterToGsKit (filter);
}

#define COLOR8(x) ((u8)(fminf (fmaxf ((x), 0.0f), 1.0f) * 255.0f))
void
PS2GL_DrawFilledTriangle (PS2GL_Renderer *self, PS2GL_Vertex *a,
                          PS2GL_Vertex *b, PS2GL_Vertex *c)
{
    gsKit_prim_triangle_gouraud_3d (
        self->Gs,

        a->Coords[0], a->Coords[1], a->Coords[2], b->Coords[0], b->Coords[1],
        b->Coords[2], c->Coords[0], c->Coords[1], c->Coords[2],

        GS_SETREG_RGBAQ (COLOR8 (a->Color[0]), COLOR8 (a->Color[1]),
                         COLOR8 (a->Color[2]), COLOR8 (a->Color[3]), 0),

        GS_SETREG_RGBAQ (COLOR8 (b->Color[0]), COLOR8 (b->Color[1]),
                         COLOR8 (b->Color[2]), COLOR8 (b->Color[3]), 0),

        GS_SETREG_RGBAQ (COLOR8 (c->Color[0]), COLOR8 (c->Color[1]),
                         COLOR8 (c->Color[2]), COLOR8 (c->Color[3]), 0));
}

void
PS2GL_DrawFilledTexturedTriangle (PS2GL_Renderer *self, PS2GL_Vertex *a,
                                  PS2GL_Vertex *b, PS2GL_Vertex *c,
                                  PS2GL_ImplTexture *texture)
{
    float au = a->TexCoords[0] * texture->Gs.Width;
    float av = a->TexCoords[1] * texture->Gs.Height;

    float bu = b->TexCoords[0] * texture->Gs.Width;
    float bv = b->TexCoords[1] * texture->Gs.Height;

    float cu = c->TexCoords[0] * texture->Gs.Width;
    float cv = c->TexCoords[1] * texture->Gs.Height;

    gsKit_prim_triangle_goraud_texture_3d (
        self->Gs,

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