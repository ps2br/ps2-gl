#include "ps2gl/texture.h"

#include <GL/gl.h>
#include <gsCore.h>
#include <gsInit.h>
#include <gsTexture.h>

#include "ps2gl/context.h"
#include "tamtypes.h"

void
glGenTextures (GLsizei n, GLuint *textures)
{
    for (int i = 0; i < n; i++)
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
glBindTexture (GLenum target, GLuint texture)
{
    if (texture >= PS2_TEXTURES_MAX)
        return;

    gl.Tex.BoundTexture = texture;
}

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
    gtex->PSM = GS_PSM_CT32;
    gtex->Filter = (tex->MinFilter == GL_NEAREST) ? GS_FILTER_NEAREST
                                                  : GS_FILTER_LINEAR;

    // <trindadedev>: I don't know how to make this actually work.
    // in the old ps2gl it has a GS Mem C++ Implementation
    // maybe we need something like that 
    {
        gtex->TBW = (gtex->Width + 63) / 64;
        gtex->Mem = (u32 *)pixels;
        int tex_size = gsKit_texture_size (gtex->Width, gtex->Height, gtex->PSM);
        u32 vram_addr = gsKit_vram_alloc (gl.Gs, tex_size, GSKIT_ALLOC_USERBUFFER);
        if (vram_addr == GSKIT_ALLOC_ERROR)
            return;
    
        gtex->Vram = vram_addr;
    }
    gsKit_texture_upload (gl.Gs, &tex->GTexture);
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
