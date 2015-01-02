#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <deque>

#include "constants.hpp"
#include "Renderer.h"
#include "Matrix.h"
#include "open-simplex-noise.h"


typedef struct {
    GLuint program;

    GLint a_normal;
    GLint a_position;
    GLint u_mvp;
    GLint u_color;
    GLint u_light;

} TextureProgram;

// static Table table;
// static Wall wall;
static TextureProgram texture_program;
// static std::deque<GLuint>   wall_vbos;
static std::deque<GLuint>   terrian_vbos;

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
 * uiWidth:     Width of window to create.
 * uiHeight:    Height of window to create.
 *
 * Returns:     Device specific window handle.
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


#define TERRAIN_WIDTH   10
#define TERRAIN_HEIGHT  10
#define TERRAIN_DEPTH   1
#define OCTAVE_COUNT    6
#define PERSISTANCE     0.5f
#define UNIT_LEN        2

float interpolate(
    float x0,
    float x1,
    float alpha
    )
{
    return x0 * (1 - alpha) + alpha * x1;
}



static void printMap(
    float   map[TERRAIN_WIDTH][TERRAIN_HEIGHT]
    )
{
    for(auto i = 0; i < TERRAIN_WIDTH; ++i)
    {
        for(auto j = 0; j < TERRAIN_HEIGHT; ++j)
        {
//            LOGE("### map[%u][%u] = %f", i, j, map[i][j]);
        }
    }
}


static void whiteNoiseGenerate(
    float heightMap[TERRAIN_WIDTH][TERRAIN_HEIGHT]
    )
{
    for(auto i = 0; i < TERRAIN_WIDTH; ++i)
    {
        for(auto j = 0; j < TERRAIN_HEIGHT; ++j)
        {
            heightMap[i][j] = (double)rand() / (double)RAND_MAX ;
        }
    }

//    LOGE("### white noise map");
//    printMap(heightMap);
}


static void smoothNoiseGenerate(
    float   whiteNoise[TERRAIN_WIDTH][TERRAIN_HEIGHT],
    int     octave,
    float   smoothNoise[TERRAIN_WIDTH][TERRAIN_HEIGHT]
    )
{
    int samplePeriod = 1 << octave;
    float sampleFrequency = 1.0f / samplePeriod;

    for(int i = 0; i < TERRAIN_WIDTH; ++i)
    {
        int sample_i0 = (i / samplePeriod) * samplePeriod;
        int sample_i1 = (sample_i0 + samplePeriod) % TERRAIN_WIDTH;
        float horizontal_blend = (i - sample_i0) * sampleFrequency;

        for(int j = 0; j < TERRAIN_HEIGHT; ++j)
        {
            int sample_j0 = (j / samplePeriod) * samplePeriod;
            int sample_j1 = (sample_j0 + samplePeriod) % TERRAIN_HEIGHT;
            float vertical_blend = (j - sample_j0) * sampleFrequency;

            float top = interpolate(
                whiteNoise[sample_i0][sample_j0],
                whiteNoise[sample_i1][sample_j0],
                horizontal_blend);

            float bottom = interpolate(
                whiteNoise[sample_i0][sample_j1],
                whiteNoise[sample_i1][sample_j1],
                horizontal_blend);

            smoothNoise[i][j] = interpolate(
                top,
                bottom,
                vertical_blend);
        }
    }
}


