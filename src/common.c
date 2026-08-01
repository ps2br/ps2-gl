#include <GL/gl.h>

#include "ps2gl/context.h"

void
glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz)
{
    gl.CurrentNormal[0] = nx;
    gl.CurrentNormal[1] = ny;
    gl.CurrentNormal[2] = nz;
}
