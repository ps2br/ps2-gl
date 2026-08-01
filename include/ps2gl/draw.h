#ifndef ps2gl_draw_h
#define ps2gl_draw_h

#include <GL/gl.h>

#include "ps2gl/types.h"

typedef struct ps2_vertex_t
{
    PS2_3DCoords Coords;
    PS2_3DCoords Normal;
    PS2_2DCoords TexCoords;
    PS2_Color Color;
} PS2_Vertex;

#define PS2_DRAW_MAX_VERTICES 4096

typedef struct ps2_draw_context_t
{
    PS2_Vertex Vertices[PS2_DRAW_MAX_VERTICES];
    int VertexCount;

    PS2_Color CurrentColor;
    PS2_3DCoords CurrentNormal;

    GLenum CurrentMode;
} PS2_DrawContext;

#endif
