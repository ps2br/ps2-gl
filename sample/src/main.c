#include <GL/gl.h>
#include <gsKit.h>
#include <ps2Kit.h>

static GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
};

static GLfloat texcoords[] = {
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
};

static GLubyte indices[] = { 0, 1, 2, 2, 3, 0 };

int
main (void)
{
    GSGLOBAL *gs = PS2Kit_InitPS2GL ();

    glViewport (0, 0, gs->Width, gs->Height);

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

        PS2Kit_SwapBuffers (gs);
        PS2Kit_QeueExec (gs);

        angle += 2.f;
        if (angle >= 360.f)
            angle = 0.f;
    }

    PS2Kit_Shutdown (gs);
    return 0;
}
