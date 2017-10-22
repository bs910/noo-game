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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// Using declarations



namespace noo {
namespace geometry {

class GeometryUtils
{
public:


    static bool
    loadMeshFromFile( std::string const & filename
                    , std::vector< glm::vec3 > & outPositions
                    , std::vector< glm::vec3 > & outNormals
                    , std::vector< uint32_t >  & outIndices )
    {
        Assimp::Importer importer;
        aiScene const * ai_scene = importer.ReadFile( filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals );

        if ( nullptr == ai_scene )
        {
            /* fail */
            return false;
        }

        for ( int m = 0; m < ai_scene->mNumMeshes; ++m )
        {
            aiMesh * mesh = ai_scene->mMeshes[ m ];

            for ( int v = 0; v < mesh->mNumVertices; ++v )
            {
                aiVector3D p = mesh->mVertices[ v ];
                aiVector3D n = mesh->mNormals[ v ];

                outPositions.emplace_back( p.x, p.y, p.z );
                outNormals.emplace_back( n.x, n.y, n.z );
            }

            for ( int f = 0; f < mesh->mNumFaces; ++f )
            {
                aiFace & face = mesh->mFaces[ f ];

                assert( face.mNumIndices == 3 );

                int i0 = face.mIndices[ 0 ];
                int i1 = face.mIndices[ 1 ];
                int i2 = face.mIndices[ 2 ];

                outIndices.push_back( i0 );
                outIndices.push_back( i1 );
                outIndices.push_back( i2 );
            }
        }

        return true;
    }

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

        for ( size_t stack = 0; stack <= numStacks; ++stack )
        {
            for ( size_t slice = 0; slice < numSlices; ++slice )
            {
                float x = std::cos( deltaTheta * slice ) * std::sin( deltaPhi * stack );
                float y = std::sin( deltaTheta * slice ) * std::sin( deltaPhi * stack );
                float z = std::cos( deltaPhi * stack );

                vertexPositions.emplace_back( glm::vec3{ x, y, z } );
            }
        }

        for ( size_t st = 0; st < numStacks; ++st )
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

