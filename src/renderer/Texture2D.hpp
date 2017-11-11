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

enum class EImageFormat
{
    RGB,
    RGBA,
    DEPTH_24_STENCIL_8
};

enum class EImagePixelType
{
    BYTE,
    UBYTE,
    SHORT,
    USHORT,
    INT,
    UINT,
    FLOAT,
    UINT_24_8
};

enum class ETextureFormat
{
    RGB,
    RGB_16F,
    RGB_32F,
    RGBA,
    RGBA_16F,
    RGBA_32F,
    DEPTH_24_STENCIL_8
};

class Texture2D
{
    friend class Renderer;

public:

    void
    activate( int textureSlot = 0 ) const
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

    Texture2D( uint32_t w, uint32_t h, ETextureFormat texFormat, void const * data, EImageFormat imgFormat, EImagePixelType pixType )
        : m_Width( w )
        , m_Height( h )
    {
        glGenTextures( 1, &m_TextureHandle );
        glBindTexture( GL_TEXTURE_2D, m_TextureHandle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glTexImage2D( GL_TEXTURE_2D, 0, toGLTextureFormat( texFormat ), w, h, 0, toGLImageFormat( imgFormat ), toGLPixelType( pixType ), data );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    static GLint
    toGLTextureFormat( ETextureFormat f )
    {
        switch ( f )
        {
            case ETextureFormat::RGB: return GL_RGB;
            case ETextureFormat::RGB_16F: return GL_RGB16F;
            case ETextureFormat::RGB_32F: return GL_RGB32F;
            case ETextureFormat::RGBA: return GL_RGBA;
            case ETextureFormat::RGBA_16F: return GL_RGBA16F;
            case ETextureFormat::RGBA_32F: return GL_RGBA32F;
            case ETextureFormat::DEPTH_24_STENCIL_8: return GL_DEPTH24_STENCIL8;
        }
    }

    static GLenum
    toGLImageFormat( EImageFormat f )
    {
        switch ( f )
        {
            case EImageFormat::RGB: return GL_RGB;
            case EImageFormat::RGBA: return GL_RGBA;
            case EImageFormat::DEPTH_24_STENCIL_8: return GL_DEPTH_STENCIL;
        }
    }

    static GLenum
    toGLPixelType( EImagePixelType f )
    {
        switch ( f )
        {
            case EImagePixelType::BYTE: return GL_BYTE;
            case EImagePixelType::UBYTE: return GL_UNSIGNED_BYTE;
            case EImagePixelType::SHORT: return GL_SHORT;
            case EImagePixelType::USHORT: return GL_UNSIGNED_SHORT;
            case EImagePixelType::INT: return GL_INT;
            case EImagePixelType::UINT: return GL_UNSIGNED_INT;
            case EImagePixelType::FLOAT: return GL_FLOAT;
            case EImagePixelType::UINT_24_8: return GL_UNSIGNED_INT_24_8;
        }
    }

private:

    GLuint m_TextureHandle;
    uint32_t m_Width;
    uint32_t m_Height;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_TEXTURE_2D_HPP_INCLUDED */

