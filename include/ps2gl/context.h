#ifndef ps2gl_context_h
#define ps2gl_context_h

#include <GL/gl.h>
#include <gsKit.h>

#include "ps2gl/draw.h"
#include "ps2gl/matrix.h"
#include "ps2gl/texture.h"

typedef struct ps2_gl_capabilities_t
{
    GLboolean DepthTest;
    GLboolean Blend;
    GLboolean CullFace;
    GLboolean Texture2D;
} PS2_GLCapabilities;

typedef struct ps2_gl_context_t
{
    GSGLOBAL *Gs;

    GLint ViewportX;
    GLint ViewportY;
    GLsizei ViewportWidth;
    GLsizei ViewportHeight;

    PS2_GLCapabilities Caps;
    PS2_DrawContext Draw;
    PS2_MatrixContext Matrix;
    PS2_TextureContext Tex;
} PS2_GLContext;

extern PS2_GLContext gl;

void PS2_GLInit (GSGLOBAL *);
void PS2_GLShutdown (void);

#endif
