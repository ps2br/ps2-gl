#ifndef ps2gl_context_h
#define ps2gl_context_h

#include <GL/gl.h>
#include <gsKit.h>

#include "ps2gl/immediate.h"
#include "ps2gl/matrix.h"

typedef struct ps2_gl_context_t
{
    GSGLOBAL *Gs;

    GLfloat ClearColor[4];

    GLint ViewportX;
    GLint ViewportY;
    GLsizei ViewportWidth;
    GLsizei ViewportHeight;

    GLboolean DepthTest;
    GLboolean Blend;
    GLboolean CullFace;
    GLboolean Texture2D;

    GLuint BoundTexture;

    PS2_ImmContext Imm;
    PS2_MatrixContext Matrix;
} PS2_GLContext;

extern PS2_GLContext gl;

void PS2_GLInit (GSGLOBAL *);
void PS2_GLShutdown (void);

#endif
