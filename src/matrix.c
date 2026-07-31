#include "ps2gl/matrix.h"

#include <GL/gl.h>
#include <string.h>

#include "ps2gl/context.h"

void
PS2_MatrixIdentity (GLfloat m[16])
{
    memset (m, 0, sizeof (GLfloat) * 16);
    m[0] = 1.f;
    m[5] = 1.f;
    m[10] = 1.f;
    m[15] = 1.f;
}

void
glMatrixMode (GLenum mode)
{
    switch (mode)
    {
    case GL_MODELVIEW:
    case GL_PROJECTION:
        gl.MatrixMode = mode;
    }
}

void
glLoadIdentity (void)
{
    if (gl.MatrixMode == GL_MODELVIEW)
    {
        PS2_MatrixIdentity (gl.ModelView);
    }
    else if (gl.MatrixMode == GL_PROJECTION)
    {
        PS2_MatrixIdentity (gl.Projection);
    }
}
