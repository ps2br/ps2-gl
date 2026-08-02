/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2026 ps2br.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

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
    gl.CurrentError = 0;

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

    /** TODO: implement all the OpenGL Capabilities */
    gl.Caps.Texture2D = 0;

    /** Initiliaze @dmaKit and @dmaKit_chan */
    dmaKit_init (D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                 D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init (DMA_CHANNEL_GIF);

    gl.Gs->PSM = GS_PSM_CT32;
    gl.Gs->PSMZ = GS_PSMZ_16S;

    /** Initialize GL hints */
    gl.Hints.PerspectiveCorrection = GL_DONT_CARE;
    gl.Hints.PointSmooth = GL_DONT_CARE;
    gl.Hints.LineSmooth = GL_DONT_CARE;
    gl.Hints.PolygonSmooth = GL_DONT_CARE;
    gl.Hints.Fog = GL_DONT_CARE;
    gl.Hints.TextureCompression = GL_DONT_CARE;
    gl.Hints.GenerateMipmap = GL_DONT_CARE;
    gl.Hints.FragmentShaderDerivative = GL_DONT_CARE;
}

void
PS2_GLShutdown (void)
{
    /** actually there's nothing to clean yet */
    gl.Gs = 0;
}
