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

#include "ps2gl/texture.h"

#include <stdlib.h>
#include <tamtypes.h>

#include "ps2gl/context.h"
#include "ps2gl/render.h"

void
glGenTextures (GLsizei n, GLuint *textures)
{
    for (GLsizei i = 0; i < n; i++)
    {
        for (int id = 1; id < PS2GL_TEXTURES_MAX; id++)
        {
            if (!gl.Tex.Textures[id].Used)
            {
                gl.Tex.Textures[id].Used = 1;
                textures[i] = id;
                break;
            }
        }
    }
}

void
glDeleteTextures (GLsizei n, const GLuint *textures)
{
    for (GLsizei i = 0; i < n; i++)
    {
        GLuint id = textures[i];
        if (id == 0)
            continue;

        if (id >= PS2GL_TEXTURES_MAX)
            continue;

        PS2GL_Texture *tex = &gl.Tex.Textures[id];
        tex->Used = 0;
        free (tex->ImplTexture);
        if (gl.Tex.BoundTexture == id)
            gl.Tex.BoundTexture = 0;
    }
}

void
glBindTexture (GLenum target, GLuint texture)
{
    if (texture >= PS2GL_TEXTURES_MAX)
    {
        gl.CurrentError = GL_STACK_OVERFLOW;
        return;
    }

    gl.Tex.BoundTexture = texture;
}

void
glTexImage2D (GLenum target, GLint level, GLint internalFormat, GLsizei width,
              GLsizei height, GLint border, GLenum format, GLenum type,
              const GLvoid *pixels)
{
    if (gl.Tex.BoundTexture == 0)
        return;

    PS2GL_Texture *tex = &gl.Tex.Textures[gl.Tex.BoundTexture];
    tex->Width = width;
    tex->Height = height;

    PS2GL_TexturePSM psm;
    if (format == GL_RGBA)
        psm = PS2GL_PSM_32;
    else if (format == GL_RGB)
        psm = PS2GL_PSM_24;
    tex->ImplTexture = PS2GL_CreateImplTexture (width, height, psm,
                                                (tex->MinFilter == GL_NEAREST)
                                                    ? PS2GL_FILTER_NEAREST
                                                    : PS2GL_FILTER_LINEAR);

    void *mem = PS2GL_AllocateImplTextureMem (tex->ImplTexture);
    u32 size = PS2GL_GetImplTextureSize (tex->ImplTexture);
    if (type == GL_UNSIGNED_BYTE)
        memcpy (mem, pixels, size);

    PS2GL_FinishImplTextureCreation (gl.Renderer, tex->ImplTexture);
}

void
glTexParameteri (GLenum target, GLenum pname, GLint param)
{
    if (gl.Tex.BoundTexture == 0)
        return;

    PS2GL_Texture *tex = &gl.Tex.Textures[gl.Tex.BoundTexture];
    switch (pname)
    {
    case GL_TEXTURE_MIN_FILTER:
        tex->MinFilter = param;
        if (param == GL_NEAREST)
            PS2GL_SetImplTextureFilter (tex->ImplTexture,
                                        PS2GL_FILTER_NEAREST);
        else
            PS2GL_SetImplTextureFilter (tex->ImplTexture, PS2GL_FILTER_LINEAR);
        break;
    case GL_TEXTURE_MAG_FILTER:
        tex->MagFilter = param;
        if (param == GL_NEAREST)
            PS2GL_SetImplTextureFilter (tex->ImplTexture,
                                        PS2GL_FILTER_NEAREST);
        else
            PS2GL_SetImplTextureFilter (tex->ImplTexture, PS2GL_FILTER_LINEAR);
        break;
    };
}

void
glTexCoord2f (GLfloat u, GLfloat v)
{
    gl.Tex.CurrentTexCoords[0] = u;
    gl.Tex.CurrentTexCoords[1] = v;
}

void
glTexCoordPointer (GLint size, GLenum type, GLsizei stride,
                   const GLvoid *pointer)
{
    gl.Tex.CurrentTexCoordsArray.Size = size;
    gl.Tex.CurrentTexCoordsArray.Type = type;
    gl.Tex.CurrentTexCoordsArray.Stride = stride;
    gl.Tex.CurrentTexCoordsArray.Pointer = pointer;
}
