#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "renderer.hpp"


/*
 * process_window(): This function handles Windows callbacks.
 */
LRESULT CALLBACK process_window(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	switch(uiMsg) {
		case WM_CLOSE:
				PostQuitMessage(0);
				return 0;

		case WM_ACTIVATE:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SIZE:
				return 0;
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}


/* 
 * create_window(): Set up Windows specific bits.
 *
 * uiWidth:	 Width of window to create.
 * uiHeight:	Height of window to create.
 *
 * Returns:	 Device specific window handle.
 */
HWND create_window(
    int uiWidth, 
    int uiHeight
    ) 
{
	WNDCLASS wc;
	RECT wRect;
	HWND sWindow;
	HINSTANCE hInstance;

	wRect.left = 0L;
	wRect.right = (long)uiWidth;
	wRect.top = 0L;
	wRect.bottom = (long)uiHeight;

	hInstance = GetModuleHandle(NULL);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)process_window;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR) "OGLES";

	RegisterClass(&wc);

	AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	sWindow = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, (LPCWSTR) "OGLES", (LPCWSTR) "main", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, uiWidth, uiHeight, NULL, NULL, hInstance, NULL);

	ShowWindow(sWindow, SW_SHOW);
	SetForegroundWindow(sWindow);
	SetFocus(sWindow);

	return sWindow;
}


/* 
 * Loads the shader source into memory.
 *
 * sFilename: String holding filename to load 
 */
char * load_shader(
    char *sFilename
    )
{
    char *pResult = NULL;
    FILE *pFile = NULL;
    long iLen = 0;

    pFile = fopen(sFilename, "r");
    if(pFile == NULL)
    {
        fprintf(stderr, "Error: Cannot read file '%s'\n", sFilename);
  		exit(-1);
    }

    fseek(pFile, 0, SEEK_END); /* Seek end of file */
    iLen = ftell(pFile);
    fseek(pFile, 0, SEEK_SET); /* Seek start of file again */
    pResult = (char *) calloc(iLen+1, sizeof(char));
    fread(pResult, sizeof(char), iLen, pFile);
    pResult[iLen] = '\0';
    fclose(pFile);

    return pResult;
}

/* 
 * Create shader, load in source, compile, dump debug as necessary.
 *
 * pShader: Pointer to return created shader ID.
 * sFilename: Passed-in filename from which to load shader source.
 * iShaderType: Passed to GL, e.g. GL_VERTEX_SHADER.
 */
void process_shader(
    GLuint *pShader,
    char *sFilename,
    GLint iShaderType
    )
{
	GLint iStatus;
	const char *aStrings[1] = { NULL };

	/* Create shader and load into GL. */
	*pShader = glCreateShader(iShaderType);
	
	aStrings[0] = load_shader(sFilename);
	
	// Set shader source.
	glShaderSource(*pShader, 1, aStrings, NULL);

	/* Clean up shader source. */
	free((void *) aStrings[0]);
	aStrings[0] = NULL;

	// Compile shader.
	glCompileShader(*pShader);

	// Get shader status.
	glGetShaderiv(*pShader,
	              GL_COMPILE_STATUS,
	              &iStatus);

	// Dump debug info (source and log) if compilation failed.
	if(iStatus != GL_TRUE)
	{
#ifdef DEBUG
		GLint iLen;
		char *sDebugSource = NULL;
		char *sErrorLog = NULL;

		/* Get shader source. */
		GL_CHECK(glGetShaderiv(*pShader, GL_SHADER_SOURCE_LENGTH, &iLen));

		sDebugSource = malloc(iLen);

		GL_CHECK(glGetShaderSource(*pShader, iLen, NULL, sDebugSource));
		
		printf("Debug source START:\n%s\nDebug source END\n\n", sDebugSource);
		free(sDebugSource);

		/* Now get the info log. */
		GL_CHECK(glGetShaderiv(*pShader, GL_INFO_LOG_LENGTH, &iLen));
		
		sErrorLog = malloc(iLen);
		
		GL_CHECK(glGetShaderInfoLog(*pShader, iLen, NULL, sErrorLog));
		
		printf("Log START:\n%s\nLog END\n\n", sErrorLog);
		free(sErrorLog);
#endif

		exit(-1);
	}
}


