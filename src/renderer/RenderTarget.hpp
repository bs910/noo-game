///////////////////////////////////////////////////////////////////////////////
/// @file: RenderTarget.hpp                                                 ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_RENDERTARGET_HPP_INCLUDED
#define NOO_RENDERER_RENDERTARGET_HPP_INCLUDED


/// Forward declarations


/// Includes
#include "../logging/Logger.hpp"
#include "glad/glad.h"
#include <cassert>
#include <cstdint>
#include "Texture2D.hpp"
#include "RenderBuffer.hpp"

/// Using declarations
using noolog = noo::logging::Logger;


namespace noo {
namespace renderer {

class RenderTarget
{
    friend class Renderer;

public:

    enum AttachmentUsage
    {
        COLOR_ATTACHMENT0,
        COLOR_ATTACHMENT1,
        COLOR_ATTACHMENT2,
        COLOR_ATTACHMENT3,
        DEPTH_ATTACHMENT,
        STENCIL_ATTACHMENT,
        DEPTH_STENCIL_ATTACHMENT
    };

    int
    getWidth() const
    { return m_Width; }

    int
    getHeight() const
    { return m_Height; }

    void
    activate() const
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_FBOHandle );
    }

    void
    deactivate() const
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void
    attachTexture2D( AttachmentUsage usage, Texture2D & tex )
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_FBOHandle );
        glFramebufferTexture2D( GL_FRAMEBUFFER, toGLAttachmentType( usage ), GL_TEXTURE_2D, tex.getGLHandle(), 0 );

        checkStatus();

        noolog::debug( "Attached texture successfully." );

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void
    attachRenderbuffer( AttachmentUsage usage, RenderBuffer & rb )
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_FBOHandle );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, toGLAttachmentType( usage ), GL_RENDERBUFFER, rb.getHandle() );

        checkStatus();

        noolog::debug( "Attached render buffer successfully." );

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    ~RenderTarget()
    {
        glDeleteFramebuffers( 1, &m_FBOHandle );
    }

protected:

    RenderTarget( int width, int height )
        : m_FBOHandle( 0 )
        , m_Width( width )
        , m_Height( height )
    {
        glGenFramebuffers( 1, &m_FBOHandle );
    }

    RenderTarget( int width, int height, int handle )
        : m_FBOHandle( handle )
        , m_Width( width )
        , m_Height( height )
    { }

private:

    GLenum
    toGLAttachmentType( AttachmentUsage a )
    {
        switch ( a )
        {
            case COLOR_ATTACHMENT0 : return GL_COLOR_ATTACHMENT0;
            case COLOR_ATTACHMENT1 : return GL_COLOR_ATTACHMENT1;
            case COLOR_ATTACHMENT2 : return GL_COLOR_ATTACHMENT2;
            case COLOR_ATTACHMENT3 : return GL_COLOR_ATTACHMENT3;
            case DEPTH_ATTACHMENT  : return GL_DEPTH_ATTACHMENT;
            case STENCIL_ATTACHMENT: return GL_STENCIL_ATTACHMENT;
            case DEPTH_STENCIL_ATTACHMENT: return GL_DEPTH_STENCIL_ATTACHMENT;
        }
    }

    void
    checkStatus()
    {
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
        {
            noolog::error( "Framebuffer incomplete!" );
            assert( false );
        }
    }

    GLuint m_FBOHandle;
    int m_Width;
    int m_Height;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_RENDERTARGET_HPP_INCLUDED */

