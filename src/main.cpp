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

    void
    onMouseMove( double xpos, double ypos )
    {
        noolog::info( "Mouse moved to " + std::to_string( xpos ) + " " + std::to_string( ypos ) + "." );
    }

    void
    onMouseWheel( double x_offset, double y_offset )
    {
        noolog::info( "Mouse wheel x: " + std::to_string( x_offset ) + " y: " + std::to_string( y_offset ) );
    }

    void
    onMouseButton( MouseButton button, MouseAction action )
    {
        noolog::info( "Mouse button " + to_string( button ) + " was " + to_string( action ) + "ed." );
    }
};


/// @brief Key input handler callback procedure.
static void keyCallback( GLFWwindow * window, int key, int /* scancode */, int action, int /* mods */ )
{
    switch( key )
    {
        case GLFW_KEY_ESCAPE:
            if ( GLFW_PRESS == action ) { glfwSetWindowShouldClose( window, 1 ); }
        break;
    }
}

/// @brief Mouse move handler.
static void cursorPositionCallback( GLFWwindow * window, double xpos, double ypos )
{
//    InputHandler & input = *static_cast< InputHandler * >( glfwGetWindowUserPointer( window ) );
//    input.onMouseMove( xpos, ypos );
}

///
static void mouseButtonCallback( GLFWwindow * window, int button, int action, int /* mods */ )
{
    InputHandler & input = *static_cast< InputHandler * >( glfwGetWindowUserPointer( window ) );

    using ib = InputHandler::MouseButton;
    using ia = InputHandler::MouseAction;

    static std::map< int, ib > glfw2noo_button = { { GLFW_MOUSE_BUTTON_LEFT, ib::LEFT }
                                                 , { GLFW_MOUSE_BUTTON_MIDDLE, ib::MIDDLE }
                                                 , { GLFW_MOUSE_BUTTON_RIGHT, ib::RIGHT } };

    static std::map< int, ia > glfw2noo_action = { { GLFW_PRESS, ia::PRESS }
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

    /*noolog::trace( "tracing..." );
    noolog::debug( "debugging... " );
    noolog::info( "informing..." );
    noolog::warn( "warning..." );
    noolog::error( "erroring..." );*/

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

    int const rt_width  = 40;
    int const rt_height = 30;

    auto rt = renderer.createRenderTarget( rt_width, rt_height );
    auto rt_color = renderer.createTexture2D( rt_width, rt_height, ETextureFormat::RGB );
    auto rt_depth = renderer.createTexture2D( rt_width, rt_height, ETextureFormat::DEPTH_24_STENCIL_8 );

    rt->attachTexture2D( noo::renderer::RenderTarget::COLOR_ATTACHMENT0, *rt_color );
    rt->attachTexture2D( noo::renderer::RenderTarget::DEPTH_STENCIL_ATTACHMENT, *rt_depth );
    //rt->attachRenderbuffer( noo::renderer::RenderTarget::DEPTH_STENCIL_ATTACHMENT, *rt_depth );

    std::vector< noo::renderer::Vertex_Pos3Color4 > vData =
    {
        { -0.5f,  0.5f, 10.0f, 1.0f, 0.0f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
        {  0.5f, -0.5f, -10.0f, 0.0f, 0.0f, 1.0f, 1.0f },
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

    auto tex = renderer.createTexture2D( tex_size, tex_size, ETextureFormat::RGBA, imgData.data() );


    noo::scene::Camera cam;
    cam.setAspectRatio( 800.0f / 600.0f );
    cam.setPosition( 0, 0, 4 );
    cam.lookAt( 0, 0, 0 );

    noo::renderer::Shader::Data shdSolid( *shaderSolid );
    noo::renderer::Shader::Data shdTex( *shaderTex );

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

    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    using noo::renderer::EWrapMode;
    using noo::renderer::EMinFilterMode;
    using noo::renderer::EMagFilterMode;
    using noo::renderer::state::StateSet;


    int frame = 0;
    int border = 15;

    while ( ! glfwWindowShouldClose( window ) )
    {
        StateSet stateSet;

        // pre-pass - render to texture
        {
            renderer.clear( *rt, clrColor, 1.0f, 0 );

            {
                shdSolid[ "u_mvp" ] = cam.getViewProjectionMatrix();
                shdSolid[ "u_color" ] = glm::vec4( 1, 1, 1, 1 );

                renderer.draw( *rt, shdSolid, stateSet, geoTri );
            }
        }

        // render textured quad to screen
        {
            stateSet.viewport = noo::renderer::state::ViewportState( border, border, renderer.defaultRenderTarget().getWidth() - border*2, renderer.defaultRenderTarget().getHeight() - border*2 );

            renderer.clear( renderer.defaultRenderTarget(), glm::vec4( 0, 1, 0, 1 ) );
            {
                shdTex[ "u_mvp" ] = glm::mat4();

                if ( frame % 150 < 50 )
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ rt_color.get(), EWrapMode::REPEAT, EWrapMode::REPEAT, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };
                else if ( frame % 150 < 100 )
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ rt_depth.get(), EWrapMode::REPEAT, EWrapMode::REPEAT, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };
                else
                    shdTex[ "s2D_tex" ] = noo::renderer::TextureSampler{ tex.get(), EWrapMode::CLAMP, EWrapMode::MIRROR, EMinFilterMode::NEAREST, EMagFilterMode::NEAREST };

                renderer.draw( renderer.defaultRenderTarget(), shdTex, stateSet, geoQuad );
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