static void perlinNoiseGenerate(
    float   perlinNoise[TERRAIN_WIDTH][TERRAIN_HEIGHT]
    )
{
    float persistance = PERSISTANCE;
    float smoothNoiseMap[OCTAVE_COUNT][TERRAIN_WIDTH][TERRAIN_HEIGHT];
    float baseNoise[TERRAIN_WIDTH][TERRAIN_HEIGHT];

    // Generate base noise.
    whiteNoiseGenerate(baseNoise);

    // Generate smooth noise based on base noise.
    for(auto i = 0; i < OCTAVE_COUNT; ++i)
    {
        smoothNoiseGenerate(baseNoise, i, smoothNoiseMap[i]);

        if(i == 5)
        {
//            LOGE("### smooth Noise map. octave = %u", i);
            printMap(smoothNoiseMap[i]);
        }
    }

    memset(perlinNoise, 0, sizeof(float)*TERRAIN_WIDTH*TERRAIN_HEIGHT);

    float amplitude         = 1.0f;
    float totalAmplitude    = 0.0f;

    for(auto octave = OCTAVE_COUNT - 1; octave >= 0; --octave)
    {
        amplitude       *= persistance;
        totalAmplitude  += amplitude;

        for(auto i = 0; i < TERRAIN_WIDTH; ++i)
        {
            for(auto j = 0; j < TERRAIN_HEIGHT; ++j)
            {
                perlinNoise[i][j] += smoothNoiseMap[octave][i][j] * amplitude;
            }
        }
    }

//    LOGE("### perlin noise before normalize");
//    printMap(perlinNoise);

    for(auto i = 0; i < TERRAIN_WIDTH; ++i)
    {
        for(auto j = 0; j < TERRAIN_HEIGHT; ++j)
        {
            perlinNoise[i][j] /= totalAmplitude;
        }
    }

//    LOGE("### perlin noise after normalize");
//    printMap(perlinNoise);

//    while(1);

//    while(1)
//    {
//        usleep(100000);
//    }
//    exit(0);
}


typedef struct
{
    std::vector<float>  vertex;
//    std::vector<float>  normal;

} sTERRIAN_STRUCT;

std::deque<sTERRIAN_STRUCT> f_terrain;

