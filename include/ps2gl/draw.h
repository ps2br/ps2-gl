#ifndef ps2gl_draw_h
#define ps2gl_draw_h

#include <GL/gl.h>

typedef GLfloat PS2_Coords[3];
typedef GLfloat PS2_Color[4];

typedef struct ps2_vertex_t
{
    PS2_Coords Coords;
    PS2_Coords Normal;
    PS2_Color Color;
} PS2_Vertex;

#define PS2_DRAW_MAX_VERTICES 4096

typedef struct ps2_draw_context_t
{
    PS2_Vertex Vertices[PS2_DRAW_MAX_VERTICES];
    int VertexCount;
    GLenum CurrentMode;
} PS2_DrawContext;

#endif
