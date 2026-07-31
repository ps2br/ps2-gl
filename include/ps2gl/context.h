#ifndef ps2gl_context_h
#define ps2gl_context_h

#include <GL/gl.h>
#include <gsKit.h>

#include "ps2gl/immediate.h"

typedef struct ps2_gl_context_t
{
    GSGLOBAL *Gs;

    GLenum MatrixMode;

    GLfloat Projection[16];
    GLfloat ModelView[16];
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
} PS2_GLContext;

extern PS2_GLContext gl;

void ps2glInit (GSGLOBAL *);
void ps2glShutdown (void);

#endif
