/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#include "cube.h"
#include "shader.h"
#include "window.h"
#include "matrix.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"

HWND hWindow;
HDC  hDisplay;

const unsigned short aTexCoords[] =
{
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0
};

const unsigned short aIndices[] =
{
    0, 2, 1,
    0, 3, 2
};

/* 3D data. Vertex range -0.5..0.5 in all axes.
* Z -0.5 is near, 0.5 is far. */
const float aVertices[] =
{
    /* Front face. */
    /* Bottom left */
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    /* Top right */
    0,  0.5, 0,
    0.5,  0.5, 0,
    0.5, 0, 0,
    /* Left face */
    /* Bottom left */
    -0.5,  0.5,  0.5,
    -0.5, -0.5, -0.5,
    -0.5, -0.5,  0.5,
    /* Top right */
    -0.5,  0.5,  0.5,
    -0.5,  0.5, -0.5,
    -0.5, -0.5, -0.5,
    /* Top face */
    /* Bottom left */
    -0.5,  0.5,  0.5,
    0.5,  0.5, -0.5,
    -0.5,  0.5, -0.5,
    /* Top right */
    -0.5,  0.5,  0.5,
    0.5,  0.5,  0.5,
    0.5,  0.5, -0.5,
    /* Right face */
    /* Bottom left */
    0.5,  0.5, -0.5,
    0.5, -0.5,  0.5,
    0.5, -0.5, -0.5,
    /* Top right */
    0.5,  0.5, -0.5,
    0.5,  0.5,  0.5,
    0.5, -0.5,  0.5,
    /* Back face */
    /* Bottom left */
    0.5,  0.5,  0.5,
    -0.5, -0.5,  0.5,
    0.5, -0.5,  0.5,
    /* Top right */
    0.5,  0.5,  0.5,
    -0.5,  0.5,  0.5,
    -0.5, -0.5,  0.5,
    /* Bottom face */
    /* Bottom left */
    -0.5, -0.5, -0.5,
    0.5, -0.5,  0.5,
    -0.5, -0.5,  0.5,
    /* Top right */
    -0.5, -0.5, -0.5,
    0.5, -0.5, -0.5,
    0.5, -0.5,  0.5,
};

const float aColours[] =
{
    /* Front face */
    /* Bottom left */
    1.0, 0.0, 0.0, /* red */
    0.0, 0.0, 1.0, /* blue */
    0.0, 1.0, 0.0, /* green */
    /* Top right */
    1.0, 0.0, 0.0, /* red */
    1.0, 1.0, 0.0, /* yellow */
    0.0, 0.0, 1.0, /* blue */
    /* Left face */
    /* Bottom left */
    1.0, 1.0, 1.0, /* white */
    0.0, 1.0, 0.0, /* green */
    0.0, 1.0, 1.0, /* cyan */
    /* Top right */
    1.0, 1.0, 1.0, /* white */
    1.0, 0.0, 0.0, /* red */
    0.0, 1.0, 0.0, /* green */
    /* Top face */
    /* Bottom left */
    1.0, 1.0, 1.0, /* white */
    1.0, 1.0, 0.0, /* yellow */
    1.0, 0.0, 0.0, /* red */
    /* Top right */
    1.0, 1.0, 1.0, /* white */
    0.0, 0.0, 0.0, /* black */
    1.0, 1.0, 0.0, /* yellow */
    /* Right face */
    /* Bottom left */
    1.0, 1.0, 0.0, /* yellow */
    1.0, 0.0, 1.0, /* magenta */
    0.0, 0.0, 1.0, /* blue */
    /* Top right */
    1.0, 1.0, 0.0, /* yellow */
    0.0, 0.0, 0.0, /* black */
    1.0, 0.0, 1.0, /* magenta */
    /* Back face */
    /* Bottom left */
    0.0, 0.0, 0.0, /* black */
    0.0, 1.0, 1.0, /* cyan */
    1.0, 0.0, 1.0, /* magenta */
    /* Top right */
    0.0, 0.0, 0.0, /* black */
    1.0, 1.0, 1.0, /* white */
    0.0, 1.0, 1.0, /* cyan */
    /* Bottom face */
    /* Bottom left */
    0.0, 1.0, 0.0, /* green */
    1.0, 0.0, 1.0, /* magenta */
    0.0, 1.0, 1.0, /* cyan */
    /* Top right */
    0.0, 1.0, 0.0, /* green */
    0.0, 0.0, 1.0, /* blue */
    1.0, 0.0, 1.0, /* magenta */
};


