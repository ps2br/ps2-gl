#ifndef ps2gl_render_h
#define ps2gl_render_h

#include <GL/gl.h>
#include <tamtypes.h>

#include "ps2gl/types.h"

/** Renderer, Implementation specific */
typedef struct ps2gl_renderer_t PS2GL_Renderer;

/** Texture, Implementation-specific */
/** ex: in gsKit it may be GSTEXTURE */
typedef struct ps2gl_impl_texture_t PS2GL_ImplTexture;

typedef enum ps2gl_texture_psm_t
{
    PS2GL_PSM_24,
    PS2GL_PSM_32
} PS2GL_TexturePSM;

typedef enum ps2gl_texture_filter_t
{
    PS2GL_FILTER_NEAREST,
    PS2GL_FILTER_LINEAR
} PS2GL_TextureFilter;

/** RENDERER */
PS2GL_Renderer *PS2GL_InitRenderer (void);

void PS2GL_ClearRenderColor (PS2GL_Renderer *, PS2GL_Color);
GLsizei PS2GL_GetRendererWidth (PS2GL_Renderer *);
GLsizei PS2GL_GetRendererHeight (PS2GL_Renderer *);
void PS2GL_SetRendererScissor (PS2GL_Renderer *, GLint x, GLint y,
                               GLsizei width, GLsizei height);
void PS2GL_ResetRendererScissor (PS2GL_Renderer *);
void PS2GL_DeInitRenderer (PS2GL_Renderer *);
/** END RENDERER */

/** TEXTURE */

/** Allocates the ImplTexture and fills Implementation-Specic data */
/** NOTE: this don't fills the image data (pixels) */
PS2GL_ImplTexture *PS2GL_CreateImplTexture (int width, int height,
                                            PS2GL_TexturePSM psm,
                                            PS2GL_TextureFilter filter);

/** Returns a pointer of allocate memory of Texture image
 * Should be filled by user
 * if PSM is 32, width is 200 and Heighr is 300
 * the allocated size is (200 * height * 4) => 240000 (bytes)
 */
void *PS2GL_AllocateImplTextureMem (PS2GL_ImplTexture *);

u32 PS2GL_GetImplTextureSize (PS2GL_ImplTexture *);

/** Send the Texture into PS2 VRAM and Frees from RAM */
void PS2GL_FinishImplTextureCreation (PS2GL_Renderer *, PS2GL_ImplTexture *);

void PS2GL_SetImplTextureFilter (PS2GL_ImplTexture *, PS2GL_TextureFilter);

/** END TEXTURE */

/** TIANGLE */
void PS2GL_DrawFilledTriangle (PS2GL_Renderer *, PS2GL_Vertex *a,
                               PS2GL_Vertex *b, PS2GL_Vertex *c);

void PS2GL_DrawFilledTexturedTriangle (PS2GL_Renderer *, PS2GL_Vertex *a,
                                       PS2GL_Vertex *b, PS2GL_Vertex *c,
                                       PS2GL_ImplTexture *texture);

/** END TRIANGLE */

#endif
