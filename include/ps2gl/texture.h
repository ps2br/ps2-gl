#ifndef ps2gl_texture_h
#define ps2gl_texture_h

#include <GL/gl.h>
#include <gsKit.h>

#include "ps2gl/types.h"

#define PS2_TEXTURES_MAX 256

typedef struct ps2_texture_t
{
    GSTEXTURE GTexture;
    int Width, Height;
    int Used;

    GLint MinFilter;
    GLint MagFilter;
} PS2_Texture;

typedef struct ps2_texture_context_t
{
    PS2_Texture Textures[PS2_TEXTURES_MAX];

    PS2_2DCoords CurrentTexCoords; // for immediate mode
    GLuint BoundTexture;

    PS2_GLArray CurrentTexCoordsArray;
} PS2_TextureContext;

#endif
