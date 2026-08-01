#include "ps2gl/immediate.h"

#include <GL/gl.h>
#include <math3d.h>

#include "ps2gl/context.h"

static void
PS2_ApplyLighting (PS2_Vertex *v)
{
    float lx = 0.f;
    float ly = 0.f;
    float lz = 1.f;

    float dot = v->NX * lx + v->NY * ly + v->NZ * lz;

    if (dot < 0.f)
        dot = 0.f;

    v->Color[0] *= dot;
    v->Color[1] *= dot;
    v->Color[2] *= dot;
}

void
glBegin (GLenum mode)
{
    gl.Imm.VertexCount = 0;
    gl.Imm.CurrentMode = mode;
}

void
glEnd (void)
{
    if (gl.Imm.CurrentMode != GL_TRIANGLES)
        return;

    for (int i = 0; i < gl.Imm.VertexCount; i += 3)
    {
        PS2_Vertex *a = &gl.Imm.Vertices[i + 0];
        PS2_Vertex *b = &gl.Imm.Vertices[i + 1];
        PS2_Vertex *c = &gl.Imm.Vertices[i + 2];

        float ax, ay, az;
        float bx, by, bz;
        float cx, cy, cz;

        PS2_TransformVertex (a->X, a->Y, a->Z, &ax, &ay, &az);
        PS2_TransformVertex (b->X, b->Y, b->Z, &bx, &by, &bz);
        PS2_TransformVertex (c->X, c->Y, c->Z, &cx, &cy, &cz);

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

#define zdepth(v) ((az + 1.f) * .5f)
        gsKit_prim_triangle_gouraud_3d (
            gl.Gs,

            xa, ya, (int)zdepth (az), xb, yb, (int)zdepth (bz), xc, yc,
            (int)zdepth (cz),

            GS_SETREG_RGBAQ (va.Color[0], va.Color[1], va.Color[2],
                             va.Color[3], 0),

            GS_SETREG_RGBAQ (vb.Color[0], vb.Color[1], vb.Color[2],
                             vb.Color[3], 0),

            GS_SETREG_RGBAQ (vc.Color[0], vc.Color[1], vc.Color[2],
                             vc.Color[3], 0));
    }
}