static void ortho_matrix(
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


Renderer::Renderer(
    void
    )
{
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

    EGLConfig           aEGLConfigs[1];
    EGLint              cEGLConfigs;
    const unsigned int  uiWidth  = 640;
    const unsigned int  uiHeight = 480;


    // Get display.
    m_hDisplay    = EGL_DEFAULT_DISPLAY;
    m_sEGLDisplay = eglGetDisplay(m_hDisplay);

    // Initialize display.
    eglInitialize(m_sEGLDisplay, NULL, NULL);

    // Choose configuration.
    eglChooseConfig(m_sEGLDisplay, aEGLAttributes, aEGLConfigs, 1, &cEGLConfigs);
    assert(cEGLConfigs != 0); 

    m_hWindow = create_window(uiWidth, uiHeight);

    // Create surface based on window and display.
    m_sEGLSurface = eglCreateWindowSurface(m_sEGLDisplay,
                                           aEGLConfigs[0],
                                           (EGLNativeWindowType) m_hWindow,
                                           NULL);
    assert(m_sEGLSurface != EGL_NO_SURFACE); 

    // Create context.
    m_sEGLContext = eglCreateContext(m_sEGLDisplay, aEGLConfigs[0], EGL_NO_CONTEXT, aEGLContextAttributes);
    assert(m_sEGLContext != EGL_NO_CONTEXT); 

    // Make current.
    eglMakeCurrent(m_sEGLDisplay, m_sEGLSurface, m_sEGLSurface, m_sEGLContext);

    // Shader Initialisation
    process_shader(&m_uiVertShader, "C:\\cygwin\\home\\richard\\dev\\gles_app\\gles_app\\shader.vert", GL_VERTEX_SHADER);
    process_shader(&m_uiFragShader, "C:\\cygwin\\home\\richard\\dev\\gles_app\\gles_app\\shader.frag", GL_FRAGMENT_SHADER);

    // Create uiProgram (ready to attach shaders)
    m_uiProgram = glCreateProgram();

    // Attach shaders and link uiProgram
    glAttachShader(m_uiProgram, m_uiVertShader);
    glAttachShader(m_uiProgram, m_uiFragShader);
    glLinkProgram(m_uiProgram);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}


Renderer::~Renderer()
{
    /* Cleanup shaders */
    glUseProgram(0);
    glDeleteShader(m_uiVertShader);
    glDeleteShader(m_uiFragShader);
    glDeleteProgram(m_uiProgram);

    /* EGL clean up */
    eglMakeCurrent(m_sEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(m_sEGLDisplay, m_sEGLSurface);
    eglDestroyContext(m_sEGLDisplay, m_sEGLContext);
    eglTerminate(m_sEGLDisplay);
}


void Renderer::entityAdd()
{

}


void Renderer::entityRemove()
{

}


void Renderer::draw(
    void
    )
{

}


void Renderer::render(
    void
    )
{
    GLint           position, color, mvp, texCoords;
    float           ortho[16];
    unsigned int    i;


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(m_uiProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Get uniform location.
    mvp = glGetUniformLocation(m_uiProgram, "mvp");
    assert(mvp != -1);

    // TODO: can this be be initialized only once?
    ortho_matrix(0, 2, 2, 0, ortho);
    glUniformMatrix4fv(mvp, 1, GL_FALSE, ortho);

    for(i = 0; i < m_entityAttributes.size(); i++)
    {
        // Get attributes.
        position = glGetAttribLocation(m_uiProgram, "mPosition");
        assert(position != -1);

        color = glGetAttribLocation(m_uiProgram, "av3colour");
        assert(color != -1);

        texCoords = glGetAttribLocation(m_uiProgram, "mTexCoords");
        assert(texCoords != -1);
        
        // Enable attributes.
        glEnableVertexAttribArray(position);
        glEnableVertexAttribArray(color);
        glEnableVertexAttribArray(texCoords);

        // Set attributes.
        glVertexAttribPointer(position,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              &m_entityAttributes[i].vertices[0]);

        glVertexAttribPointer(color,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              &m_entityAttributes[i].colors[0]);

        glVertexAttribPointer(texCoords,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              &m_entityAttributes[i].texCoords[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &m_entityAttributes[i].indices[0]);
    }

    if(!eglSwapBuffers(m_sEGLDisplay, m_sEGLSurface))
    {
        printf("Failed to swap buffers.\n");
    }
}
