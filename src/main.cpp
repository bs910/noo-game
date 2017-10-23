///////////////////
/// Entry Point ///
///////////////////


#include "scene/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/VertexTypes.hpp"
#include "logging/Logger.hpp"
#include "common/Utils.hpp"
#include "geometry/GeometryUtils.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <algorithm>
#include <map>

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

using noolog = noo::logging::Logger;


class InputHandler
{
public:

    enum class MouseButton
    {
        LEFT,
        MIDDLE,
        RIGHT
    };

    enum class MouseAction
    {
        PRESS,
        RELEASE
    };

    enum class Key
    {
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_W
    };

    enum class KeyAction
    {
        PRESS,
        RELEASE
    };

    class Listener
    {
    public:
        virtual ~Listener() { }

        virtual void onMouseMove( double /* pos_x */, double /* pos_y */ ) { }
        virtual void onMouseWheel( double /* delta_x */, double /* delta_y */ ) { }
        virtual void onMouseButton( MouseButton /* button */, MouseAction /* action */ ) { }
        virtual void onKeyEvent( Key /* key */, KeyAction /* action */ ) { }
    };

    void
    addListener( Listener * l )
    {
        m_Listeners.push_back( l );
    }

    static std::string
    to_string( MouseButton b )
    {
        switch ( b )
        {
            case MouseButton::LEFT  : return "LEFT";
            case MouseButton::MIDDLE: return "MIDDLE";
            case MouseButton::RIGHT : return "RIGHT";
        }
    }

    static std::string
    to_string( MouseAction a )
    {
        switch ( a )
        {
            case MouseAction::PRESS  : return "PRESS";
            case MouseAction::RELEASE: return "RELEASE";
        }
    }

    static std::string
    to_string( Key k )
    {
        switch ( k )
        {
            case Key::KEY_0: return "KEY_0";
            case Key::KEY_1: return "KEY_1";
            case Key::KEY_2: return "KEY_2";
            case Key::KEY_3: return "KEY_3";
            case Key::KEY_4: return "KEY_4";
            case Key::KEY_5: return "KEY_5";
            case Key::KEY_6: return "KEY_6";
            case Key::KEY_7: return "KEY_7";
            case Key::KEY_8: return "KEY_8";
            case Key::KEY_9: return "KEY_9";
            case Key::KEY_W: return "KEY_W";
        }
    }

    static std::string
    to_string( KeyAction a )
    {
        switch ( a )
        {
            case KeyAction::PRESS  : return "PRESS";
            case KeyAction::RELEASE: return "RELEASE";
        }
    }

    void
    onMouseMove( double xpos, double ypos )
    {
        //noolog::info( "Mouse moved to " + std::to_string( xpos ) + " " + std::to_string( ypos ) + "." );

        for ( auto * l : m_Listeners )
        {
            l->onMouseMove( xpos, ypos );
        }
    }

    void
    onMouseWheel( double x_offset, double y_offset )
    {
        //noolog::info( "Mouse wheel x: " + std::to_string( x_offset ) + " y: " + std::to_string( y_offset ) );

        for ( auto * l : m_Listeners )
        {
            l->onMouseWheel( x_offset, y_offset );
        }
    }

    void
    onMouseButton( MouseButton button, MouseAction action )
    {
        //noolog::info( "Mouse button " + to_string( button ) + " was " + to_string( action ) + "ed." );

        for ( auto * l : m_Listeners )
        {
            l->onMouseButton( button, action );
        }
    }

    void
    onKeyEvent( Key key, KeyAction action )
    {
        //noolog::info( "Key " + to_string( key ) + " was " + to_string( action ) + "ed." );

        for ( auto * l : m_Listeners )
        {
            l->onKeyEvent( key, action );
        }
    }

private:

    std::vector< Listener * > m_Listeners;
};


class CameraOrbitController : public InputHandler::Listener
{
public:

    CameraOrbitController( noo::scene::Camera & cam )
        : m_Camera( cam )
    { }

    void
    onMouseMove( double x, double y ) override
    {
        float dx = m_LastX - (float)x;
        float dy = m_LastY - (float)y;

        if ( m_LeftMousePressed )
        {
            glm::vec3 dir = m_Camera.getDirection();
            glm::vec3 r = m_Camera.getRight();
            glm::vec3 u = m_Camera.getUp();
            glm::vec3 p = m_Camera.getPosition();
            float dist = m_Camera.getEyeDistance();

            glm::vec3 focus_point = p + dir * dist;

            p -= r * ( dx * 0.01f );
            p += u * ( dy * 0.01f );

            float newDist = glm::distance( focus_point, p );
            p -= ( newDist - dist ) * dir;

            m_Camera.setPosition( p );
            m_Camera.lookAt( focus_point );
        }

        m_LastX = x;
        m_LastY = y;
    }

