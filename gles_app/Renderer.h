#pragma once

#include <vector>
#include "assert.h"
#include "Vector.h"
#include "Singleton.h"
#include "Texture.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"


typedef struct
{
    GLuint                      texture;
    std::vector<float>          texCoords;
    std::vector<unsigned short> indices;
    std::vector<Vector2f>       vertices;
    std::vector<Color4f>        colors;

} sEntityAttributes;


class Renderer 
	: public Singleton<Renderer>
{
public:
    Renderer();
    virtual ~Renderer();

    void entityAdd();
    void entityRemove();

    void render();
    void draw(
        Vector2f   &position,
        Vector2f   &dimension,
        float       orientation,
        Texture    *texture,
        Vector4f   &color
    );

    void Renderer::draw(
        Vector2f   &position,
        Vector2f   &dimension,
        float       orientation,
        Texture    *texture,
        Color4f    &color,
        float       scale
    );

private:
    EGLDisplay  m_sEGLDisplay;
    EGLContext  m_sEGLContext;
    EGLSurface  m_sEGLSurface;
    HWND        m_hWindow;
    HDC         m_hDisplay;
    GLuint      m_uiProgram, m_uiFragShader, m_uiVertShader;
    GLuint      m_textureId;

    std::vector<sEntityAttributes>  m_entityAttributes;

	friend class Singleton<Renderer>; 

	friend class Input;
};
