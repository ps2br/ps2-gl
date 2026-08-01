#ifndef ps2gl_context_h
#define ps2gl_context_h

#include <GL/gl.h>
#include <gsKit.h>

#include "ps2gl/draw.h"
#include "ps2gl/matrix.h"
#include "ps2gl/texture.h"

typedef struct ps2_gl_capabilities_t
{
    GLboolean DepthTest : 1;
    GLboolean Blend : 1;
    GLboolean CullFace : 1;
    GLboolean Texture2D : 1;

    /** TODO: implement all the OpenGL Capabilities */
} __attribute__ ((packed)) PS2_GLCapabilities;

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

/**
 * Initilazes the GL State.
 * this function MUST be called after @gsKit_init_global.
 * this function initialzes @dmaKit and @dmaKit_chan.
 * Every call to @PS2_GLInit should have it PS2_GLShutdown at end of usage.
 */
extern void PS2_GLInit (GSGLOBAL *);
extern void PS2_GLShutdown (void);

#endif
