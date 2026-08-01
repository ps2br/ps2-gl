#include <GL/gl.h>

#include "ps2gl/context.h"

void
glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz)
{
    gl.Draw.CurrentNormal[0] = nx;
    gl.Draw.CurrentNormal[1] = ny;
    gl.Draw.CurrentNormal[2] = nz;
}

/** TODO: implement all the OpenGL Capabilities */
void
glEnable (GLenum cap)
{
    switch (cap)
    {
    case GL_DEPTH_TEST:
        gl.Caps.DepthTest = 1;
        break;
    case GL_BLEND:
        gl.Caps.Blend = 1;
        break;
    case GL_CULL_FACE:
        gl.Caps.CullFace = 1;
        break;
    case GL_TEXTURE_2D:
        gl.Caps.Texture2D = 1;
        break;
    }
}

/** TODO: implement all the OpenGL Capabilities */
void
glDisable (GLenum cap)
{
    switch (cap)
    {
    case GL_DEPTH_TEST:
        gl.Caps.DepthTest = 0;
        break;
    case GL_BLEND:
        gl.Caps.Blend = 0;
        break;
    case GL_CULL_FACE:
        gl.Caps.CullFace = 0;
        break;
    case GL_TEXTURE_2D:
        gl.Caps.Texture2D = 0;
        break;
    }
}
