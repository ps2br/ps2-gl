#include <GL/gl.h>

#include "ps2gl/context.h"

void
glColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
    gl.Draw.CurrentColor[0] = red * 255;
    gl.Draw.CurrentColor[1] = green * 255;
    gl.Draw.CurrentColor[2] = blue * 255;
}

void
glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    gl.Draw.CurrentColor[0] = red * 255;
    gl.Draw.CurrentColor[1] = green * 255;
    gl.Draw.CurrentColor[2] = blue * 255;
    gl.Draw.CurrentColor[3] = alpha * 255;
}

void
glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    gl.Draw.CurrentColor[0] = red;
    gl.Draw.CurrentColor[1] = green;
    gl.Draw.CurrentColor[2] = blue;
    gl.Draw.CurrentColor[3] = alpha;
}
