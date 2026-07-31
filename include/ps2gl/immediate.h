#ifndef ps2gl_immediate_h
#define ps2gl_immediate_h

#include <GL/gl.h>

typedef struct ps2_vertex_t
{
    GLfloat X, Y, Z;
    GLubyte Color[4];
} PS2_Vertex;

#define PS2_IMM_MAX_VERTICES 4096

typedef struct ps2_immediate_context_t
{
    PS2_Vertex Vertices[PS2_IMM_MAX_VERTICES];
    GLubyte CurrentColor[4];

    int VertexCount;
    GLenum CurrentMode;
} PS2_ImmContext;

#endif
