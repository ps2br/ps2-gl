#include "ps2gl/render.h"
#include <GL/gl.h>
#include <gsKit.h>
#include <ps2gl/context.h>

static GLfloat vertices[] = {
    -.5f, -.5f, 0.f, // left bottom
    .5f,  -.5f, 0.f, // right bottom
    .5f,  .5f,  0.f, // right top
    -.5f, .5f,  0.f  // left top
};

static GLfloat texcoords[] = {
    0.f, 0.f, // 0
    1.f, 0.f, // 1
    1.f, 1.f, // 2
    0.f, 1.f  // 3
};

static GLubyte indices[] = {
    0, 1, 2, // 1 triangle
    2, 3, 0  // 2 triangle
};

int
main (void)
{
    PS2GL_Renderer *renderer = PS2GL_InitGSKitRenderer ();
    PS2GL_Init (renderer);

    glViewport (0, 0, renderer->GetWidth (renderer),
                renderer->GetHeight (renderer));

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glFrustum (-1.f, 1.f, -1.f, 1.f, 1.f, 100.f);

    float angle = 0.f;

    GLuint texs[2];

    GLubyte ub_pixels[] = { 255, 0, 0,   255, 0,   255, 0,   255,
                            0,   0, 255, 255, 255, 255, 255, 255 };

    GLfloat f_pixels[] = { 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f,
                           0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f };

    glGenTextures (2, texs);

    glBindTexture (GL_TEXTURE_2D, texs[0]);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA,
                  GL_UNSIGNED_BYTE, ub_pixels);

    glBindTexture (GL_TEXTURE_2D, texs[1]);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT,
                  f_pixels);

    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);

    glVertexPointer (3, GL_FLOAT, 0, vertices);
    glTexCoordPointer (2, GL_FLOAT, 0, texcoords);

    while (1)
    {
        glClearColor (0.f, 0.f, 0.f, 1.f);
        glClear (GL_COLOR_BUFFER_BIT);

        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity ();

        glTranslatef (0.f, 0.f, -5.f);
        glRotatef (angle, 0.f, 1.f, 0.f);

        glEnable (GL_TEXTURE_2D);
        glColor4ub (255, 255, 255, 255);
        glBindTexture (GL_TEXTURE_2D, texs[0]);

        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

        gl.Renderer->SwapBuffers (gl.Renderer);

        angle += 2.f;
        if (angle >= 360.f)
            angle = 0.f;
    }

    PS2GL_Shutdown ();
    return 0;
}