    void
    onMouseWheel( double /* dx */, double dy ) override
    {
        glm::vec3 p = m_Camera.getPosition();
        glm::vec3 dir = m_Camera.getDirection();
        float dist = m_Camera.getEyeDistance();

        glm::vec3 fp = p + dir * dist;

        p += dir * (float)dy * 0.025f;

        m_Camera.setPosition( p );
        m_Camera.lookAt( fp );
    }

    void
    onMouseButton( InputHandler::MouseButton b, InputHandler::MouseAction a ) override
    {
        if ( b == InputHandler::MouseButton::LEFT )
        {
            if ( a == InputHandler::MouseAction::PRESS )
                m_LeftMousePressed = true;

            if ( a == InputHandler::MouseAction::RELEASE )
                m_LeftMousePressed = false;
        }
    }

private:

    noo::scene::Camera & m_Camera;

    bool m_LeftMousePressed;

    float m_LastX, m_LastY;
};


class RenderModeSwitch : public InputHandler::Listener
{
public:

    void
    onKeyEvent( InputHandler::Key key, InputHandler::KeyAction action ) override
    {
        if ( key == InputHandler::Key::KEY_1 && action == InputHandler::KeyAction::PRESS )
            State = 1;

        if ( key == InputHandler::Key::KEY_2 && action == InputHandler::KeyAction::PRESS )
            State = 2;

        if ( key == InputHandler::Key::KEY_3 && action == InputHandler::KeyAction::PRESS )
            State = 3;

        if ( key == InputHandler::Key::KEY_4 && action == InputHandler::KeyAction::PRESS )
            State = 4;

        if ( key == InputHandler::Key::KEY_W && action == InputHandler::KeyAction::PRESS )
            Wireframe = !Wireframe;
    }

    int State = 1;
    bool Wireframe = false;
};


/// @brief Key input handler callback procedure.
static void keyCallback( GLFWwindow * window, int key, int /* scancode */, int action, int /* mods */ )
{
    InputHandler & input = *static_cast< InputHandler * >( glfwGetWindowUserPointer( window ) );

    static std::map< int, InputHandler::Key > glfw2nooKey = { { GLFW_KEY_0, InputHandler::Key::KEY_0 }
                                                            , { GLFW_KEY_1, InputHandler::Key::KEY_1 }
                                                            , { GLFW_KEY_2, InputHandler::Key::KEY_2 }
                                                            , { GLFW_KEY_3, InputHandler::Key::KEY_3 }
                                                            , { GLFW_KEY_4, InputHandler::Key::KEY_4 }
                                                            , { GLFW_KEY_5, InputHandler::Key::KEY_5 }
                                                            , { GLFW_KEY_6, InputHandler::Key::KEY_6 }
                                                            , { GLFW_KEY_7, InputHandler::Key::KEY_7 }
                                                            , { GLFW_KEY_8, InputHandler::Key::KEY_8 }
                                                            , { GLFW_KEY_9, InputHandler::Key::KEY_9 }
                                                            , { GLFW_KEY_W, InputHandler::Key::KEY_W } };

    static std::map< int, InputHandler::KeyAction > glfw2nooAction = { { GLFW_PRESS  , InputHandler::KeyAction::PRESS }
                                                                     , { GLFW_RELEASE, InputHandler::KeyAction::RELEASE } };

    switch( key )
    {
        case GLFW_KEY_ESCAPE:
            if ( GLFW_PRESS == action ) { glfwSetWindowShouldClose( window, 1 ); }
        break;
    }

    input.onKeyEvent( glfw2nooKey[ key ], glfw2nooAction[ action ] );
}

/// @brief Mouse move handler.
static void cursorPositionCallback( GLFWwindow * window, double xpos, double ypos )
{
    InputHandler & input = *static_cast< InputHandler * >( glfwGetWindowUserPointer( window ) );
    input.onMouseMove( xpos, ypos );
}

