
#include "cube.h"
#include "shader.h"
#include "window.h"
#include "matrix.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"

#include <il/il.h>

HWND hWindow;
HDC  hDisplay;

const float aTexCoords[] =
{
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
};

const unsigned short aIndices[] =
{
    0, 2, 1,    // Triangle 1
    0, 3, 2     // Triangle 2
};

const float aVertices[] =
{
    0.0, 0.0,   // 0 
    1.0, 0.0,   // 1
    1.0, 1.0,   // 2
    0.0, 1.0,   // 3
};

const float aColours[] =
{
    1.0, 0.0, 0.0, /* red */
    0.0, 0.0, 1.0, /* blue */
    0.0, 1.0, 0.0, /* green */
    1.0, 1.0, 0.0, /* yellow */
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

void loadTexture(
    char           *path, 
    unsigned int   *width, 
    unsigned int   *height, 
    char          **pixels
    )
{
    ILuint  texid; 
//    ILenum  error; 
    char *location = "C:/cygwin/home/richard/dev/gles_app/gles_app/player.png"; 

    ilInit();

    ilGenImages(1, &texid); 
    ilBindImage(texid); 
    ilLoadImage((const wchar_t *) location); 

    *width = ilGetInteger(IL_IMAGE_WIDTH); 
    *height = ilGetInteger(IL_IMAGE_HEIGHT);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 

    *pixels = malloc((*width) * (*height) * 4); 

    memcpy(*pixels, (void *) ilGetData(), (*width) * (*height) * 4);
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

    EGLint aEGLAttributes[] = 
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLint aEGLContextAttributes[] = 
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLConfig	    aEGLConfigs[1];
    EGLint		    cEGLConfigs;
    MSG             sMessage;
    GLint           iLocPosition = 0;
    GLint           iLocColour, iLocMVP, iTexCoords;
    GLuint          uiProgram, uiFragShader, uiVertShader;
    int             bDone = 0;
    const unsigned int uiWidth  = 640;
    const unsigned int uiHeight = 480;
    unsigned char   color = 0; 
    float           aPerspective[16];
    int             i, j;
    unsigned int width, height; 
    char *pixel_data = NULL; 


    // Get display.
    hDisplay    = EGL_DEFAULT_DISPLAY;
    sEGLDisplay = eglGetDisplay(hDisplay);

    // Initialize display.
    eglInitialize(sEGLDisplay, NULL, NULL);

    // Choose configuration.
    eglChooseConfig(sEGLDisplay, aEGLAttributes, aEGLConfigs, 1, &cEGLConfigs);
    if (cEGLConfigs == 0) 
    {
        printf("No EGL configurations were returned.\n");
        exit(-1);
    }

    hWindow = create_window(uiWidth, uiHeight);

    // Create surface based on window and display.
    sEGLSurface = EGL_CHECK(eglCreateWindowSurface(sEGLDisplay,
                                                   aEGLConfigs[0],
                                                   (EGLNativeWindowType) hWindow,
                                                   NULL));
    if(sEGLSurface == EGL_NO_SURFACE) 
    {
        printf("Failed to create EGL surface.\n");
        exit(-1);
    }

    // Create context.
    sEGLContext = eglCreateContext(sEGLDisplay, aEGLConfigs[0], EGL_NO_CONTEXT, aEGLContextAttributes);
    if(sEGLContext == EGL_NO_CONTEXT) 
    {
        printf("Failed to create EGL context.\n");
        exit(-1);
    }

    // Make current.
    eglMakeCurrent(sEGLDisplay, sEGLSurface, sEGLSurface, sEGLContext);

    // Shader Initialisation
    process_shader(&uiVertShader, "C:\\cygwin\\home\\richard\\dev\\gles_app\\gles_app\\shader.vert", GL_VERTEX_SHADER);
    process_shader(&uiFragShader, "C:\\cygwin\\home\\richard\\dev\\gles_app\\gles_app\\shader.frag", GL_FRAGMENT_SHADER);

    // Create uiProgram (ready to attach shaders)
    uiProgram = glCreateProgram();

    // Attach shaders and link uiProgram
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

    iTexCoords = glGetAttribLocation(uiProgram, "mTexCoords");
    if(iTexCoords == -1)
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
    glEnableVertexAttribArray(iTexCoords);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    loadTexture("test", &width, &height, &pixel_data); 

    glTexImage2D(GL_TEXTURE_2D,
                 0, 
                 (GLint) GL_RGBA, 
                 (GLsizei) width, 
                 (GLsizei) height, 
                 0, 
                 GL_RGBA, 
                 GL_UNSIGNED_BYTE,
                 pixel_data); 

    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, textureId);

    /* Populate attributes for position, colour and texture coordinates etc. */
    glVertexAttribPointer(iLocPosition, 2, GL_FLOAT, GL_FALSE, 0, aVertices);
    glVertexAttribPointer(iLocColour, 3, GL_FLOAT, GL_FALSE, 0, aColours);
    glVertexAttribPointer(iTexCoords, 2, GL_FLOAT, GL_FALSE, 0, aTexCoords);

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

        // perspective_matrix(45.0, (double)uiWidth/(double)uiHeight, 0.01, 100.0, aPerspective);
        ortho_matrix(0, 2, 2, 0, aPerspective); 
        glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aPerspective);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &aIndices[0]);

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
