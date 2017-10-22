///////////////////////////////////////////////////////////////////////////////
/// @file: VertexTypes.hpp                                                  ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_VERTEXTYPES_HPP_INCLUDED
#define NOO_RENDERER_VERTEXTYPES_HPP_INCLUDED

/// Forward declarations

/// Includes
#include <type_traits>
#include <cstdint>
#include <vector>
#include "glad/glad.h"

/// Using declarations


namespace noo {
namespace renderer {

enum class EVertexComponentType
{
    INT,
    INT_2,
    INT_3,
    INT_4,
    FLOAT,
    FLOAT_2,
    FLOAT_3,
    FLOAT_4,
};

inline int
vcSize( EVertexComponentType t )
{
    switch ( t )
    {
        case EVertexComponentType::INT  : return 1;
        case EVertexComponentType::INT_2: return 2;
        case EVertexComponentType::INT_3: return 3;
        case EVertexComponentType::INT_4: return 4;

        case EVertexComponentType::FLOAT  : return 1;
        case EVertexComponentType::FLOAT_2: return 2;
        case EVertexComponentType::FLOAT_3: return 3;
        case EVertexComponentType::FLOAT_4: return 4;
    }
}

inline GLenum
toGLType( EVertexComponentType t )
{
    switch ( t )
    {
        case EVertexComponentType::INT  : return GL_UNSIGNED_INT;
        case EVertexComponentType::INT_2: return GL_UNSIGNED_INT;
        case EVertexComponentType::INT_3: return GL_UNSIGNED_INT;
        case EVertexComponentType::INT_4: return GL_UNSIGNED_INT;

        case EVertexComponentType::FLOAT  : return GL_FLOAT;
        case EVertexComponentType::FLOAT_2: return GL_FLOAT;
        case EVertexComponentType::FLOAT_3: return GL_FLOAT;
        case EVertexComponentType::FLOAT_4: return GL_FLOAT;
    }
}

struct VertexComponent
{
    EVertexComponentType Type;
    int Offset; // in bytes
};

struct VertexDescription
{
    std::vector< VertexComponent > Components;
    int Stride;
};

struct Vertex_Pos3
{
    float x, y, z;

    static constexpr int SizeInBytes = sizeof( float ) * 3;

    static VertexDescription const &
    VertexDesc()
    {
        static VertexDescription vd{ { { EVertexComponentType::FLOAT_3, 0 } }
                                   , 0 };
        return vd;
    }
};


struct Vertex_Pos3Color4
{
    float x, y, z;
    float r, g, b, a;

    static constexpr int SizeInBytes = sizeof( float ) * ( 3 + 4 );

    static VertexDescription const &
    VertexDesc()
    {
        static VertexDescription vd{ { { EVertexComponentType::FLOAT_3, 0 }
                                     , { EVertexComponentType::FLOAT_4, 3 * sizeof( float ) } }
                                   , SizeInBytes };
        return vd;
    }
};


struct Vertex_Pos3Tex2
{
    float x, y, z;
    float u, v;

    static constexpr int SizeInBytes = sizeof( float ) * ( 3 + 2 );

    static VertexDescription const &
    VertexDesc()
    {
        static VertexDescription vd{ { { EVertexComponentType::FLOAT_3, 0 }
                                     , { EVertexComponentType::FLOAT_2, 3 * sizeof( float ) } }
                                   , SizeInBytes };
        return vd;
    }
};


struct Vertex_Pos3Nrm3
{
    float x, y, z;
    float nx, ny, nz;

    static constexpr int SizeInBytes = sizeof( float ) * ( 3 + 3 );

    static VertexDescription const &
    VertexDesc()
    {
        static VertexDescription vd{ { { EVertexComponentType::FLOAT_3, 0 }
                                     , { EVertexComponentType::FLOAT_3, 3 * sizeof( float ) } }
                                   , SizeInBytes };

        return vd;
    }
};


} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_VERTEXTYPES_HPP_INCLUDED */


    /*Mesh( std::vector< VertexType > const & vertexData, std::vector< IndexType > const & indexData )
    {
        static_assert( ! std::is_void< IndexType >::value, "Mesh is not indexed!" );

        glGenVertexArrays( 1, &m_VertexArrayObjectHandle );
        glBindVertexArray( m_VertexArrayObjectHandle );

        glGenBuffers( 1, &m_VertexBufferHandle );
        glBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferHandle );
        glBufferData( GL_ARRAY_BUFFER, vertexData.size() * VertexType::SizeInBytes, vertexData.data(), GL_STATIC_DRAW );

        glGenBuffers( 1, &m_IndexBufferHandle );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferHandle );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof( IndexType ), indexData.data(), GL_STATIC_DRAW );

        VertexType::BindVertexAttributes();

        glBindVertexArray( 0 );
    }

    void
    draw()
    {
        glBindVertexArray( m_VertexArrayObjectHandle );

        if ( isIndexed() )
        {
            glDrawElements( GL_TRIANGLES, m_NumPrimitives, toGLIndexType< IndexType >(), NULL );
        }
        else
        {
            glDrawArrays( GL_TRIANGLES, 0, m_NumPrimitives * 3 );
        }

        glBindVertexArray( 0 );
    }*/


