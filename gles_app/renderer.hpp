#include <vector>

#include "GLES2/gl2.h"
#include "EGL/egl.h"

typedef struct
{
    std::vector<float>          texCoords;
    std::vector<unsigned short> indices;
    std::vector<float>          vertices;
    std::vector<float>          colors;

} sEntityAttributes;


class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    void entityAdd();
    void entityRemove();

    void render();
    void draw();

private:
    EGLDisplay  m_sEGLDisplay;
    EGLContext  m_sEGLContext;
    EGLSurface  m_sEGLSurface;
    HWND        m_hWindow;
    HDC         m_hDisplay;
    GLuint      m_uiProgram, m_uiFragShader, m_uiVertShader;
    GLuint      m_textureId;

    std::vector<sEntityAttributes>  m_entityAttributes;
};
