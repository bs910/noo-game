///////////////////////////////////////////////////////////////////////////////
/// @file: Model.hpp                                                        ///
/// @brief:                                                                 ///
///                                                                         ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_SCENE_MODEL_HPP_INCLUDED_
#define NOO_SCENE_MODEL_HPP_INCLUDED_


/// Includes
#include "Mesh.hpp"
#include "Material.hpp"
#include "../renderer/Renderer.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace noo {
namespace scene {

class Model
{
public:

    Model()
        : m_VertexBuffer( nullptr )
        , m_IndexBuffer( nullptr )
        , m_Materials()
        , m_Meshes()
        , m_Geometries()
        , m_GeometryGenerated( false )
    { }

    static bool
    createFromFile( std::string const & filename, Model & outModel )
    {
        Assimp::Importer importer;
        aiScene const * ai_scene = importer.ReadFile( filename.c_str(), 0 );

        if ( nullptr == ai_scene )
        {
            /* fail */
            return false;
        }

        std::cout << "Num Materials: " << ai_scene->mNumMaterials << std::endl;
        std::cout << "Num Meshes: " << ai_scene->mNumMeshes << std::endl;

        for ( int mat = 0; mat < ai_scene->mNumMaterials; ++mat )
        {
            aiMaterial * mtl = ai_scene->mMaterials[ mat ];

            aiColor3D diffuseColor{ 1, 0, 1 };
            mtl->Get( AI_MATKEY_COLOR_DIFFUSE, diffuseColor );

            outModel.m_Materials.push_back( { glm::vec3{ diffuseColor.r, diffuseColor.g, diffuseColor.b } } );
        }

        for ( int m = 0; m < ai_scene->mNumMeshes; ++m )
        {
            aiMesh * mesh = ai_scene->mMeshes[ m ];

            outModel.m_Meshes.emplace_back();
            Mesh & outMesh = outModel.m_Meshes.back();

            outMesh.m_Material = &outModel.m_Materials[ mesh->mMaterialIndex ];

            std::cout << "   Mesh " << m << " has " << mesh->mNumVertices << " vertices." << std::endl;

            for ( int v = 0; v < mesh->mNumVertices; ++v )
            {
                aiVector3D p = mesh->mVertices[ v ];
                aiVector3D n = mesh->mNormals[ v ];

                outMesh.VertexPositions.emplace_back( p.x, p.y, p.z );
                outMesh.VertexNormals.emplace_back( n.x, n.y, n.z );
            }

            std::cout << "   Mesh " << m << " has " << mesh->mNumFaces << " faces." << std::endl;

            for ( int f = 0; f < mesh->mNumFaces; ++f )
            {
                aiFace & face = mesh->mFaces[ f ];

                assert( face.mNumIndices == 3 );

                int i0 = face.mIndices[ 0 ];
                int i1 = face.mIndices[ 1 ];
                int i2 = face.mIndices[ 2 ];

                outMesh.FaceIndices.push_back( i0 );
                outMesh.FaceIndices.push_back( i1 );
                outMesh.FaceIndices.push_back( i2 );
            }
        }

        return true;
    }

    void
    draw( renderer::Renderer & renderer, renderer::RenderTarget const & rt, renderer::Shader::Data & shd, renderer::state::StateSet const & state )
    {
        if ( ! m_GeometryGenerated )
        {
            m_VertexBuffer = renderer.createVertexBuffer();
            m_IndexBuffer  = renderer.createIndexBuffer();

            std::vector< renderer::Vertex_Pos3Nrm3 > vposnrm;
            std::vector< uint32_t > vind;

            int offset = 0;
            int baseVertex = 0;

            for ( auto const & m : m_Meshes )
            {
                assert( m.VertexPositions.size() == m.VertexNormals.size() );

                for ( int i = 0; i < m.VertexPositions.size(); ++i )
                {
                    auto const & p = m.VertexPositions[ i ];
                    auto const & n = m.VertexNormals[ i ];

                    vposnrm.push_back( { p.x, p.y, p.z, n.x, n.y, n.z } );
                }

                renderer::Geometry geo;
                geo.Vertices = m_VertexBuffer.get();
                geo.Indices = m_IndexBuffer.get();
                geo.NumPrimitives = m.FaceIndices.size() / 3;
                geo.VertexFormat = renderer::Vertex_Pos3Nrm3::VertexDesc();
                geo.Offset = offset;
                geo.BaseVertex = baseVertex;

                vind.insert( vind.end(), m.FaceIndices.begin(), m.FaceIndices.end() );

                offset += m.FaceIndices.size();
                baseVertex += m.VertexPositions.size();

                m_Geometries.push_back( geo );
                m_MaterialList.push_back( m.m_Material );
            }

            m_VertexBuffer->upload( renderer::Vertex_Pos3Nrm3::SizeInBytes * vposnrm.size(), vposnrm.data() );
            m_IndexBuffer->upload( sizeof( uint32_t ) * vind.size(), vind.data() );

            m_GeometryGenerated = true;
        }

        for ( int g = 0; g < m_Geometries.size(); ++g )
        {
            shd[ "u_color" ] = m_MaterialList[ g ]->Color;

            renderer.draw( rt, shd, state, m_Geometries[ g ] );
        }
    }

private:

    std::unique_ptr< renderer::VertexBuffer > m_VertexBuffer;
    std::unique_ptr< renderer::IndexBuffer > m_IndexBuffer;

    std::vector< Material > m_Materials;
    std::vector< Mesh > m_Meshes;

    std::vector< renderer::Geometry > m_Geometries;
    std::vector< Material * > m_MaterialList;

    bool m_GeometryGenerated;
};

} // - namespace scene
} // - namespace noo


#endif /* NOO_SCENE_MODEL_HPP_INCLUDED_ */