GLuint vboCreate(
    const GLsizeiptr    size,
    const GLvoid       *data,
    const GLenum        usage
    )
{
    assert(data != NULL);


    GLuint vbo_object;
    glGenBuffers(1, &vbo_object);
    assert(vbo_object != 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo_object;
}



static void simplex_test(void)
{
    float density_cube[TERRAIN_WIDTH][TERRAIN_HEIGHT][TERRAIN_DEPTH] = {0};
    float processed_cube[TERRAIN_WIDTH][TERRAIN_HEIGHT][TERRAIN_DEPTH] = {0};
    struct osn_context *ctx;


    // Initialize noise.
    open_simplex_noise(77374, &ctx);

    for(auto x = 0; x < TERRAIN_WIDTH; x++)
    {
        for(auto y = 0; y < TERRAIN_HEIGHT; y++)
        {
            for(auto z = 0; z < TERRAIN_HEIGHT; z++)
            {
                density_cube[x][y][z] = open_simplex_noise3(ctx, x, y, z);
            }
        }
    }

    for(auto x = 0; x < TERRAIN_WIDTH; x++)
    {
        for(auto y = 0; y < TERRAIN_HEIGHT; y++)
        {
            for(auto z = 0; z < TERRAIN_HEIGHT; z++)
            {
                if(density_cube[x][y][z] > 0)
                {
                    processed_cube[x][y][z] = (density_cube[x][y][z] > 0) ? 1 : 0;
                }
            }
        }
    }
}


static void surface_add(
    Vector3f           &v0,
    Vector3f           &v1,
    Vector3f           &v2,
    Vector3f           &v3,
    std::vector<float> &terrain
    )
{
    auto x = v2 - v0;
    auto y = v1 - v0;

    auto crossProduct = x.cross(y);
    auto normal = crossProduct.normalize();

    // 0 2 1    0, 3, 2
    terrain.push_back(v0[0]);
    terrain.push_back(v0[1]);
    terrain.push_back(v0[2]);

    terrain.push_back(normal.x);
    terrain.push_back(normal.y);
    terrain.push_back(normal.z);

    terrain.push_back(v2[0]);
    terrain.push_back(v2[1]);
    terrain.push_back(v2[2]);

    terrain.push_back(normal.x);
    terrain.push_back(normal.y);
    terrain.push_back(normal.z);

    terrain.push_back(v1[0]);
    terrain.push_back(v1[1]);
    terrain.push_back(v1[2]);

    terrain.push_back(normal.x);
    terrain.push_back(normal.y);
    terrain.push_back(normal.z);

    //            for(auto k = 0; k < 3; k++)
        //            {
        //                terrain.vertex.push_back(normal.x);
    //                terrain.vertex.push_back(normal.y);
    //                terrain.vertex.push_back(normal.z);
    //            }

    //            LOGE("### normal %f, %f, %f", normal.x, normal.y, normal.z);

    x = v2 - v0;
    y = v1 - v0;

    crossProduct = x.cross(y);
    normal = crossProduct.normalize();

    terrain.push_back(v0[0]);
    terrain.push_back(v0[1]);
    terrain.push_back(v0[2]);

    terrain.push_back(normal.x);
    terrain.push_back(normal.y);
    terrain.push_back(normal.z);

    terrain.push_back(v3[0]);
    terrain.push_back(v3[1]);
    terrain.push_back(v3[2]);

    terrain.push_back(normal.x);
    terrain.push_back(normal.y);
    terrain.push_back(normal.z);

    terrain.push_back(v2[0]);
    terrain.push_back(v2[1]);
    terrain.push_back(v2[2]);

    terrain.push_back(normal.x);
    terrain.push_back(normal.y);
    terrain.push_back(normal.z);

    //            auto x = v2 - v0;
    //            auto y = v1 - v0;
    //
    //            auto cross = {x[1]*y[2] - y[1]*x[2],
        //                          x[2]*y[0] - y[2]*x[0],
    //                          x[0]*y[1] - y[0]*x[1]};
    //
    //            auto normal = normalize(cross);



    //            for(auto k = 0; k < 3; k++)
    //            {
    //                terrain.vertex.push_back(normal.x);
    //                terrain.vertex.push_back(normal.y);
    //                terrain.vertex.push_back(normal.z);
    //            }
}


static Vector3f eye(20.0f, 20.0f, 20.0f);
static Vector3f light(1.0f, 1.0f, 1.0f);

static void cube_add(
    Vector3f           &pos,
    std::vector<float> &terrain
    )
{
    Vector3f v0, v1, v2, v3;


    // top
    v0 = Vector3f(pos.x,        pos.y + 1,  pos.z);
    v1 = Vector3f(pos.x + 1,    pos.y + 1,  pos.z);
    v2 = Vector3f(pos.x + 1,    pos.y + 1,  pos.z + 1);
    v3 = Vector3f(pos.x,        pos.y + 1,  pos.z + 1);
    surface_add(v0, v1, v2, v3, terrain);

    // front
    v0 = Vector3f(pos.x,        pos.y + 1,  pos.z + 1);
    v1 = Vector3f(pos.x + 1,    pos.y + 1,  pos.z + 1);
    v2 = Vector3f(pos.x + 1,    pos.y,      pos.z + 1);
    v3 = Vector3f(pos.x,        pos.y,      pos.z + 1);
    surface_add(v0, v1, v2, v3, terrain);

    // right
    v0 = Vector3f(pos.x + 1,    pos.y + 1,  pos.z + 1);
    v1 = Vector3f(pos.x + 1,    pos.y + 1,  pos.z);
    v2 = Vector3f(pos.x + 1,    pos.y,      pos.z);
    v3 = Vector3f(pos.x + 1,    pos.y,      pos.z + 1);
    surface_add(v0, v1, v2, v3, terrain);

    // left
    v0 = Vector3f(pos.x,    pos.y + 1,  pos.z);
    v1 = Vector3f(pos.x,    pos.y + 1,  pos.z + 1);
    v2 = Vector3f(pos.x,    pos.y,      pos.z + 1);
    v3 = Vector3f(pos.x,    pos.y,      pos.z);
//    surface_add(v0, v1, v2, v3, terrain);

}


static void terrain_simplex_init(void)
{
    float density_cube[TERRAIN_WIDTH][TERRAIN_HEIGHT][TERRAIN_DEPTH] = {0};
    float processed_cube[TERRAIN_WIDTH][TERRAIN_HEIGHT][TERRAIN_DEPTH] = {0};
    struct osn_context *ctx;


    // Initialize noise.
//    open_simplex_noise(77374, &ctx);
//
//    for(auto x = 0; x < TERRAIN_WIDTH; x++)
//    {
//        for(auto y = 0; y < TERRAIN_HEIGHT; y++)
//        {
//            for(auto z = 0; z < TERRAIN_HEIGHT; z++)
//            {
//                density_cube[x][y][z] = open_simplex_noise3(ctx, x, y, z);
//            }
//        }
//    }
//
//    for(auto x = 0; x < TERRAIN_WIDTH; x++)
//    {
//        for(auto y = 0; y < TERRAIN_HEIGHT; y++)
//        {
//            for(auto z = 0; z < TERRAIN_HEIGHT; z++)
//            {
//                if(density_cube[x][y][z] > 0)
//                {
//                    processed_cube[x][y][z] = (density_cube[x][y][z] > 0) ? 1 : 0;
//                }
//            }
//        }
//    }

    Vector3f pos(0, 0, 0);

    sTERRIAN_STRUCT terrain;

    cube_add(pos, terrain.vertex);

    f_terrain.push_back(terrain);

    GLuint buffer = vboCreate(f_terrain[0].vertex.size()*4, &f_terrain[0].vertex[0], GL_STATIC_DRAW);
    terrian_vbos.push_back(buffer);
}


static void terrain_init(void)
{
    float height_map[TERRAIN_WIDTH][TERRAIN_HEIGHT] = {0};

    perlinNoiseGenerate(height_map);

    for(auto i = 0; i < TERRAIN_WIDTH; ++i)
    {
        for(auto j = 0; j < TERRAIN_HEIGHT; ++j)
        {
            height_map[i][j] *= 20;
        }
    }

//    LOGE("### perlin noise generate");
//    printMap(height_map);

    sTERRIAN_STRUCT terrain;
//    std::vector<float>  vertex;
//    std::vector<float>  normal;

    for(auto i = 0; i < TERRAIN_WIDTH - 1; ++i)
    {
        for(auto j = 0; j < TERRAIN_HEIGHT - 1; ++j)
        {
            Vector3f v0((float)(i * UNIT_LEN),
                       height_map[i][j],
                       (float)(j * UNIT_LEN));

            Vector3f v1((float)((i+1) * UNIT_LEN),
                       height_map[i+1][j],
                       (float)(j * UNIT_LEN));

            Vector3f v2((float)((i+1) * UNIT_LEN),
                       height_map[i+1][j+1],
                       (float)((j+1) * UNIT_LEN));

            Vector3f v3((float)(i * UNIT_LEN),
                       height_map[i][j+1],
                       (float)((j+1) * UNIT_LEN));

            auto x = v2 - v0;
            auto y = v1 - v0;

            auto crossProduct = x.cross(y);
            auto normal = crossProduct.normalize();

            // 0 2 1    0, 3, 2
            terrain.vertex.push_back(v0[0]);
            terrain.vertex.push_back(v0[1]);
            terrain.vertex.push_back(v0[2]);

            terrain.vertex.push_back(normal.x);
            terrain.vertex.push_back(normal.y);
            terrain.vertex.push_back(normal.z);

            terrain.vertex.push_back(v2[0]);
            terrain.vertex.push_back(v2[1]);
            terrain.vertex.push_back(v2[2]);

            terrain.vertex.push_back(normal.x);
            terrain.vertex.push_back(normal.y);
            terrain.vertex.push_back(normal.z);

            terrain.vertex.push_back(v1[0]);
            terrain.vertex.push_back(v1[1]);
            terrain.vertex.push_back(v1[2]);

            terrain.vertex.push_back(normal.x);
            terrain.vertex.push_back(normal.y);
            terrain.vertex.push_back(normal.z);

//            for(auto k = 0; k < 3; k++)
//            {
//                terrain.vertex.push_back(normal.x);
//                terrain.vertex.push_back(normal.y);
//                terrain.vertex.push_back(normal.z);
//            }

//            LOGE("### normal %f, %f, %f", normal.x, normal.y, normal.z);

            x = v2 - v0;
            y = v1 - v0;

            crossProduct = x.cross(y);
            normal = crossProduct.normalize();

            terrain.vertex.push_back(v0[0]);
            terrain.vertex.push_back(v0[1]);
            terrain.vertex.push_back(v0[2]);

            terrain.vertex.push_back(normal.x);
            terrain.vertex.push_back(normal.y);
            terrain.vertex.push_back(normal.z);

            terrain.vertex.push_back(v3[0]);
            terrain.vertex.push_back(v3[1]);
            terrain.vertex.push_back(v3[2]);

            terrain.vertex.push_back(normal.x);
            terrain.vertex.push_back(normal.y);
            terrain.vertex.push_back(normal.z);

            terrain.vertex.push_back(v2[0]);
            terrain.vertex.push_back(v2[1]);
            terrain.vertex.push_back(v2[2]);

            terrain.vertex.push_back(normal.x);
            terrain.vertex.push_back(normal.y);
            terrain.vertex.push_back(normal.z);

//            auto x = v2 - v0;
//            auto y = v1 - v0;
//
//            auto cross = {x[1]*y[2] - y[1]*x[2],
//                          x[2]*y[0] - y[2]*x[0],
//                          x[0]*y[1] - y[0]*x[1]};
//
//            auto normal = normalize(cross);



//            for(auto k = 0; k < 3; k++)
//            {
//                terrain.vertex.push_back(normal.x);
//                terrain.vertex.push_back(normal.y);
//                terrain.vertex.push_back(normal.z);
//            }
        }
    }

//    LOGE("#### terrian vertex count = %u", terrain.vertex.size());

    f_terrain.push_back(terrain);

    GLuint buffer = vboCreate(f_terrain[0].vertex.size()*4, &f_terrain[0].vertex[0], GL_STATIC_DRAW);
    terrian_vbos.push_back(buffer);
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
    const unsigned int  uiWidth  = constants::WINDOW_WIDTH;
    const unsigned int  uiHeight = constants::WINDOW_HEIGHT;


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
    process_shader(&m_uiVertShader, "C:\\cygwin\\home\\ricky\\dev\\gles_app\\gles_app\\color_shader.vsh", GL_VERTEX_SHADER);
    process_shader(&m_uiFragShader, "C:\\cygwin\\home\\ricky\\dev\\gles_app\\gles_app\\color_shader.fsh", GL_FRAGMENT_SHADER);

    // Create uiProgram (ready to attach shaders)
    texture_program.program = glCreateProgram();

    // Attach shaders and link uiProgram
    glAttachShader(texture_program.program, m_uiVertShader);
    glAttachShader(texture_program.program, m_uiFragShader);
    glLinkProgram(texture_program.program);

    texture_program.a_position = glGetAttribLocation(texture_program.program, "a_Position");
    assert(texture_program.a_position != -1);

    texture_program.a_normal = glGetAttribLocation(texture_program.program, "a_Normal");
    assert(texture_program.a_normal != -1);

    texture_program.u_mvp = glGetUniformLocation(texture_program.program, "u_MvpMatrix");
    assert(texture_program.u_mvp != -1);

    texture_program.u_color = glGetUniformLocation(texture_program.program, "u_Color");
    assert(texture_program.u_color != -1);

    texture_program.u_light = glGetUniformLocation(texture_program.program, "u_VectorToLight");
    assert(texture_program.u_light != -1);

#if 0
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
#endif

//    terrain_init();

//    simplex_test();

    terrain_simplex_init();
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


static void texCoordsPush(
    Renderer::Value   &ref
    )
{
    // Point 1.
    ref.texCoords.push_back(0.0f);
    ref.texCoords.push_back(0.0f);

    // Point 2.
    ref.texCoords.push_back(1.0f);
    ref.texCoords.push_back(0.0f);

    // Point 3.
    ref.texCoords.push_back(1.0f);
    ref.texCoords.push_back(1.0f);

    // Point 4.
    ref.texCoords.push_back(0.0f);
    ref.texCoords.push_back(1.0f);
}


static void verticesPush(
    Renderer::Value    &ref,
    Vector2f           &position,
    Vector2f           &dimension,
    float               orientation
  )
{
    float       co = cosf(orientation);
    float       s = sinf(orientation);
    Matrix2x2f  rotMatrix(Vector2f(co, -s), Vector2f(s, co));
    unsigned    int i;
    unsigned    int curr;


    curr = ref.vertices.size();

    ref.vertices.push_back(position);
    ref.vertices.push_back(Vector2f((float)(position.x + dimension.x), (float) position.y));
    ref.vertices.push_back(position + dimension);
    ref.vertices.push_back(Vector2f((float) position.x, (float)(position.y + dimension.y)));

    for(i = 0; i < 4; i++)
    {
        Vector2f in = ref.vertices[curr + i]
                                - position
                                - dimension/2;

        Vector2f out = in * rotMatrix;

        ref.vertices[curr + i] = out + position;
    }
}


static void colorPush(
    Renderer::Value    &ref,
    Color4f            &color
    )
{
    ref.colors.push_back(color);
    ref.colors.push_back(color);
    ref.colors.push_back(color);
    ref.colors.push_back(color);
}


static void indicesPush(
    Renderer::Value   &ref
    )
{
    size_t curr;


    curr = ref.vertices.size();

    ref.indices.push_back(curr + 0);
    ref.indices.push_back(curr + 2);
    ref.indices.push_back(curr + 1);

    ref.indices.push_back(curr + 0);
    ref.indices.push_back(curr + 3);
    ref.indices.push_back(curr + 2);
}


void Renderer::draw(
    Vector2f   &position,
    Vector2f   &dimension,
    float       orientation,
    Texture    *texture,
    Color4f    &color,
    float       scale
    )
{
    draw(position, dimension * scale, orientation, texture, color);
}


void Renderer::draw(
    Vector2f   &position,
    Vector2f   &dimension,
    float       orientation,
    Texture    *texture,
    Color4f    &color,
    Vector2f    scale
    )
{
    draw(position, dimension * scale, orientation, texture, color);
}


void Renderer::draw(
    Vector2f   &position,
    Vector2f   &dimension,
    float       orientation,
    Texture    *texture,
    Color4f    &color
    )
{
    Value &ref = m_steps[texture];


    // Push indices.
    indicesPush(ref);

    // Push vertices.
    verticesPush(ref, position, dimension, orientation);

    // Push text coordinates.
    texCoordsPush(ref);

    // Push color
    colorPush(ref, color);
}

//struct vec3
//{
//    float x;
//    float y;
//    float z;
//
//    vec3(float x, y, z)
//        : x(x), y(y), z(z)
//    {
//    }
//};

typedef float vec3[3];
typedef float vec4[4];
typedef vec4 mat4x4[4];


static float vec3_mul_inner(
    vec3 a, vec3 b
    )
{
    float p = 0;
    unsigned int i;

    for(i = 0; i < 3; ++i)
    {
        p += b[i] * a[i];
    }

    return p;
}


static inline float vec3_len(
    vec3 v
    )
{
    return sqrtf(vec3_mul_inner(v, v));
}


static inline void mat4x4_translate_in_place(
    mat4x4 m,
    float x,
    float y,
    float z
    )
{
    unsigned int i;


    for(i = 0; i < 4; ++i)
    {
        m[3][i] += m[0][i] * x
                +  m[1][i] * y
                +  m[2][i] * z;
    }
}


static inline void mat4x4_mul(
    mat4x4 M,
    mat4x4 a,
    mat4x4 b
    )
{
    unsigned int r, c, k;
    mat4x4 R;

    for(r = 0; r < 4; r++)
    {
        for(c = 0; c < 4; c++)
        {
            R[c][r] = 0;
            for(k = 0; k < 4; k++)
            {
                R[c][r] += a[k][r] * b[c][k];
            }
        }
    }

    memcpy(M, R, sizeof(R));
}


static void mat4x4_perspective(
    mat4x4  m,
    float   y_fov_in_degrees,
    float   aspect,
    float   n,
    float   f
    )
{
#define M_PI 3.14159f
    const float angle_in_radians = (float)(y_fov_in_degrees * M_PI / 180.0f);
    const float a = (float)(1.0f / tan(angle_in_radians / 2.0f));

    m[0][0] = a / aspect;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = a;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = -((f + n) / (f - n));
    m[2][3] = -1.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = -((2.0f * f * n) / (f - n));
    m[3][3] = 0.0f;
}


static void mat4x4_look_at(
    mat4x4  m,
    float   eyeX,
    float   eyeY,
    float   eyeZ,
    float   centerX,
    float   centerY,
    float   centerZ,
    float   upX,
    float   upY,
    float   upZ
    )
{
    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;

    // Normalize f.
    vec3 f_vec = {fx, fy, fz};
    float rlf = 1.0f / vec3_len(f_vec);
    fx *= rlf;
    fy *= rlf;
    fz *= rlf;

    // compute s = f x up (cross product)
    float sx = fy * upZ - fz * upY;
    float sy = fz * upX - fx * upZ;
    float sz = fx * upY - fy * upX;

    // Normalize s.
    vec3 s_vec = {sx, sy, sz};
    float rls = 1.0f / vec3_len(s_vec);
    sx *= rls;
    sy *= rls;
    sz *= rls;

    // Compute u = s x f
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;

    m[0][0] = sx;
    m[0][1] = ux;
    m[0][2] = -fx;
    m[0][3] = 0.0f;

    m[1][0] = sy;
    m[1][1] = uy;
    m[1][2] = -fy;
    m[1][3] = 0.0f;

    m[2][0] = sz;
    m[2][1] = uz;
    m[2][2] = -fz;
    m[2][3] = 0.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;

    mat4x4_translate_in_place(m, -eyeX, -eyeY, -eyeZ);
}



void terrain_draw(
    const GLuint            buffer,
    const TextureProgram   *texture_program,
    mat4x4                  m
    )
{
    const vec4 colors[] =
    {
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 1.0f, 1.0f}
    };
    static unsigned int colorIndex = 0;

//    if(buffer == 2)
//    {
//        colorIndex = 0;
//    }

    glUseProgram(texture_program->program);
    glUniformMatrix4fv(texture_program->u_mvp, 1, GL_FALSE, (GLfloat *) m);
    glUniform4f(texture_program->u_color, colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2], 1);
    glUniform3f(texture_program->u_light, light.x, light.y, light.z);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(texture_program->a_position,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT)*6,   // For every vertex (float * 3), there's a normal.
                                                // therefore the stride is float * 3 * 2
                          (void *) 0);
    glVertexAttribPointer(texture_program->a_normal,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GL_FLOAT)*6,
                          (void *) (sizeof(GL_FLOAT)*3));
    glEnableVertexAttribArray(texture_program->a_position);
    glEnableVertexAttribArray(texture_program->a_normal);

    glDrawArrays(GL_TRIANGLES, 0, f_terrain[0].vertex.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    colorIndex = (colorIndex+1) % 6;
}


