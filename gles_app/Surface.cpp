#include "IL/il.h"
#include "Surface.h"


//Surface& Surface::operator =(const Surface& origSurface)
//{
//    if(this != &origSurface)
//    {
//        delete [] m_ptr;
//
//        m_size = origSurface.m_size;
//        m_bytesPerRow = origSurface.m_bytesPerRow;
//
//        m_ptr = new char [origSurface.m_size.height * m_bytesPerRow];
//        assert(m_ptr);
//
//        memcpy(m_ptr,
//               origSuface.m_ptr,
//               origSurface.m_size.height * m_bytesPerRow);
//    }
//
//    return *this;
//}

bool Surface::m_initialized = false;


Surface::Surface(
    const std::string &filename
    )
    : m_size(0, 0), m_ptr(NULL)
{
//    char           *location,
//    unsigned int   *width,
//    unsigned int   *height,
//    char          **pixels

    std::string path        = "C:/cygwin/home/ricky/dev/gles_app/gles_app/";
    std::string location    = path + filename;
    ILuint      texid;


    if(!m_initialized)
    {
        // Initialize IL.
        ilInit();

        m_initialized = true;
    }

    ilGenImages(1, &texid);
    ilBindImage(texid);
	ilLoadImage((const wchar_t *) location.c_str());

    m_size.width  = (float) ilGetInteger(IL_IMAGE_WIDTH);
    m_size.height = (float) ilGetInteger(IL_IMAGE_HEIGHT);
    m_bytesPerRow = (short) (m_size.width * 4);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    // Create new buffer.
    m_ptr = new char [(unsigned int) m_size.height * m_bytesPerRow];
    assert(m_ptr != NULL);

    // Hold data.
    memcpy(m_ptr,
           (void *) ilGetData(),
           m_size.height * m_bytesPerRow);
}


Surface::~Surface()
{
    if(m_ptr)
    {
        delete [] m_ptr;
    }
}


Dimension2f Surface::getSize() const
{
    return m_size;
}
