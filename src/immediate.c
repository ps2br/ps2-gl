#include "ps2gl/immediate.h"

#include <GL/gl.h>
#include <math3d.h>
#include <stdio.h>

#include "ps2gl/context.h"

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

        printf ("A: %f %f %f\n", ax, ay, az);
        printf ("B: %f %f %f\n", bx, by, bz);
        printf ("C: %f %f %f\n", cx, cy, cz);

#define zdepth(v) ((az + 1.f) * .5f)
        gsKit_prim_triangle_gouraud_3d (
            gl.Gs,

            xa, ya, (int)zdepth (az), xb, yb, (int)zdepth (bz), xc, yc,
            (int)zdepth (cz),

            GS_SETREG_RGBAQ (a->Color[0], a->Color[1], a->Color[2],
                             a->Color[3], 0),

            GS_SETREG_RGBAQ (b->Color[0], b->Color[1], b->Color[2],
                             b->Color[3], 0),

            GS_SETREG_RGBAQ (c->Color[0], c->Color[1], c->Color[2],
                             c->Color[3], 0));
    }
}

void
glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    gl.Imm.CurrentColor[0] = red;
    gl.Imm.CurrentColor[1] = green;
    gl.Imm.CurrentColor[2] = blue;
    gl.Imm.CurrentColor[3] = alpha;
}

void
glVertex3f (GLfloat x, GLfloat y, GLfloat z)
{
    if (gl.Imm.VertexCount >= PS2_IMM_MAX_VERTICES)
        return;

    PS2_Vertex *v = &gl.Imm.Vertices[gl.Imm.VertexCount++];
    v->X = x;
    v->Y = y;
    v->Z = z;
    v->Color[0] = gl.Imm.CurrentColor[0];
    v->Color[1] = gl.Imm.CurrentColor[1];
    v->Color[2] = gl.Imm.CurrentColor[2];
    v->Color[3] = gl.Imm.CurrentColor[3];
}
