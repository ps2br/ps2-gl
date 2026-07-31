#include "ps2gl/context.h"

#include <string.h>

#include <GL/gl.h>
#include <gsInit.h>
#include <tamtypes.h>

#include "ps2gl/matrix.h"

PS2_GLContext gl;

void
ps2glInit (GSGLOBAL *gs)
{
    memset (&gl, 0, sizeof gl);
    gl.Gs = gs;
    gl.ViewportWidth = gs->Width;
    gl.ViewportHeight = gs->Height;
    gl.MatrixMode = GL_MODELVIEW;

    PS2_MatrixIdentity (gl.ModelView);
    PS2_MatrixIdentity (gl.Projection);

    gl.ClearColor[0] = 0.f;
    gl.ClearColor[0] = 0.f;
    gl.ClearColor[0] = 0.f;
    gl.ClearColor[0] = 1.f;

    memset (&gl.Imm, 0, sizeof gl.Imm);
    gl.Imm.CurrentColor[0] = 1.f;
    gl.Imm.CurrentColor[1] = 1.f;
    gl.Imm.CurrentColor[2] = 1.f;
    gl.Imm.CurrentColor[3] = 1.f;
}

void
ps2glShutdown (void)
{
}
