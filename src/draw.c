#include "ps2gl/draw.h"

#include <GL/gl.h>
#include <debug.h>
#include <math.h>
#include <math3d.h>

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

                xa, ya, (int)ZDEPTH (ax), ua, va_uv,

                xb, yb, (int)ZDEPTH (bx), ub, vb_uv,

                xc, yc, (int)ZDEPTH (cx), uc, uc_uv,

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

                xa, ya, (int)ZDEPTH (ax), xb, yb, (int)ZDEPTH (bx), xc, yc,
                (int)ZDEPTH (cx),

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
