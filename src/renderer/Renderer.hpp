#ifndef NOO_RENDERER_RENDERER_HPP_INCLUDED
#define NOO_RENDERER_RENDERER_HPP_INCLUDED

#include "glm/glm.hpp"
#include <memory>

#include "Shader.hpp"
#include "states/StateSet.hpp"
#include "Texture2D.hpp"
#include "RenderBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "RenderTarget.hpp"
#include "Geometry.hpp"


namespace noo {
namespace renderer {


class Renderer
{
public:

    void
    initialize( int width, int height );

    void
    destroy();

    /// @brief Clear the color, depth and stencil buffer.
    /// @param clearColor The color to clear the color buffer with.
    void
    clear( RenderTarget const & rt, glm::vec4 const & clearColor, float clearDepth = 1.0f, int clearStencil = 0 );


    /// @brief Create a new shader from the given source. Vertex and fragment stages are mandatory.
    ///        Tesselation control/evaluation and geometry stages are optional.
    /// @param vertexSource The source code for the vertex shader stage.
    /// @param tessCtrlSource The source code for the tesselation control shader stage.
    /// @param tessEvalSource The source code for the tesselation evaluation shader stage.
    /// @param geometrySource The source code for the geometry shader stage.
    /// @param fragmentSource The source code for the fragment shader stage.
    /// @return A pointer to the newly created shader object.
    std::shared_ptr< Shader >
    createShader( char const * vertexSource
                , char const * tessCtrlSource
                , char const * tessEvalSource
                , char const * geometrySource
                , char const * fragmentSource )
    {
        return std::shared_ptr< Shader >( new Shader( vertexSource
                                                    , tessCtrlSource
                                                    , tessEvalSource
                                                    , geometrySource
                                                    , fragmentSource ) );
    }

    std::unique_ptr< VertexBuffer >
    createVertexBuffer()
    {
        return std::unique_ptr< VertexBuffer >( new VertexBuffer );
    }

    std::unique_ptr< IndexBuffer >
    createIndexBuffer()
    {
        return std::unique_ptr< IndexBuffer >( new IndexBuffer );
    }

    std::unique_ptr< Texture2D >
    createTexture2D( uint32_t w, uint32_t h, ETextureFormat format, void const * data, EImageFormat imgFormat, EImagePixelType pixType )
    {
        return std::unique_ptr< Texture2D >( new Texture2D( w, h, format, data, imgFormat, pixType ) );
    }

    std::unique_ptr< RenderTarget >
    createRenderTarget( int width, int height )
    {
        return std::unique_ptr< RenderTarget >( new RenderTarget( width, height ) );
    }

    RenderTarget const &
    defaultRenderTarget() const
    {
        return *m_DefaultRenderTarget;
    }

    std::unique_ptr< RenderBuffer >
    createRenderBuffer( uint32_t width, uint32_t height, RenderBuffer::Format format )
    {
        return std::unique_ptr< RenderBuffer >( new RenderBuffer( width, height, format ) );
    }

    void
    draw( RenderTarget const & rt, Shader::Data & shd, state::StateSet & state, Geometry & geo )
    {
        // apply states
        // blend
        if ( state.blend.Enabled == state::EBlendEnable::ENABLE )
        {
            glEnable( GL_BLEND );
            glBlendFunc( toGLBlendFunc( state.blend.SourceBlendFunc ), toGLBlendFunc( state.blend.DestBlendFunc ) );
            glBlendEquation( toGLBlendEq( state.blend.BlendEq ) );
        }
        else
        {
            glDisable( GL_BLEND );
        }

        // cull
        if ( state.cull.CullMode == state::ECullMode::NONE )
        {
            glDisable( GL_CULL_FACE );
        }
        else
        {
            glEnable( GL_CULL_FACE );
            glCullFace( toGLCullMode( state.cull.CullMode ) );
            glFrontFace( toGLFrontFace( state.cull.FrontFaceWinding ) );
        }

        // depth
        if ( state.depth.EnableDepthTesting == state::EEnableDepthTest::ENABLE )
        {
            glEnable( GL_DEPTH_TEST );
            glDepthMask( state.depth.EnableDepthWriting == state::EEnableDepthWrite::ENABLE ? GL_TRUE : GL_FALSE );
            glDepthFunc( toGLDepthFunc( state.depth.CompareFunc ) );
        }
        else
        {
            glDisable( GL_DEPTH_TEST );
        }

        // stencil
        // ... TODO

        // rasterizer
        if ( state.rasterizer.FillMode == state::EPolygonFillMode::LINE )
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glLineWidth( state.rasterizer.LineWidth );
        }
        else
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        // viewport
        if ( state.viewport.isDefault() )
        {
            glViewport( 0, 0, rt.getWidth(), rt.getHeight() );
        }
        else
        {
            glViewport( state.viewport.X, state.viewport.Y, state.viewport.Width, state.viewport.Height );
        }

