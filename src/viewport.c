#include <GL/gl.h>

#include "ps2gl/context.h"

void
glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
    gl.ViewportX = x;
    gl.ViewportY = y;
    gl.ViewportWidth = width;
    gl.ViewportHeight = height;
}