///
static void mouseButtonCallback( GLFWwindow * window, int button, int action, int /* mods */ )
{
    InputHandler & input = *static_cast< InputHandler * >( glfwGetWindowUserPointer( window ) );

    using ib = InputHandler::MouseButton;
    using ia = InputHandler::MouseAction;

    static std::map< int, ib > glfw2noo_button = { { GLFW_MOUSE_BUTTON_LEFT  , ib::LEFT }
                                                 , { GLFW_MOUSE_BUTTON_MIDDLE, ib::MIDDLE }
                                                 , { GLFW_MOUSE_BUTTON_RIGHT , ib::RIGHT } };

    static std::map< int, ia > glfw2noo_action = { { GLFW_PRESS  , ia::PRESS }
                                                 , { GLFW_RELEASE, ia::RELEASE } };

    input.onMouseButton( glfw2noo_button[ button ], glfw2noo_action[ action ] );
}

///
static void scrollCallback( GLFWwindow * window, double xoffset, double yoffset )
{
    InputHandler & input = *static_cast< InputHandler * >( glfwGetWindowUserPointer( window ) );

    input.onMouseWheel( xoffset, yoffset );
}


/// @brief Application entry point.
int main( int /* argc */, char ** /* argv */ )
{
    noolog::info( "Hello Nation of One!" );

    if ( ! glfwInit() )
    {
        return -1;
    }

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
    //glfwWindowHint( GLFW_SAMPLES, 16 ); // enable multi-sampling

    glm::ivec2 const window_size( 800, 600 );
    GLFWwindow * window = glfwCreateWindow( window_size.x, window_size.y, "Nation of One", nullptr, nullptr );

    InputHandler inputHandler;
    glfwSetWindowUserPointer( window, &inputHandler );

    if ( ! window )
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback( window, keyCallback );
    glfwSetCursorPosCallback( window, cursorPositionCallback );
    glfwSetMouseButtonCallback( window, mouseButtonCallback );
    glfwSetScrollCallback( window, scrollCallback );

    glfwMakeContextCurrent( window );

    gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) );

    noolog::debug( glfwGetVersionString() );
    noolog::debug( std::string( reinterpret_cast< char const * >( glGetString( GL_VERSION ) ) ) );

    glm::vec4 const clrColor{ 0.39, 0.58, 0.92, 1.0 };

    noo::renderer::Renderer renderer;
    renderer.initialize( window_size.x, window_size.y );

    using noo::renderer::ETextureFormat;
    using noo::renderer::EImageFormat;
    using noo::renderer::EImagePixelType;

    int const rt_width  = 400;
    int const rt_height = 300;

    auto rt = renderer.createRenderTarget( rt_width, rt_height );
    auto rt_color = renderer.createTexture2D( rt_width, rt_height, ETextureFormat::RGB, nullptr, EImageFormat::RGB, EImagePixelType::UBYTE );
    auto rt_depth = renderer.createTexture2D( rt_width, rt_height, ETextureFormat::DEPTH_24_STENCIL_8, nullptr, EImageFormat::DEPTH_24_STENCIL_8, EImagePixelType::UINT_24_8 );

    rt->attachTexture2D( noo::renderer::EAttachmentUsage::COLOR_ATTACHMENT0, *rt_color );
    rt->attachTexture2D( noo::renderer::EAttachmentUsage::DEPTH_STENCIL_ATTACHMENT, *rt_depth );

    //auto rt_rb = renderer.createRenderBuffer( rt_width, rt_height, noo::renderer::RenderBuffer::DEPTH_24_STENCIL_8 );
    //rt->attachRenderbuffer( noo::renderer::RenderTarget::DEPTH_STENCIL_ATTACHMENT, *rt_rb );


    /* Deferred Rendering G-Buffer */
    auto rt_def = renderer.createRenderTarget( rt_width, rt_height );
    auto rt_def_diffuse  = renderer.createTexture2D( rt_width, rt_height, ETextureFormat::RGB, nullptr, EImageFormat::RGB, EImagePixelType::UBYTE );
    auto rt_def_position = renderer.createTexture2D( rt_width, rt_height, ETextureFormat::RGB_32F, nullptr, EImageFormat::RGB, EImagePixelType::FLOAT );
    auto rt_def_normal   = renderer.createTexture2D( rt_width, rt_height, ETextureFormat::RGB_32F, nullptr, EImageFormat::RGB, EImagePixelType::FLOAT );
    //auto rt_def_texcoord = renderer.createTexture2D( rt_width, rt_height, ETextureFormat::RGB );
    auto rt_def_depth = renderer.createRenderBuffer( rt_width, rt_height, noo::renderer::RenderBuffer::Format::DEPTH_24_STENCIL_8 );

    rt_def->attachTexture2D( noo::renderer::EAttachmentUsage::COLOR_ATTACHMENT0, *rt_def_diffuse );
    rt_def->attachTexture2D( noo::renderer::EAttachmentUsage::COLOR_ATTACHMENT1, *rt_def_position );
    rt_def->attachTexture2D( noo::renderer::EAttachmentUsage::COLOR_ATTACHMENT2, *rt_def_normal );
    //rt_def->attachTexture2D( noo::renderer::EAttachmentUsage::COLOR_ATTACHMENT3, *rt_def_texcoord );
    rt_def->attachRenderbuffer( noo::renderer::EAttachmentUsage::DEPTH_STENCIL_ATTACHMENT, *rt_def_depth );

    std::string const def_pre_VS = noo::common::readFile( "resources/shaders/deferred_pre.vsh" );
    std::string const def_pre_FS = noo::common::readFile( "resources/shaders/deferred_pre.fsh" );

    auto shader_def_pre = renderer.createShader( def_pre_VS.c_str(), nullptr, nullptr, nullptr, def_pre_FS.c_str() );

    noo::renderer::Shader::Data shdDefPre( *shader_def_pre );




    std::vector< noo::renderer::Vertex_Pos3Color4 > vData =
    {
        { -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
    };

    auto vbo_tri = renderer.createVertexBuffer();
    vbo_tri->upload( vData.size() * noo::renderer::Vertex_Pos3Color4::SizeInBytes, vData.data() );

    std::vector< noo::renderer::Vertex_Pos3Tex2 > vQuad =
    {
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
        {  1.0f,  1.0f, 0.0f, 1.0f, 1.0f },
        { -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },

        {  1.0f,  1.0f, 0.0f, 1.0f, 1.0f },
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
    };

    auto vbo_quad = renderer.createVertexBuffer();
    vbo_quad->upload( vQuad.size() * noo::renderer::Vertex_Pos3Tex2::SizeInBytes, vQuad.data() );

    std::string const solidVS = noo::common::readFile( "resources/shaders/simple.vsh" );
    std::string const solidFS = noo::common::readFile( "resources/shaders/simple.fsh" );

    auto shaderSolid = renderer.createShader( solidVS.c_str(), nullptr, nullptr, nullptr, solidFS.c_str() );

    std::string const texVS = noo::common::readFile( "resources/shaders/textured.vsh" );
    std::string const texFS = noo::common::readFile( "resources/shaders/textured.fsh" );

    auto shaderTex = renderer.createShader( texVS.c_str(), nullptr, nullptr, nullptr, texFS.c_str() );

    auto const diffLitVS = noo::common::readFile( "resources/shaders/diffuse.vsh" );
    auto const diffLitFS = noo::common::readFile( "resources/shaders/diffuse.fsh" );

    auto shaderLit = renderer.createShader( diffLitVS.c_str(), nullptr, nullptr, nullptr, diffLitFS.c_str() );

    int const tex_size = 32;

    std::array< unsigned char, tex_size * tex_size * 4 > imgData;

    for ( int x = 0; x < tex_size; ++x )
        for ( int y = 0; y < tex_size; ++y )
        {
            imgData[ 4*y * tex_size + x*4 + 0 ] = ( ( x + y ) % 2 ) * 255;
            imgData[ 4*y * tex_size + x*4 + 1 ] = ( ( x + y ) % 2 ) * 255;
            imgData[ 4*y * tex_size + x*4 + 2 ] = ( ( x + y ) % 2 ) * 255;
            imgData[ 4*y * tex_size + x*4 + 3 ] = 255;
        }

    auto tex = renderer.createTexture2D( tex_size, tex_size, ETextureFormat::RGBA, imgData.data(), EImageFormat::RGBA, EImagePixelType::UBYTE );


    noo::scene::Camera cam;
    cam.setAspectRatio( 800.0f / 600.0f );
    cam.setPosition( 0, 0, 3 );
    cam.lookAt( 0, 0, 0 );

    CameraOrbitController camControl( cam );
    inputHandler.addListener( &camControl );

    RenderModeSwitch rms;
    inputHandler.addListener( &rms );

    noo::renderer::Shader::Data shdSolid( *shaderSolid );
    noo::renderer::Shader::Data shdTex( *shaderTex );
    noo::renderer::Shader::Data shdLit( *shaderLit );

    noo::renderer::Geometry geoTri;
    geoTri.Vertices = vbo_tri.get();
    geoTri.Indices = nullptr;
    geoTri.NumPrimitives = vData.size() / 3;
    geoTri.VertexFormat = noo::renderer::Vertex_Pos3Color4::VertexDesc();

    noo::renderer::Geometry geoQuad;
    geoQuad.Vertices = vbo_quad.get();
    geoQuad.Indices = nullptr;
    geoQuad.NumPrimitives = vQuad.size() / 3;
    geoQuad.VertexFormat = noo::renderer::Vertex_Pos3Tex2::VertexDesc();


    std::vector< glm::vec3 > sphere_pos;
    std::vector< uint32_t > sphere_idx;
    noo::geometry::GeometryUtils::createSphere( 32, 16, sphere_pos, sphere_idx );

    std::vector< noo::renderer::Vertex_Pos3Nrm3 > vSphere;

    for ( auto const & v : sphere_pos )
    {
        glm::vec3 n = glm::normalize( v );
        vSphere.push_back( { v.x, v.y, v.z, n.x, n.y, n.z } );
    }

    auto vbo_sphere = renderer.createVertexBuffer();
    vbo_sphere->upload( vSphere.size() * noo::renderer::Vertex_Pos3Nrm3::SizeInBytes, vSphere.data() );

    auto ibo_sphere = renderer.createIndexBuffer();
    ibo_sphere->upload( sphere_idx.size() * sizeof( uint32_t ), sphere_idx.data() );

    noo::renderer::Geometry geoSphere;
    geoSphere.Vertices = vbo_sphere.get();
    geoSphere.Indices = ibo_sphere.get();
    geoSphere.NumPrimitives = sphere_idx.size() / 3;
    geoSphere.VertexFormat = noo::renderer::Vertex_Pos3Nrm3::VertexDesc();


    std::string const meshFilename = "/home/ben/torus.obj";
    std::vector< glm::vec3 > meshPos;
    std::vector< glm::vec3 > meshNrm;
    std::vector< uint32_t > meshInd;
    glm::vec3 meshMtl{ 0, 0, 0 };
    bool mesh_loaded = noo::geometry::GeometryUtils::loadMeshFromFile( meshFilename, meshPos, meshNrm, meshInd, meshMtl );

    if ( mesh_loaded )
    {
        noolog::info( "Mesh " + meshFilename + " successfully loaded." );
    }
    else
    {
        noolog::error( "Mesh " + meshFilename + " couldn't be loaded." );
    }

    std::vector< noo::renderer::Vertex_Pos3Nrm3 > vMesh;

    for ( int i = 0; i < meshPos.size(); ++i )
    {
        glm::vec3 p = meshPos[ i ];
        glm::vec3 n = meshNrm[ i ];

        vMesh.push_back( { p.x, p.y, p.z, n.x, n.y, n.z } );
    }

    auto vbo_mesh = renderer.createVertexBuffer();
    vbo_mesh->upload( vMesh.size() * noo::renderer::Vertex_Pos3Nrm3::SizeInBytes, vMesh.data() );

    auto ibo_mesh = renderer.createIndexBuffer();
    ibo_mesh->upload( meshInd.size() * sizeof( uint32_t ), meshInd.data() );

    noo::renderer::Geometry geoMesh;
    geoMesh.Vertices = vbo_mesh.get();
    geoMesh.Indices = ibo_mesh.get();
    geoMesh.NumPrimitives = meshInd.size() / 3;
    geoMesh.VertexFormat = noo::renderer::Vertex_Pos3Nrm3::VertexDesc();

    using noo::renderer::EWrapMode;
    using noo::renderer::EMinFilterMode;
    using noo::renderer::EMagFilterMode;
    using noo::renderer::state::StateSet;

    int frame = 0;
    int border = 15;

    while ( ! glfwWindowShouldClose( window ) )
    {

        // pre-pass - render to texture
        {

            if ( rms.State == 4 && mesh_loaded )
            {
                renderer.clear( *rt_def, { 0, 0, 0, 0 }, 1.0f, 0 );

                StateSet stateSet;

                shdDefPre[ "u_mvp" ] = cam.getViewProjectionMatrix();
                shdDefPre[ "u_mat_rot" ] = glm::mat3( cam.getViewMatrix() );
                shdDefPre[ "u_color" ] = meshMtl;

                renderer.draw( *rt_def, shdDefPre, stateSet, geoMesh );


                renderer.clear( *rt, clrColor, 1.0f, 0 );

                shdLit[ "u_mvp" ] = cam.getViewProjectionMatrix();
                shdLit[ "u_color" ] = meshMtl;
                shdLit[ "u_light_dir" ] = glm::normalize( glm::vec3( 1, 1, 1 ) );

                renderer.draw( *rt, shdLit, stateSet, geoMesh );
            }
            else
            {
                renderer.clear( *rt, clrColor, 1.0f, 0 );
                {
                    // render a lit sphere
                    StateSet stateSet;
                    stateSet.depth = noo::renderer::state::DepthState::WriteOnly();
                    stateSet.cull.FrontFaceWinding = noo::renderer::state::EFrontFaceWinding::CW;
                    if ( rms.Wireframe ) stateSet.rasterizer = noo::renderer::state::RasterizerState::Wireframe();

                    shdLit[ "u_mvp" ] = cam.getViewProjectionMatrix();
                    shdLit[ "u_color" ] = glm::vec3( 0.5, 0.5, 1.0 );
                    shdLit[ "u_light_dir" ] = glm::normalize( glm::vec3( 1, 1, 1 ) );

                    renderer.draw( *rt, shdLit, stateSet, geoSphere );
                }

                {
                    // render a colorful triangle
                    StateSet stateSet;
                    stateSet.depth = noo::renderer::state::DepthState::WriteOnly();
                    stateSet.cull = noo::renderer::state::CullState::Disabled();
                    if ( rms.Wireframe ) stateSet.rasterizer = noo::renderer::state::RasterizerState::Wireframe();

                    shdSolid[ "u_mvp" ] = cam.getViewProjectionMatrix();
                    shdSolid[ "u_color" ] = glm::vec4( 1, 1, 1, 1 );

                    renderer.draw( *rt, shdSolid, stateSet, geoTri );
                }
            }
        }

        // render textured quad to screen
        {
            StateSet stateSet;

            renderer.clear( renderer.defaultRenderTarget(), glm::vec4( 0, 1, 0, 1 ) );
            {
                shdTex[ "u_mvp" ] = glm::mat4();

                if ( rms.State == 1 )
                {
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ rt_color.get(), EWrapMode::REPEAT, EWrapMode::REPEAT, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };
                    renderer.draw( renderer.defaultRenderTarget(), shdTex, stateSet, geoQuad );
                }
                else if ( rms.State == 2 )
                {
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ rt_depth.get(), EWrapMode::REPEAT, EWrapMode::REPEAT, EMinFilterMode::LINEAR, EMagFilterMode::LINEAR };
                    renderer.draw( renderer.defaultRenderTarget(), shdTex, stateSet, geoQuad );
                }
                else if ( rms.State == 3 )
                {
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ tex.get(), EWrapMode::CLAMP, EWrapMode::MIRROR, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };
                    renderer.draw( renderer.defaultRenderTarget(), shdTex, stateSet, geoQuad );
                }
                else
                {
                    int w = renderer.defaultRenderTarget().getWidth();
                    int h = renderer.defaultRenderTarget().getHeight();

                    stateSet.cull = noo::renderer::state::CullState::Disabled();

                    // show the g-buffer textures
                    stateSet.viewport = noo::renderer::state::ViewportState( 0, 0, w/2, h/2 );
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ rt_def_diffuse.get(), EWrapMode::CLAMP, EWrapMode::CLAMP, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };
                    renderer.draw( renderer.defaultRenderTarget(), shdTex, stateSet, geoQuad );

                    stateSet.viewport = noo::renderer::state::ViewportState( w/2, 0, w/2, h/2 );
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ rt_def_position.get(), EWrapMode::CLAMP, EWrapMode::CLAMP, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };
                    renderer.draw( renderer.defaultRenderTarget(), shdTex, stateSet, geoQuad );

                    stateSet.viewport = noo::renderer::state::ViewportState( 0, h/2, w/2, h/2 );
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ rt_def_normal.get(), EWrapMode::CLAMP, EWrapMode::CLAMP, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };
                    renderer.draw( renderer.defaultRenderTarget(), shdTex, stateSet, geoQuad );

                    stateSet.viewport = noo::renderer::state::ViewportState( w/2, h/2, w/2, h/2 );
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ rt_color.get(), EWrapMode::CLAMP, EWrapMode::CLAMP, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };
                    renderer.draw( renderer.defaultRenderTarget(), shdTex, stateSet, geoQuad );
                }
            }
        }

        glfwSwapBuffers( window );
        glfwPollEvents();

        ++frame;
    }

    renderer.destroy();

    glfwTerminate();

    noolog::info( "Goodbye, Nation of One!" );

    return 0;
}