void ortho_matrix(
    float left,
    float right, 
    float bottom, 
    float top,
    float matrix[16]
    )
{
    float f = 1.0f; 
    float n = -1.0f; 


    memset(matrix, 0, sizeof(float) * 16); 

    // Setup identify matrix.
    matrix[0*4 + 0] = 1; 
    matrix[1*4 + 1] = 1; 
    matrix[2*4 + 2] = 1; 
    matrix[3*4 + 3] = 1; 

    // Setup ortho matrix. 
    matrix[0*4 + 0] = 2.0f / (right - left); 
    matrix[1*4 + 1] = 2.0f / (top - bottom); 
    matrix[2*4 + 2] = -2.0f / (f - n); 
    matrix[3*4 + 0] = - (right + left) / (right - left); 
    matrix[3*4 + 1] = - (top + bottom) / (top - bottom); 
    matrix[3*4 + 2] = - (f + n) / (f - n); 
}


int main(
    int     argc,
    char  **argv
    )
{
    EGLDisplay	sEGLDisplay;
    EGLContext	sEGLContext;
    EGLSurface	sEGLSurface;
    GLuint      textureId;
    unsigned char artificial_texture[400*400*4];


    /* EGL Configuration */

    EGLint aEGLAttributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLint aEGLContextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLConfig	aEGLConfigs[1];
    EGLint		cEGLConfigs;

#ifdef _WIN32
    MSG sMessage;
#else
    XSetWindowAttributes win_attrs;
    int attrs[64], idx = 0, num_config = 0;
    int major, minor;
    Colormap colormap;
    XVisualInfo *pVisual;
    XEvent e;
#endif

    GLint iLocPosition = 0;

    GLint iLocColour, iLocTexCoord, iLocNormal, iLocMVP;
    GLint iLocXangle, iLocYangle, iLocZangle;
    GLint iLocAspect, iLocLightPos, iLocSampler, iLocSampler2;

    GLuint uiProgram, uiFragShader, uiVertShader;

    GLenum myTex, myTex2;

    int bDone = 0;

    const unsigned int uiWidth  = 640;
    const unsigned int uiHeight = 480;

    int iXangle = 0, iYangle = 0, iZangle = 0;

    float aTBNmatrix1[9], aTBNmatrix2[9];

    float aLightPos[] = { 0.0f, 0.0f, -1.0f }; // Light is nearest camera.

    unsigned char *myPixels = calloc(1, 128*128*4); // Holds texture data.
    unsigned char *myPixels2 = calloc(1, 128*128*4); // Holds texture data.

    float aRotate[16], aModelView[16], aPerspective[16], aMVP[16], projection_matrix[16];

    int i;

    /* EGL Init */
    hDisplay = EGL_DEFAULT_DISPLAY;

    // 1. Get display.
    sEGLDisplay = EGL_CHECK(eglGetDisplay(hDisplay));

    // 2. Initialize display.
    EGL_CHECK(eglInitialize(sEGLDisplay, NULL, NULL));

    // 3. Choose configuration.
    eglChooseConfig(sEGLDisplay, aEGLAttributes, aEGLConfigs, 1, &cEGLConfigs);
    if (cEGLConfigs == 0) 
    {
        printf("No EGL configurations were returned.\n");
        exit(-1);
    }

    hWindow = create_window(uiWidth, uiHeight);

    // 4. Create surface based on window and display.
    sEGLSurface = EGL_CHECK(eglCreateWindowSurface(sEGLDisplay,
                                                   aEGLConfigs[0],
                                                   (EGLNativeWindowType) hWindow,
                                                   NULL));
    if(sEGLSurface == EGL_NO_SURFACE) 
    {
        printf("Failed to create EGL surface.\n");
        exit(-1);
    }

    // 5. Create context.
    sEGLContext = eglCreateContext(sEGLDisplay, aEGLConfigs[0], EGL_NO_CONTEXT, aEGLContextAttributes);
    if(sEGLContext == EGL_NO_CONTEXT) 
    {
        printf("Failed to create EGL context.\n");
        exit(-1);
    }

    // 6. Make current.
    eglMakeCurrent(sEGLDisplay, sEGLSurface, sEGLSurface, sEGLContext);

    /* Shader Initialisation */
    process_shader(&uiVertShader, "C:\\cygwin\\home\\richard\\dev\\gles_app\\gles_app\\shader.vert", GL_VERTEX_SHADER);
    process_shader(&uiFragShader, "C:\\cygwin\\home\\richard\\dev\\gles_app\\gles_app\\shader.frag", GL_FRAGMENT_SHADER);

    /* Create uiProgram (ready to attach shaders) */
    uiProgram = glCreateProgram();

    /* Attach shaders and link uiProgram */
    glAttachShader(uiProgram, uiVertShader);
    glAttachShader(uiProgram, uiFragShader);
    glLinkProgram(uiProgram);

    /* Get attribute locations of non-fixed attributes like colour and texture coordinates. */
    iLocPosition = glGetAttribLocation(uiProgram, "mPosition");
    if(iLocPosition == -1)
    {
        goto cleanup;
    }

    iLocColour = glGetAttribLocation(uiProgram, "av3colour");
    if(iLocColour == -1)
    {
        goto cleanup;
    }

    /* Get uniform locations */
    iLocMVP = glGetUniformLocation(uiProgram, "mvp");
    if(iLocMVP == -1)
    {
        goto cleanup;
    }

    glUseProgram(uiProgram);

    /* Enable attributes for position, colour and texture coordinates etc. */
    glEnableVertexAttribArray(iLocPosition);
    glEnableVertexAttribArray(iLocColour);

    /* Populate attributes for position, colour and texture coordinates etc. */
    glVertexAttribPointer(iLocPosition, 2, GL_FLOAT, GL_FALSE, 0, aVertices);
    glVertexAttribPointer(iLocColour, 3, GL_FLOAT, GL_FALSE, 0, aColours);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // set glTexParameteri

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Load texture.
    glGenTextures(1, &textureId); 
    if(textureId == 0)
    {
        goto cleanup;
    }

    // -------------------
    //  Setup texture
    // -------------------

    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    for(i = 0; i < 400*400; i += 4)
    {
        artificial_texture[i    ] = i; 
        artificial_texture[i + 1] = i; 
        artificial_texture[i + 2] = i; 
        artificial_texture[i + 3] = 255; 
    }

    glTexImage2D(GL_TEXTURE_2D,
                 0, 
                 (GLint) GL_RGBA, 
                 (GLsizei) 400, 
                 (GLsizei) 400, 
                 0, 
                 GL_RGBA, 
                 GL_UNSIGNED_BYTE,
                 artificial_texture); 

    glBindTexture(GL_TEXTURE_2D, 0);

    /* Enter event loop */
    while(!bDone) 
    {
        if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE)) 
        {
            if(sMessage.message == WM_QUIT) 
            {
                bDone = 1;
            } 
            else 
            {
                TranslateMessage(&sMessage);
                DispatchMessage(&sMessage);
            }
        }

