///////////////////////////////////////////////////////////////////////////////
/// @file: Texture2D.hpp                                                    ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_TEXTURE_2D_HPP_INCLUDED
#define NOO_RENDERER_TEXTURE_2D_HPP_INCLUDED


/// Forward declarations


/// Includes
#include <cstdint>
#include "glad/glad.h"

/// Using declarations



namespace noo {
namespace renderer {

class Texture2D
{
    friend class Renderer;

public:

    void
    activate( int textureSlot = 0 )
    {
        glActiveTexture( GL_TEXTURE0 + textureSlot );
        glBindTexture( GL_TEXTURE_2D, m_TextureHandle );
    }

    ~Texture2D()
    {
        glDeleteTextures( 1, &m_TextureHandle );
    }

    uint32_t
    getWidth() const
    { return m_Width; }

    uint32_t
    getHeight() const
    { return m_Height; }

    GLuint
    getGLHandle() const
    { return m_TextureHandle; }

protected:

    Texture2D( uint32_t w, uint32_t h, void const * data )
        : m_Width( w )
        , m_Height( h )
    {
        glGenTextures( 1, &m_TextureHandle );
        glBindTexture( GL_TEXTURE_2D, m_TextureHandle );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

private:

    GLuint m_TextureHandle;
    uint32_t m_Width;
    uint32_t m_Height;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_TEXTURE_2D_HPP_INCLUDED */

