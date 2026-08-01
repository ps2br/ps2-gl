#include "ps2gl/context.h"

#include <string.h>

#include <GL/gl.h>
#include <gsInit.h>
#include <tamtypes.h>

#include "dmaInit.h"
#include "ps2gl/matrix.h"

PS2_GLContext gl;

void
PS2_GLInit (GSGLOBAL *gs)
{
    memset (&gl, 0, sizeof gl);
    memset (&gl.Draw, 0, sizeof gl.Draw);
    memset (&gl.Tex, 0, sizeof gl.Tex);

    gl.Gs = gs;
    gl.ViewportWidth = gs->Width;
    gl.ViewportHeight = gs->Height;
    gl.Matrix.Mode = GL_MODELVIEW;

    PS2_MatrixIdentity (gl.Matrix.ModelView);
    PS2_MatrixIdentity (gl.Matrix.Projection);

    gl.Draw.CurrentColor[0] = 0.f;
    gl.Draw.CurrentColor[1] = 0.f;
    gl.Draw.CurrentColor[2] = 0.f;
    gl.Draw.CurrentColor[3] = 1.f;

    gl.Draw.CurrentNormal[0] = 0.f;
    gl.Draw.CurrentNormal[1] = 0.f;
    gl.Draw.CurrentNormal[2] = 1.f;

    gl.Tex.CurrentTexCoords[0] = 0.f;
    gl.Tex.CurrentTexCoords[1] = 0.f;

    gl.Caps.Blend = 0;
    gl.Caps.CullFace = 0;
    gl.Caps.DepthTest = 0;
    gl.Caps.Texture2D = 0;

    dmaKit_init (D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                 D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init (DMA_CHANNEL_GIF);
    gl.Gs->PSM = GS_PSM_CT32;
    gl.Gs->PSMZ = GS_PSMZ_16S;
}

void
PS2_GLShutdown (void)
{
}
