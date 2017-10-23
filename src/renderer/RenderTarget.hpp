///////////////////////////////////////////////////////////////////////////////
/// @file: RenderTarget.hpp                                                 ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_RENDERTARGET_HPP_INCLUDED
#define NOO_RENDERER_RENDERTARGET_HPP_INCLUDED


/// Forward declarations


/// Includes
#include <cassert>
#include <cstdint>
#include <array>
#include <vector>

#include "glad/glad.h"

#include "../logging/Logger.hpp"
#include "../common/Utils.hpp"
#include "Texture2D.hpp"
#include "RenderBuffer.hpp"

/// Using declarations
using noolog = noo::logging::Logger;


namespace noo {
namespace renderer {

enum class EAttachmentUsage : GLenum
{
    COLOR_ATTACHMENT0,
    COLOR_ATTACHMENT1,
    COLOR_ATTACHMENT2,
    COLOR_ATTACHMENT3,
    DEPTH_ATTACHMENT,
    STENCIL_ATTACHMENT,
    DEPTH_STENCIL_ATTACHMENT,

    ENUM_END
};


class RenderTarget
{
    friend class Renderer;

public:

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

        std::vector< GLenum > bufs;

        for ( int i = 0; i < m_IsAttachmentPresent.size(); ++i )
        {
            if ( m_IsAttachmentPresent[ i ] )
            {
                EAttachmentUsage a = EAttachmentUsage( i );

                if ( isColorAttachment( a ) )
                {
                    GLenum gle = toGLAttachmentType( a );
                    bufs.push_back( gle );
                }
            }
        }

        if ( !bufs.empty() ) glDrawBuffers( bufs.size(), bufs.data() );
    }

    void
    deactivate() const
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void
    attachTexture2D( EAttachmentUsage usage, Texture2D & tex )
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_FBOHandle );
        glFramebufferTexture2D( GL_FRAMEBUFFER, toGLAttachmentType( usage ), GL_TEXTURE_2D, tex.getGLHandle(), 0 );

        checkStatus();

        noolog::debug( "Attached texture successfully." );

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        m_IsAttachmentPresent[ noo::common::enum_index( usage ) ] = true;
    }

    void
    attachRenderbuffer( EAttachmentUsage usage, RenderBuffer & rb )
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_FBOHandle );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, toGLAttachmentType( usage ), GL_RENDERBUFFER, rb.getHandle() );

        checkStatus();

        noolog::debug( "Attached render buffer successfully." );

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        m_IsAttachmentPresent[ noo::common::enum_index( usage ) ] = true;
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
    toGLAttachmentType( EAttachmentUsage a ) const
    {
        switch ( a )
        {
            case EAttachmentUsage::COLOR_ATTACHMENT0 : return GL_COLOR_ATTACHMENT0;
            case EAttachmentUsage::COLOR_ATTACHMENT1 : return GL_COLOR_ATTACHMENT1;
            case EAttachmentUsage::COLOR_ATTACHMENT2 : return GL_COLOR_ATTACHMENT2;
            case EAttachmentUsage::COLOR_ATTACHMENT3 : return GL_COLOR_ATTACHMENT3;
            case EAttachmentUsage::DEPTH_ATTACHMENT  : return GL_DEPTH_ATTACHMENT;
            case EAttachmentUsage::STENCIL_ATTACHMENT: return GL_STENCIL_ATTACHMENT;
            case EAttachmentUsage::DEPTH_STENCIL_ATTACHMENT: return GL_DEPTH_STENCIL_ATTACHMENT;
            default: assert( false );
        }
    }

    bool
    isColorAttachment( EAttachmentUsage a ) const
    {
        switch ( a )
        {
            case EAttachmentUsage::COLOR_ATTACHMENT0 : return true;
            case EAttachmentUsage::COLOR_ATTACHMENT1 : return true;
            case EAttachmentUsage::COLOR_ATTACHMENT2 : return true;
            case EAttachmentUsage::COLOR_ATTACHMENT3 : return true;
            case EAttachmentUsage::DEPTH_ATTACHMENT  : return false;
            case EAttachmentUsage::STENCIL_ATTACHMENT: return false;
            case EAttachmentUsage::DEPTH_STENCIL_ATTACHMENT: return false;
            default: assert( false );
        }
    }

    void
    checkStatus() const
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

    std::array< bool, noo::common::enum_count< EAttachmentUsage >() > m_IsAttachmentPresent = { { false } };
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_RENDERTARGET_HPP_INCLUDED */

