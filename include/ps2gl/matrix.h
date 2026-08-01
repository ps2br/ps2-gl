#ifndef ps2gl_matrix_h
#define ps2gl_matrix_h

#include <GL/gl.h>
#include <linmath.h>

#define PS2_MATRIX_STACK_MAX 32

typedef struct ps2_matrix_context
{
    mat4x4 ModelView;
    mat4x4 Projection;

    mat4x4 ModelViewStack[PS2_MATRIX_STACK_MAX];
    mat4x4 ProjectionStack[PS2_MATRIX_STACK_MAX];

    int ModelViewTop;
    int ProjectionTop;

    GLenum Mode;
} PS2_MatrixContext;

mat4x4 *PS2_CurrentMatrix (void);
void PS2_MatrixIdentity (mat4x4);
void PS2_MatrixMultiply (mat4x4, const mat4x4, const mat4x4);
void PS2_TransformVertex (float, float, float, float *, float *, float *);

#endif
