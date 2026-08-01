#include <GL/gl.h>

#include "ps2gl/context.h"

#include <debug.h>

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
    case GL_BLEND:
    case GL_COLOR_LOGIC_OP:
    case GL_CULL_FACE:
    case GL_DEPTH_TEST:
    case GL_DITHER:
    case GL_LINE_SMOOTH:
    case GL_POLYGON_OFFSET_FILL:
    case GL_POLYGON_OFFSET_LINE:
    case GL_POLYGON_OFFSET_POINT:
    case GL_POLYGON_SMOOTH:
    case GL_SCISSOR_TEST:
    case GL_STENCIL_TEST:
        scr_printf ("PS2GL: Capability %d not implemented.\n", cap);
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
    case GL_BLEND:
    case GL_COLOR_LOGIC_OP:
    case GL_CULL_FACE:
    case GL_DEPTH_TEST:
    case GL_DITHER:
    case GL_LINE_SMOOTH:
    case GL_POLYGON_OFFSET_FILL:
    case GL_POLYGON_OFFSET_LINE:
    case GL_POLYGON_OFFSET_POINT:
    case GL_POLYGON_SMOOTH:
    case GL_SCISSOR_TEST:
    case GL_STENCIL_TEST:
        scr_printf ("PS2GL: Capability %d not implemented.\n", cap);
        break;
    case GL_TEXTURE_2D:
        gl.Caps.Texture2D = 0;
        break;
    }
}

void
glEnableClientState (GLenum cap)
{
    switch (cap)
    {
    case GL_COLOR_ARRAY:
    case GL_EDGE_FLAG_ARRAY:
    case GL_INDEX_ARRAY:
    case GL_NORMAL_ARRAY:
    case GL_VERTEX_ARRAY:
        scr_printf ("PS2GL: ClientState %d not implemented.\n", cap);
        break;
    case GL_TEXTURE_COORD_ARRAY:
        gl.Tex.CurrentTexCoordsArray.Enabled = 1;
        break;
    }
}

void
glDisableClientState (GLenum cap)
{
    switch (cap)
    {
    case GL_COLOR_ARRAY:
    case GL_EDGE_FLAG_ARRAY:
    case GL_INDEX_ARRAY:
    case GL_NORMAL_ARRAY:
    case GL_VERTEX_ARRAY:
        scr_printf ("PS2GL: ClientState %d not implemented.\n", cap);
        break;
    case GL_TEXTURE_COORD_ARRAY:
        gl.Tex.CurrentTexCoordsArray.Enabled = 0;
        break;
    }
}
