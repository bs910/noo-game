///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_INDEXBUFFER_HPP_INCLUDED
#define NOO_RENDERER_INDEXBUFFER_HPP_INCLUDED


/// Forward declarations


/// Includes
#include "glad/glad.h"
#include <vector>

/// Using declarations



namespace noo {
namespace renderer {

class IndexBuffer
{
    friend class Renderer;

public:

    ~IndexBuffer()
    {
        glDeleteBuffers( 1, &m_GLHandle );
    }

    void
    activate()
    { glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_GLHandle ); }

    void
    deactivate()
    { glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); }

    void
    upload( uint32_t numBytes, void * data )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_GLHandle );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, numBytes, data, GL_STATIC_DRAW );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

protected:

    IndexBuffer()
        : m_GLHandle( 0 )
    {
        glGenBuffers( 1, &m_GLHandle );
    }

private:

    GLuint m_GLHandle;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_INDEXBUFFER_HPP_INCLUDED */

