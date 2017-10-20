///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_RENDERBUFFER_HPP_INCLUDED
#define NOO_RENDERER_RENDERBUFFER_HPP_INCLUDED


/// Forward declarations


/// Includes
#include "glad/glad.h"

/// Using declarations



namespace noo {
namespace renderer {

class RenderBuffer
{
    friend class Renderer;

public:

    enum Format
    {
        DEPTH_16,
        DEPTH_24,
        STENCIL_8,
        DEPTH_24_STENCIL_8,
        COLOR_RGBA8888
    };

    ~RenderBuffer()
    {
        glDeleteRenderbuffers( 1, &m_GLHandle );
    }

    GLuint
    getHandle() const
    { return m_GLHandle; }

protected:

    RenderBuffer( uint32_t width, uint32_t height, Format format )
        : m_GLHandle( 0 )
    {
        glGenRenderbuffers( 1, &m_GLHandle );
        glBindRenderbuffer( GL_RENDERBUFFER, m_GLHandle );
        glRenderbufferStorage( GL_RENDERBUFFER, toGLFormat( format ), width, height );
        glBindRenderbuffer( GL_RENDERBUFFER, 0 );
    }

private:

    GLenum
    toGLFormat( Format f )
    {
        switch ( f )
        {
            case DEPTH_16: return GL_DEPTH_COMPONENT16;
            case DEPTH_24: return GL_DEPTH_COMPONENT24;
            case STENCIL_8: return GL_STENCIL_INDEX8;
            case DEPTH_24_STENCIL_8: return GL_DEPTH24_STENCIL8;
            case COLOR_RGBA8888: return GL_RGBA8;
        }
    }

    GLuint m_GLHandle;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_RENDERBUFFER_HPP_INCLUDED */

