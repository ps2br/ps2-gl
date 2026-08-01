#include <GL/gl.h>

#include "ps2gl/context.h"

static void
PS2_SetVertexState(PS2_Vertex *v)
{
    v->Color[0] = gl.CurrentColor[0];
    v->Color[1] = gl.CurrentColor[1];
    v->Color[2] = gl.CurrentColor[2];
    v->Color[3] = gl.CurrentColor[3];

    v->Normal[0] = gl.CurrentNormal[0];
    v->Normal[1] = gl.CurrentNormal[1];
    v->Normal[2] = gl.CurrentNormal[2];
}

void
glVertex2i (int x, int y)
{
    if (gl.Draw.VertexCount >= PS2_DRAW_MAX_VERTICES)
        return;

    PS2_Vertex *v = &gl.Draw.Vertices[gl.Draw.VertexCount++];
    v->Coords[0] = x;
    v->Coords[1] = y;
    v->Coords[2] = 0;

    PS2_SetVertexState(v);
}

void
glVertex2f (GLfloat x, GLfloat y)
{
    glVertex3f (x, y, 0.f);
}

void
glVertex3f (GLfloat x, GLfloat y, GLfloat z)
{
    if (gl.Draw.VertexCount >= PS2_DRAW_MAX_VERTICES)
        return;

    PS2_Vertex *v = &gl.Draw.Vertices[gl.Draw.VertexCount++];
    v->Coords[0] = x;
    v->Coords[1] = y;
    v->Coords[2] = z;
    
    PS2_SetVertexState(v);
}