        rt.activate();

        shd.getShader().activate();

        // TODO: texture unit manager ?
        int currentTexSlot = 0;

        for ( auto const & u : shd.getUniformData() )
        {
            if ( ! u.isTextureSampler() )
            {
                u.apply();
            }
            else
            {
                TextureSampler ts = *reinterpret_cast< TextureSampler* >( u.Data );

                ts.Texture->activate( currentTexSlot );
                ++currentTexSlot;

                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLWrapMode( ts.WrapS ) );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLWrapMode( ts.WrapT ) );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLMinFilter( ts.MinFilter ) );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLMagFilter( ts.MagFilter ) );

                // Just for testing BORDER wrap mode
                /*GLfloat b[] = { 0.0f, 1.0f, 0.0f, 1.0f };
                glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, b );*/
            }
        }

        // TODO: expand to multiple vertex streams
        geo.Vertices->activate();

        for ( int i = 0; i < geo.VertexFormat.Components.size(); ++i )
        {
            VertexComponent & v = geo.VertexFormat.Components[ i ];

            glEnableVertexAttribArray( i );
            glVertexAttribPointer( i, vcSize( v.Type ), toGLType( v.Type ), GL_FALSE, geo.VertexFormat.Stride, (GLvoid*)( v.Offset ) );
        }

        if ( geo.IsIndexed() )
        {
            geo.Indices->activate();
            glDrawElements( GL_TRIANGLES, geo.NumPrimitives * 3, GL_UNSIGNED_INT, NULL );
        }
        else
        {
            glDrawArrays( GL_TRIANGLES, 0, geo.NumPrimitives * 3 );
        }
    }

private:

    static GLenum
    toGLBlendFunc( state::EBlendFunc b )
    {
        switch ( b )
        {
            case state::EBlendFunc::ZERO: return GL_ZERO;
            case state::EBlendFunc::ONE: return GL_ONE;
            case state::EBlendFunc::SRC_COLOR: return GL_SRC_COLOR;
            case state::EBlendFunc::DST_COLOR: return GL_DST_COLOR;
            case state::EBlendFunc::ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
            case state::EBlendFunc::ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
            case state::EBlendFunc::SRC_ALPHA: return GL_SRC_ALPHA;
            case state::EBlendFunc::DST_ALPHA: return GL_DST_ALPHA;
            case state::EBlendFunc::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
            case state::EBlendFunc::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
        }
    }

    static GLenum
    toGLBlendEq( state::EBlendEquation e )
    {
        switch ( e )
        {
            case state::EBlendEquation::ADD: return GL_FUNC_ADD;
            case state::EBlendEquation::SUBTRACT: return GL_FUNC_SUBTRACT;
            case state::EBlendEquation::REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
            case state::EBlendEquation::MIN: return GL_MIN;
            case state::EBlendEquation::MAX: return GL_MAX;
        }
    }

    static GLenum
    toGLCullMode( state::ECullMode c )
    {
        switch ( c )
        {
            case state::ECullMode::FRONT: return GL_FRONT;
            case state::ECullMode::BACK: return GL_BACK;
            case state::ECullMode::BOTH: return GL_FRONT_AND_BACK;

            // should be handled earlier
            case state::ECullMode::NONE: return GL_BACK;
        }
    }

    static GLenum
    toGLFrontFace( state::EFrontFaceWinding f )
    {
        switch ( f )
        {
            case state::EFrontFaceWinding::CW: return GL_CW;
            case state::EFrontFaceWinding::CCW: return GL_CCW;
        }
    }

    static GLenum
    toGLDepthFunc( state::EDepthFunc f )
    {
        switch ( f )
        {
            case state::EDepthFunc::ALWAYS: return GL_ALWAYS;
            case state::EDepthFunc::NEVER: return GL_NEVER;
            case state::EDepthFunc::LESS: return GL_LESS;
            case state::EDepthFunc::EQUAL: return GL_EQUAL;
            case state::EDepthFunc::LESS_EQUAL: return GL_LEQUAL;
            case state::EDepthFunc::GREATER: return GL_GREATER;
            case state::EDepthFunc::NOT_EQUAL: return GL_NOTEQUAL;
            case state::EDepthFunc::GREATER_EQUAL: return GL_GEQUAL;
        }
    }

private:

    /// @brief The window surface back buffer.
    std::shared_ptr< RenderTarget > m_DefaultRenderTarget;

    /// @brief In OpenGL 4 core profile a vertex array object has to be used.
    GLuint m_DefaultVAO;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_RENDERER_HPP_INCLUDED */
