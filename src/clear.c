#include <GL/gl.h>
#include <gsKit.h>
#include <tamtypes.h>

#include "ps2gl/context.h"

void
glClearColor (GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
    if (r < 0.f)
        r = 0.f;
    if (r > 1.f)
        r = 1.f;

    if (g < 0.f)
        g = 0.f;
    if (g > 1.f)
        g = 1.f;

    if (b < 0.f)
        b = 0.f;
    if (b > 1.f)
        b = 1.f;

    if (a < 0.f)
        a = 0.f;
    if (a > 1.f)
        a = 1.f;

    gl.Draw.CurrentColor[0] = r;
    gl.Draw.CurrentColor[1] = g;
    gl.Draw.CurrentColor[2] = b;
    gl.Draw.CurrentColor[3] = a;
}

void
glClear (GLbitfield mask)
{
    if (mask & GL_COLOR_BUFFER_BIT)
    {
        u64 color
            = GS_SETREG_RGBAQ (gl.Draw.CurrentColor[0], gl.Draw.CurrentColor[1],
                               gl.Draw.CurrentColor[2], gl.Draw.CurrentColor[3], 0x00);
        gsKit_clear (gl.Gs, color);
    }
}
