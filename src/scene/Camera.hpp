///////////////////////////////////////////////////////////////////////////////
/// @file: Camera.hpp                                                       ///
/// @brief: Defines a camera in cartesian space. Positive X points to the   ///
///         right, Y upwards and Z towards the viewer.                      ///
///                                                                         ///
///         Y                                                               ///
///         |                                                               ///
///         +---X                                                           ///
///        /                                                                ///
///       /                                                                 ///
///      Z                                                                  ///
///                                                                         ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_SCENE_CAMERA_HPP_INCLUDED_
#define NOO_SCENE_CAMERA_HPP_INCLUDED_


/// Forward declarations


/// Includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

/// Using declarations



namespace noo {
namespace scene {

class Camera
{
public:

    Camera()
        : m_Position( 0, 0, 0 )
        , m_Direction( 0, 0, -1 )
        , m_Up( 0, 1, 0 )
        , m_FOVY( 45.0f )
        , m_AspectRatio( 1.0f )
        , m_NearDistance( 0.01f )
        , m_FarDistance( 100.0f )
    { }

    Camera( glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float fovy, float aspect, float zNear, float zFar )
        : m_Position( pos )
        , m_Direction( dir )
        , m_Up( up )
        , m_FOVY( fovy )
        , m_AspectRatio( aspect )
        , m_NearDistance( zNear )
        , m_FarDistance( zFar )
    { }


    glm::mat4
    getViewMatrix() const
        { return glm::lookAtRH( m_Position, m_Position + m_Direction, m_Up ); }


    glm::mat4
    getProjectionMatrix() const
        { return glm::perspectiveRH( m_FOVY, m_AspectRatio, m_NearDistance, m_FarDistance ); }


    glm::mat4
    getViewProjectionMatrix() const
        { return getProjectionMatrix() * getViewMatrix(); }


    glm::vec3
    getPosition() const
        { return m_Position; }

    void
    setPosition( glm::vec3 const & pos )
        { m_Position = pos; }

    void
    setPosition( float x, float y, float z )
        { m_Position = glm::vec3( x, y, z ); }


    glm::vec3
    getDirection() const
        { return m_Direction; }

    void
    setDirection( glm::vec3 const & dir )
        { m_Direction = dir; }


    glm::vec3
    getUp() const
        { return m_Up; }

    void
    setUp( glm::vec3 const & up )
        { m_Up = up; }


    float
    getFovy() const
        { return m_FOVY; }

    void
    setFovy( float fovy )
        { m_FOVY = fovy; }


    float
    getAspectRatio() const
        { return m_AspectRatio; }

    void
    setAspectRatio( float aspect )
        { m_AspectRatio = aspect; }


    float
    getNearDistance() const
        { return m_NearDistance; }

    void
    setNearDistance( float near )
        { m_NearDistance = near; }


    float
    getFarDistance() const
        { return m_FarDistance; }

    void
    setFarDistance( float far )
        { m_FarDistance = far; }


    void
    lookAt( glm::vec3 const & focusPoint )
    {
        m_Direction = glm::normalize( focusPoint - m_Position );
        glm::vec3 right = glm::cross( m_Direction, m_Up );
        m_Up = glm::cross( right, m_Direction );
    }

private:

    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    glm::vec3 m_Up;

    float m_FOVY;
    float m_AspectRatio;
    float m_NearDistance;
    float m_FarDistance;
};

} // - namespace scene
} // - namespace noo


#endif /* NOO_SCENE_CAMERA_HPP_INCLUDED_ */
