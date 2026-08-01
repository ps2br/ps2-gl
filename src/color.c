#include <GL/gl.h>

#include "ps2gl/context.h"

void
glColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
    gl.CurrentColor[0] = red * 255;
    gl.CurrentColor[1] = green * 255;
    gl.CurrentColor[2] = blue * 255;
}

void
glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    gl.CurrentColor[0] = red * 255;
    gl.CurrentColor[1] = green * 255;
    gl.CurrentColor[2] = blue * 255;
    gl.CurrentColor[3] = alpha * 255;
}

void
glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    gl.CurrentColor[0] = red;
    gl.CurrentColor[1] = green;
    gl.CurrentColor[2] = blue;
    gl.CurrentColor[3] = alpha;
}
