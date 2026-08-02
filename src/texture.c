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

#include <GL/gl.h>
#include <gsCore.h>
#include <gsInit.h>
#include <gsTexture.h>
#include <gsToolkit.h>
#include <malloc.h>
#include <tamtypes.h>

#include "ps2gl/context.h"

void
glGenTextures (GLsizei n, GLuint *textures)
{
    for (GLsizei i = 0; i < n; i++)
    {
        for (int id = 1; id < PS2_TEXTURES_MAX; id++)
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

        if (id >= PS2_TEXTURES_MAX)
            continue;

        PS2_Texture *tex = &gl.Tex.Textures[id];
        free (tex->GTexture.Mem); /** delete from RAM */
        tex->Used = 0;
        if (gl.Tex.BoundTexture == id)
            gl.Tex.BoundTexture = 0;
    }
}

void
glBindTexture (GLenum target, GLuint texture)
{
    if (texture >= PS2_TEXTURES_MAX){
        gl.CurrentError = GL_STACK_OVERFLOW;
        return;
     }
     
    gl.Tex.BoundTexture = texture;
}

extern int gsKit_texture_finish (GSGLOBAL *gsGlobal, GSTEXTURE *Texture);

void
glTexImage2D (GLenum target, GLint level, GLint internalFormat, GLsizei width,
              GLsizei height, GLint border, GLenum format, GLenum type,
              const GLvoid *pixels)
{
    if (gl.Tex.BoundTexture == 0)
        return;

    PS2_Texture *tex = &gl.Tex.Textures[gl.Tex.BoundTexture];
    tex->Width = width;
    tex->Height = height;

    /** gsKit texture needs to be filled before "uploading" it */
    GSTEXTURE *gtex = &tex->GTexture;
    gtex->Width = width;
    gtex->Height = height;

    if (format == GL_RGBA)
        gtex->PSM = GS_PSM_CT32;
    else if (format == GL_RGB)
        gtex->PSM = GS_PSM_CT24;

    gtex->Filter = (tex->MinFilter == GL_NEAREST) ? GS_FILTER_NEAREST
                                                  : GS_FILTER_LINEAR;

    u32 tex_size = gsKit_texture_size (gtex->Width, gtex->Height, gtex->PSM);
    gtex->Mem = memalign (128, tex_size);

    if (type == GL_UNSIGNED_BYTE)
        memcpy (gtex->Mem, pixels, tex_size);

    gsKit_texture_finish (gl.Gs, gtex);
}

void
glTexParameteri (GLenum target, GLenum pname, GLint param)
{
    if (gl.Tex.BoundTexture == 0)
        return;

    PS2_Texture *tex = &gl.Tex.Textures[gl.Tex.BoundTexture];
    switch (pname)
    {
    case GL_TEXTURE_MIN_FILTER:
        tex->MinFilter = param;
        if (param == GL_NEAREST)
            tex->GTexture.Filter = GS_FILTER_NEAREST;
        else
            tex->GTexture.Filter = GS_FILTER_LINEAR;
        break;
    case GL_TEXTURE_MAG_FILTER:
        tex->MagFilter = param;
        if (param == GL_NEAREST)
            tex->GTexture.Filter = GS_FILTER_NEAREST;
        else
            tex->GTexture.Filter = GS_FILTER_LINEAR;
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
