///////////////////////////////////////////////////////////////////////////////
/// @file: Material.hpp                                                     ///
/// @brief:                                                                 ///
///                                                                         ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_SCENE_MATERIAL_HPP_INCLUDED_
#define NOO_SCENE_MATERIAL_HPP_INCLUDED_


/// Forward declarations


/// Includes
#include "glm/glm.hpp"

/// Using declarations



namespace noo {
namespace scene {

struct Material
{
    glm::vec3 Color;
};

} // - namespace scene
} // - namespace noo


#endif /* NOO_SCENE_MATERIAL_HPP_INCLUDED_ */
