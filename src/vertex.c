#include <GL/gl.h>

#include "ps2gl/context.h"

void
glVertex2i (int x, int y)
{
    if (gl.Imm.VertexCount >= PS2_IMM_MAX_VERTICES)
        return;

    PS2_Vertex *v = &gl.Imm.Vertices[gl.Imm.VertexCount++];
    v->X = x;
    v->Y = y;
    v->Z = 0;
    v->Color[0] = gl.CurrentColor[0];
    v->Color[1] = gl.CurrentColor[1];
    v->Color[2] = gl.CurrentColor[2];
    v->Color[3] = 0;
}

void
glVertex2f (GLfloat x, GLfloat y)
{
    if (gl.Imm.VertexCount >= PS2_IMM_MAX_VERTICES)
        return;

    PS2_Vertex *v = &gl.Imm.Vertices[gl.Imm.VertexCount++];
    v->X = x;
    v->Y = y;
    v->Z = 0;
    v->Color[0] = gl.CurrentColor[0];
    v->Color[1] = gl.CurrentColor[1];
    v->Color[2] = gl.CurrentColor[2];
    v->Color[3] = 0;
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
    v->Color[0] = gl.CurrentColor[0];
    v->Color[1] = gl.CurrentColor[1];
    v->Color[2] = gl.CurrentColor[2];
    v->Color[3] = gl.CurrentColor[3];
}
