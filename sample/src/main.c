#include <GL/gl.h>
#include <gsKit.h>
#include <ps2gl/context.h>

int
main (void)
{
    GSGLOBAL *gs = gsKit_init_global ();
    gsKit_init_screen (gs);

    /** Initialize PS2gl */
    /** It will also initilaze *dmaKit* and *dmaKit chan* */
    PS2_GLInit (gs);

    glViewport (0, 0, gs->Width, gs->Height);

    /** setup proj matrix */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glFrustum (-1.f, 1.f, -1.f, 1.f, 1.f, 100.f);

    float angle = 0.f;

    /** texture test */
    GLuint texs[2];
    unsigned char ub_pixels[] = {
        255, 0,   0,   255, // full red
        0,   255, 0,   255, // full green
        0,   0,   255, 255, // full blue
        255, 255, 255, 255  // full whize
    };

    GLfloat f_pixels[] = {
        1.f, 1.f, 1.f, 1.f, // full white
        0.f, 0.f, 1.f, 1.f, // full blue
        0.f, 1.f, 0.f, 1.f, // full green
        1.f, 0.f, 0.f, 1.f  // full red
    };

    glGenTextures (2, texs);

    /** setup tex 1 */
    glBindTexture (GL_TEXTURE_2D, texs[0]);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA,
                  GL_UNSIGNED_BYTE, ub_pixels);

    /** setup tex 2 */
    glBindTexture (GL_TEXTURE_2D, texs[1]);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT,
                  f_pixels);

    while (1)
    {
        glClearColor (0.f, 0.f, 0.f, 1.f);
        glClear (GL_COLOR_BUFFER_BIT);

        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity ();

        glTranslatef (0.f, 0.f, -5.f);
        glRotatef (angle, 0.f, 1.f, 0.f);

        glEnable (GL_TEXTURE_2D);
        glColor4ub (255, 255, 255, 255); // use white if tex not work
        glBindTexture (GL_TEXTURE_2D, texs[0]);

        glBegin (GL_TRIANGLES);
        {
            glTexCoord2f (0.f, 0.f);
            glVertex3f (-.5f, -.5f, 0.f);

            glTexCoord2f (1.f, 0.f);
            glVertex3f (.5f, -.5f, 0.f);

            glTexCoord2f (.5f, 1.f);
            glVertex3f (0.f, .5f, 0.f);
        }
        glEnd ();

        gsKit_sync_flip (gs);
        gsKit_queue_exec (gs);

        angle += 2.f;
        if (angle >= 360.f)
            angle = 0.f;
    }

    PS2_GLShutdown ();
    return 0;
}
