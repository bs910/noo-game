///////////////////////////////////////////////////////////////////////////////
/// @file: VertexBuffer.hpp                                                 ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_VERTEXBUFFER_HPP_INCLUDED
#define NOO_RENDERER_VERTEXBUFFER_HPP_INCLUDED

#include <vector>
#include "glad/glad.h"


namespace noo {
namespace renderer {


class VertexBuffer
{
    friend class Renderer;

public:

    ~VertexBuffer()
    {
        glDeleteBuffers( 1, &m_VboHandle );
    }

    void
    activate()
    { glBindBuffer( GL_ARRAY_BUFFER, m_VboHandle ); }

    void
    deactivate()
    { glBindBuffer( GL_ARRAY_BUFFER, 0 ); }

    void
    upload( size_t numBytes, void * data )
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_VboHandle );
        glBufferData( GL_ARRAY_BUFFER, numBytes, data, GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    GLuint
    getHandle() const
    { return m_VboHandle; }

protected:

    VertexBuffer()
        : m_VboHandle( 0 )
    {
        glGenBuffers( 1, &m_VboHandle );
    }

private:

    GLuint m_VboHandle;
};


} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_VERTEXBUFFER_HPP_INCLUDED */

