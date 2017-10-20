///////////////////////////////////////////////////////////////////////////////
/// @file: GeometryUtils.hpp                                                ///
/// @brief: Can generate simple meshes like spheres and cylinders.          ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_GEOMETRY_GEOMETRYUTILS_HPP__INCLUDED__
#define NOO_GEOMETRY_GEOMETRYUTILS_HPP__INCLUDED__


/// Forward declarations


/// Includes
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

/// Using declarations



namespace noo {
namespace geometry {

class GeometryUtils
{
public:

    /*static void
    createSphere( size_t numStacks, size_t numSlices
                , std::vector< glm::vec3 > & vertexPositions
                , std::vector< int > & indices
                , std::vector< glm::vec3 > & vertexNormals );


    static void
    createSphere( size_t numStacks, size_t numSlices
                , std::vector< glm::vec3 > & vertexPositions
                , std::vector< int > & indices
                , std::vector< glm::vec3 > & vertexNormals
                , std::vector< glm::vec2 > & vertexTexCoords );*/


    // Implementation
    static void
    createSphere( size_t numStacks, size_t numSlices
                , std::vector< glm::vec3 > & vertexPositions
                , std::vector< uint32_t > & indices )
    {
        float deltaTheta = glm::pi< float >() * 2.0f / static_cast< float >( numSlices );
        float deltaPhi   = glm::pi< float >() / static_cast< float >( numStacks );

        for ( size_t stack = 0; stack < numStacks; ++stack )
        {
            for ( size_t slice = 0; slice < numSlices; ++slice )
            {
                float x = cos( deltaTheta * slice ) * sin( deltaPhi * stack );
                float y = sin( deltaTheta * slice ) * sin( deltaPhi * stack );
                float z = cos( deltaPhi * stack );

                vertexPositions.emplace_back( glm::vec3{ x, y, z } );
            }
        }

        for ( size_t st = 0; st < numStacks - 1; ++st )
        {
            for ( size_t sl = 0; sl < numSlices; ++sl )
            {
                indices.push_back( st         * numSlices +   sl );
                indices.push_back( st         * numSlices +   sl + 1 );
                indices.push_back( ( st + 1 ) * numSlices +   sl );

                indices.push_back( ( st + 1 ) * numSlices +   sl );
                indices.push_back(   st       * numSlices +   sl + 1 );
                indices.push_back( ( st + 1 ) * numSlices + ( sl + 1 ) );
            }
        }
    }
};

} // - namespace geometry
} // - namespace noo


#endif /* NOO_GEOMETRY_GEOMETRYUTILS_HPP__INCLUDED__ */

