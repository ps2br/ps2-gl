#ifndef ps2gl_types_h
#define ps2gl_types_h

#include <GL/gl.h>

typedef GLfloat PS2_3DCoords[3];
typedef GLfloat PS2_2DCoords[2];
typedef GLfloat PS2_Color[4];

typedef struct ps2_gl_array_t
{
    GLint Size;
    GLenum Type;
    GLsizei Stride;
    const void *Pointer;
    GLboolean Enabled : 1;
} __attribute__ ((packed)) PS2_GLArray;

#endif
