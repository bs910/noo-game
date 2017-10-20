#ifndef NOO_RENDERER_RENDERER_HPP_INCLUDED
#define NOO_RENDERER_RENDERER_HPP_INCLUDED

#include "glm/glm.hpp"
#include <memory>

#include "Shader.hpp"
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
    initialize();

    void
    destroy();

    /// @brief Clear the color, depth and stencil buffer.
    /// @param clearColor The color to clear the color buffer with.
    void
    clear( RenderTarget & rt, glm::vec4 const & clearColor, float clearDepth = 1.0f, int clearStencil = 0 );


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
    createTexture2D( uint32_t w, uint32_t h, void const * data = nullptr )
    {
        return std::unique_ptr< Texture2D >( new Texture2D( w, h, data ) );
    }

    std::unique_ptr< RenderTarget >
    createRenderTarget()
    {
        return std::unique_ptr< RenderTarget >( new RenderTarget );
    }

    std::unique_ptr< RenderBuffer >
    createRenderBuffer( uint32_t width, uint32_t height, RenderBuffer::Format format )
    {
        return std::unique_ptr< RenderBuffer >( new RenderBuffer( width, height, format ) );
    }

    void
    draw( RenderTarget & rt, Shader::Data & shd, Geometry & geo )
    {
        rt.activate();

        shd.getShader().activate();

        int currentTexSlot = 0;

        for ( auto const & u : shd.getUniformData() )
        {
            if ( ! u.isTextureSampler() )
                u.apply();
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
            glDrawElements( GL_TRIANGLES, geo.NumPrimitives, GL_UNSIGNED_INT, NULL );
        }
        else
        {
            glDrawArrays( GL_TRIANGLES, 0, geo.NumPrimitives * 3 );
        }
    }
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_RENDERER_HPP_INCLUDED */
