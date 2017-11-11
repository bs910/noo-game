///////////////////////////////////////////////////////////////////////////////
/// @file: Mesh.hpp                                                         ///
/// @brief:                                                                 ///
///                                                                         ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_SCENE_MESH_HPP_INCLUDED_
#define NOO_SCENE_MESH_HPP_INCLUDED_


/// Includes
#include <vector>
#include "glm/glm.hpp"

#include "Material.hpp"


namespace noo {
namespace scene {

struct Mesh
{
    std::vector< glm::vec3 > VertexPositions;
    std::vector< glm::vec3 > VertexNormals;

    std::vector< uint32_t > FaceIndices;

    Material * m_Material;
};

} // - namespace scene
} // - namespace noo


#endif /* NOO_SCENE_MESH_HPP_INCLUDED_ */
