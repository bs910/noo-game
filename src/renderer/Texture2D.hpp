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

enum class ETextureFormat
{
    RGB,
    RGBA,
    DEPTH_24_STENCIL_8
};

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

    Texture2D( uint32_t w, uint32_t h, ETextureFormat format, void const * data )
        : m_Width( w )
        , m_Height( h )
        , Format( format )
    {
        glGenTextures( 1, &m_TextureHandle );
        glBindTexture( GL_TEXTURE_2D, m_TextureHandle );
        glTexImage2D( GL_TEXTURE_2D, 0, toGLInternalFormat( format ), w, h, 0, toGLFormat( format ), toGLType( format ), data );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    static GLint
    toGLInternalFormat( ETextureFormat f )
    {
        switch ( f )
        {
            case ETextureFormat::RGB: return GL_RGB;
            case ETextureFormat::RGBA: return GL_RGBA;
            case ETextureFormat::DEPTH_24_STENCIL_8: return GL_DEPTH24_STENCIL8;
        }
    }

    static GLenum
    toGLFormat( ETextureFormat f )
    {
        switch ( f )
        {
            case ETextureFormat::RGB: return GL_RGB;
            case ETextureFormat::RGBA: return GL_RGBA;
            case ETextureFormat::DEPTH_24_STENCIL_8: return GL_DEPTH_STENCIL;
        }
    }

    static GLenum
    toGLType( ETextureFormat f )
    {
        switch ( f )
        {
            case ETextureFormat::RGB:
            case ETextureFormat::RGBA: return GL_UNSIGNED_BYTE;
            case ETextureFormat::DEPTH_24_STENCIL_8: return GL_UNSIGNED_INT_24_8;
        }
    }

private:

    GLuint m_TextureHandle;
    uint32_t m_Width;
    uint32_t m_Height;
    ETextureFormat Format;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_TEXTURE_2D_HPP_INCLUDED */

