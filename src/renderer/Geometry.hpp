///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_GEOMETRY_HPP_INCLUDED
#define NOO_RENDERER_GEOMETRY_HPP_INCLUDED


/// Forward declarations


/// Includes
#include "VertexBuffer.hpp"
#include "VertexTypes.hpp"
#include "IndexBuffer.hpp"


/// Using declarations


namespace noo {
namespace renderer {

struct Geometry
{
    bool IsIndexed() const
    { return Indices != nullptr; }

    VertexBuffer * Vertices;
    int NumPrimitives;
    VertexDescription VertexFormat;

    IndexBuffer * Indices;
};

} // - namespace renderer
} // - namespace noo

#endif /* NOO_RENDERER_GEOMETRY_HPP_INCLUDED */

