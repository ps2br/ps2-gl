#include "ps2gl/context.h"

#include <string.h>

#include <GL/gl.h>
#include <gsInit.h>
#include <tamtypes.h>

#include "ps2gl/matrix.h"

PS2_GLContext gl;

void
PS2_GLInit (GSGLOBAL *gs)
{
    memset (&gl, 0, sizeof gl);
    gl.Gs = gs;
    gl.ViewportWidth = gs->Width;
    gl.ViewportHeight = gs->Height;
    gl.Matrix.Mode = GL_MODELVIEW;

    PS2_MatrixIdentity (gl.Matrix.ModelView);
    PS2_MatrixIdentity (gl.Matrix.Projection);

    gl.ClearColor[0] = 0.f;
    gl.ClearColor[1] = 0.f;
    gl.ClearColor[2] = 0.f;
    gl.ClearColor[3] = 1.f;

    memset (&gl.Imm, 0, sizeof gl.Imm);
    gl.Imm.CurrentColor[0] = 1.f;
    gl.Imm.CurrentColor[1] = 1.f;
    gl.Imm.CurrentColor[2] = 1.f;
    gl.Imm.CurrentColor[3] = 1.f;
}

void
PS2_GLShutdown (void)
{
}