void Renderer::render(
    void
    )
{
    GLint   position, mvp, texCoords, color; 
    mat4x4  projection_matrix;
    mat4x4  view_matrix;
    mat4x4  view_projection_matrix;


    // Render to frame buffer object.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clear framebuffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set active texture unit.
    glActiveTexture(GL_TEXTURE0);

    glViewport(0, 0, constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);

    mat4x4_perspective(projection_matrix,
                       45,
                       constants::WINDOW_WIDTH / constants::WINDOW_HEIGHT,
                       1.0f,
                       1000.0f);

//    Vector2f pos = Player::getInstance()->getPosition();

    mat4x4_look_at(view_matrix,
                   eye.x,
                   eye.y,
                   eye.z,
                   0.0f,
                   0.0f,
                   0.0f,
                   0.0f,
                   1.0f,
                   0.0f);

    mat4x4_mul(view_projection_matrix, projection_matrix, view_matrix);

//    draw_table(&table, &texture_program, view_projection_matrix);

//    for(auto i = 0; i < wallsToPaint; ++i)
//    {
//        wall_draw(wall_vbos[i], &texture_program, view_projection_matrix);
//    }

    terrain_draw(terrian_vbos[0], &texture_program, view_projection_matrix);

    if(!eglSwapBuffers(m_sEGLDisplay, m_sEGLSurface))
    {
        printf("Failed to swap buffers.\n");
    }
}