#if 0
        /* 
        * Do some rotation with Euler angles. It is not a fixed axis as
        * quaterions would be, but the effect is cool. 
        */
        rotate_matrix(iXangle, 1.0, 0.0, 0.0, aModelView);
        rotate_matrix(iYangle, 0.0, 1.0, 0.0, aRotate);

        multiply_matrix(aRotate, aModelView, aModelView);

        rotate_matrix(iZangle, 0.0, 1.0, 0.0, aRotate);

        multiply_matrix(aRotate, aModelView, aModelView);

        /* Pull the camera back from the cube */
        aModelView[14] -= 2.5;

        perspective_matrix(45.0, (double)uiWidth/(double)uiHeight, 0.01, 100.0, aPerspective);
        multiply_matrix(aPerspective, aModelView, aMVP);
#endif

        // perspective_matrix(45.0, (double)uiWidth/(double)uiHeight, 0.01, 100.0, aPerspective);
        ortho_matrix(0, 2, 2, 0, aPerspective); 
        glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aPerspective);

#if 1
        iXangle += 3;
        iYangle += 2;
        iZangle += 1;

        if(iXangle >= 360) iXangle -= 360;
        if(iXangle < 0) iXangle += 360;
        if(iYangle >= 360) iYangle -= 360;
        if(iYangle < 0) iYangle += 360;
        if(iZangle >= 360) iZangle -= 360;
        if(iZangle < 0) iZangle += 360;
#endif

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#if 0
        glDrawArrays(GL_TRIANGLES, 0, 6);
#else
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &aIndices[0]);
#endif
          

        if(!eglSwapBuffers(sEGLDisplay, sEGLSurface)) 
        {
            printf("Failed to swap buffers.\n");
        }

        Sleep(20);
    }

cleanup:
    /* Cleanup shaders */
    GL_CHECK(glUseProgram(0));
    GL_CHECK(glDeleteShader(uiVertShader));
    GL_CHECK(glDeleteShader(uiFragShader));
    GL_CHECK(glDeleteProgram(uiProgram));

    /* EGL clean up */
    EGL_CHECK(eglMakeCurrent(sEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
    EGL_CHECK(eglDestroySurface(sEGLDisplay, sEGLSurface));
    EGL_CHECK(eglDestroyContext(sEGLDisplay, sEGLContext));
    EGL_CHECK(eglTerminate(sEGLDisplay));

    return 0;
}
