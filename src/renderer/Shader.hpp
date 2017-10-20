///////////////////////////////////////////////////////////////////////////////
/// @file: Shader.hpp                                                       ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_SHADER_HPP__INCLUDED__
#define NOO_RENDERER_SHADER_HPP__INCLUDED__


/// Forward declarations


/// Includes
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../logging/Logger.hpp"
#include "TextureSampler.hpp"

/// Using declarations
using noolog = noo::logging::Logger;

#ifndef SHADER_SOURCE
#define SHADER_SOURCE(x) #x
#endif


namespace noo {
namespace renderer {

struct UniformData
{
    void * Data;
    int SizeOfData;

    GLenum Type;
    GLint Location;
    std::string Name;

    UniformData( GLenum type, GLint loc, std::string name )
        : Data( nullptr )
        , SizeOfData( 0 )
        , Type( type )
        , Location( loc )
        , Name( name )
    {
        switch ( type )
        {
            case GL_INT       : SizeOfData = sizeof( GLint ); break;
            case GL_INT_VEC2  : SizeOfData = sizeof( glm::ivec2 ); break;
            case GL_INT_VEC3  : SizeOfData = sizeof( glm::ivec3 ); break;
            case GL_INT_VEC4  : SizeOfData = sizeof( glm::ivec4 ); break;
            case GL_FLOAT     : SizeOfData = sizeof( GLfloat ); break;
            case GL_FLOAT_VEC2: SizeOfData = sizeof( glm::vec2 ); break;
            case GL_FLOAT_VEC3: SizeOfData = sizeof( glm::vec3 ); break;
            case GL_FLOAT_VEC4: SizeOfData = sizeof( glm::vec4 ); break;
            case GL_FLOAT_MAT4: SizeOfData = sizeof( glm::mat4 ); break;

            case GL_SAMPLER_1D:
            case GL_SAMPLER_2D:
                /* etc... */
                SizeOfData = sizeof( TextureSampler ); break;

            default: assert( false && "Uniform data type not supported!" );
        }

        Data = malloc( SizeOfData );
    }

    ~UniformData()
    {
        if ( Data != nullptr )
            free( Data );
    }

    UniformData( UniformData const & other )
        : Data( nullptr )
        , SizeOfData( other.SizeOfData )
        , Type( other.Type )
        , Location( other.Location )
        , Name( other.Name )
    {
        Data = malloc( SizeOfData );
        memcpy( Data, other.Data, SizeOfData );
    }

    UniformData & operator=( UniformData const & other )
    {
        SizeOfData = other.SizeOfData;
        Data = malloc( SizeOfData );

        memcpy( Data, other.Data, SizeOfData );

        Type = other.Type;
        Location = other.Location;
        Name = other.Name;

        return *this;
    }

    bool
    isTextureSampler() const
    { return Type == GL_SAMPLER_1D || Type == GL_SAMPLER_2D; /* etc... */ }

    UniformData & operator=( int val )
    {
        memcpy( Data, &val, sizeof( int ) );
        return *this;
    }

    UniformData & operator=( glm::ivec2 const & val )
    {
        memcpy( Data, &val, sizeof( glm::ivec2 ) );
        return *this;
    }

    UniformData & operator=( glm::ivec3 const & val )
    {
        memcpy( Data, &val, sizeof( glm::ivec3 ) );
        return *this;
    }

    UniformData & operator=( glm::ivec4 const & val )
    {
        memcpy( Data, &val, sizeof( glm::ivec4 ) );
        return *this;
    }

    UniformData & operator=( float val )
    {
        memcpy( Data, &val, sizeof( float ) );
        return *this;
    }

    UniformData & operator=( glm::vec2 const & val )
    {
        memcpy( Data, &val, sizeof( glm::vec2 ) );
        return *this;
    }

    UniformData & operator=( glm::vec3 const & val )
    {
        memcpy( Data, &val, sizeof( glm::vec3 ) );
        return *this;
    }

    UniformData & operator=( glm::vec4 const & val )
    {
        memcpy( Data, &val, sizeof( glm::vec4 ) );
        return *this;
    }

    UniformData & operator=( glm::mat4 const & mat )
    {
        memcpy( Data, glm::value_ptr( mat ), sizeof( glm::mat4 ) );
        return *this;
    }

    UniformData & operator=( TextureSampler const & ts )
    {
        memcpy( Data, &ts, sizeof( TextureSampler ) );
        return *this;
    }

    void
    apply() const
    {
        switch ( Type )
        {
            case GL_INT       : glUniform1i ( Location, *reinterpret_cast< GLint* >( Data ) ); break;
            case GL_INT_VEC2  : glUniform2iv( Location, 1, reinterpret_cast< GLint* >( Data) ); break;
            case GL_INT_VEC3  : glUniform3iv( Location, 1, reinterpret_cast< GLint* >( Data) ); break;
            case GL_INT_VEC4  : glUniform4iv( Location, 1, reinterpret_cast< GLint* >( Data) ); break;

            case GL_FLOAT     : glUniform1f ( Location, *reinterpret_cast< GLfloat* >( Data ) ); break;
            case GL_FLOAT_VEC2: glUniform2fv( Location, 1, reinterpret_cast< GLfloat* >( Data ) ); break;
            case GL_FLOAT_VEC3: glUniform3fv( Location, 1, reinterpret_cast< GLfloat* >( Data ) ); break;
            case GL_FLOAT_VEC4: glUniform4fv( Location, 1, reinterpret_cast< GLfloat* >( Data ) ); break;

            case GL_FLOAT_MAT4: glUniformMatrix4fv( Location, 1, GL_FALSE, reinterpret_cast< GLfloat* >( Data ) ); break;

            default: assert( false && "Uniform data type not supported or tried to apply a texture sampler!" );
        }
    }
};


class Shader
{
    friend class Renderer;

public:

    class Data
    {
    public:

        explicit Data( Shader & shader )
            : m_Shader( shader )
        {
            // create uniform data for each uniform of the shader
            for ( auto const & u : shader.getUniforms() )
            {
                m_UniformData.emplace_back( u.Type, u.Location, u.Name );
            }
        }

        UniformData & operator[]( char const * name )
        {
            for ( auto & u : m_UniformData )
            {
                if ( u.Name.compare( name ) == 0 )
                    return u;
            }

            throw std::exception();
        }

        Shader &
        getShader()
        { return m_Shader; }

        std::vector< UniformData > &
        getUniformData()
        { return m_UniformData; }

    private:
        Shader & m_Shader;
        std::vector< UniformData > m_UniformData;
    };

    struct UniformDesc
    {
        UniformDesc( GLint loc, GLenum type, std::string const & name )
            : Location( loc )
            , Type( type )
            , Name( name )
        { }

        GLint Location;
        GLenum Type;
        std::string Name;
    };

    void
    activate()
    {
        glUseProgram( m_ProgramHandle );
    }

    void
    deactivate()
    {
        glUseProgram( 0 );
    }

    ~Shader()
    {
        glDeleteShader( m_VertexShader );
        glDeleteShader( m_TessCtrlShader );
        glDeleteShader( m_TessEvalShader );
        glDeleteShader( m_GeometryShader );
        glDeleteShader( m_FragmentShader );

        glDeleteProgram( m_ProgramHandle );

        noolog::trace( "line " + std::to_string( __LINE__ ) + ":" + std::string( __func__ ) + " :: Destroyed shader." );
    }


    Shader( Shader const & ) = delete;
    Shader( Shader const && ) = delete;
    Shader & operator=( Shader const & ) = delete;

protected:

    Shader( char const * vertexSource
          , char const * tessCtrlSource
          , char const * tessEvalSource
          , char const * geometrySource
          , char const * fragmentSource )
        : m_VertexShader( 0 )
        , m_TessCtrlShader( 0 )
        , m_TessEvalShader( 0 )
        , m_GeometryShader( 0 )
        , m_FragmentShader( 0 )
        , m_ProgramHandle( 0 )
    {
        assert( vertexSource && "No vertex shader source given but is mandatory!" );
        assert( fragmentSource && "No fragment shader source given but is mandatory!" );

        m_ProgramHandle = glCreateProgram();

        m_VertexShader = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( m_VertexShader, 1, &vertexSource, NULL );
        glCompileShader( m_VertexShader );
        glAttachShader( m_ProgramHandle, m_VertexShader );

        logShaderInfo( m_VertexShader );

        if ( tessCtrlSource )
        {
            m_TessCtrlShader = glCreateShader( GL_TESS_CONTROL_SHADER );
            glShaderSource( m_TessCtrlShader, 1, &tessCtrlSource, NULL );
            glCompileShader( m_TessCtrlShader );
            glAttachShader( m_ProgramHandle, m_TessCtrlShader );

            logShaderInfo( m_TessCtrlShader );
        }

        if ( tessEvalSource )
        {
            m_TessEvalShader = glCreateShader( GL_TESS_EVALUATION_SHADER );
            glShaderSource( m_TessEvalShader, 1, &tessEvalSource, NULL );
            glCompileShader( m_TessEvalShader );
            glAttachShader( m_ProgramHandle, m_TessEvalShader );

            logShaderInfo( m_TessEvalShader );
        }

        if ( geometrySource )
        {
            m_GeometryShader = glCreateShader( GL_GEOMETRY_SHADER );
            glShaderSource( m_GeometryShader, 1, &geometrySource, NULL );
            glCompileShader( m_GeometryShader );
            glAttachShader( m_ProgramHandle, m_GeometryShader );

            logShaderInfo( m_GeometryShader );
        }

        m_FragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( m_FragmentShader, 1, &fragmentSource, NULL );
        glCompileShader( m_FragmentShader );
        glAttachShader( m_ProgramHandle, m_FragmentShader );

        logShaderInfo( m_FragmentShader );

        glLinkProgram( m_ProgramHandle );

        GLint logLen;
        glGetProgramiv( m_ProgramHandle, GL_INFO_LOG_LENGTH, &logLen );
        if ( logLen > 1 )
        {
            std::vector< char > logInfo( logLen );
            glGetProgramInfoLog( m_ProgramHandle, logLen, NULL, &logInfo[ 0 ] );

            noolog::debug( logInfo.data() );
        }

        // enumerate uniforms
        GLint numUniforms{ 0 };
        glGetProgramiv( m_ProgramHandle, GL_ACTIVE_UNIFORMS, &numUniforms );

        size_t const bufferSize = 128;

        for ( int i = 0; i < numUniforms; ++i )
        {
            GLchar name[ bufferSize ];
            GLenum type;
            GLint size;
            glGetActiveUniform( m_ProgramHandle, i, bufferSize - 1, nullptr, &size, &type, name );

            GLint loc = glGetUniformLocation( m_ProgramHandle, name );
            if ( loc == -1 )
            {
                noolog::error( name );
            }

            m_Uniforms.emplace_back( loc, type, std::string( name ) );
        }

        noolog::trace( "line " + std::to_string( __LINE__ ) + ":" + std::string( __func__ ) + " :: Created shader." );
    }

    std::vector< UniformDesc > const &
    getUniforms() const
    {
        return m_Uniforms;
    }

private:

    void
    logShaderInfo( GLuint shader )
    {
        GLint logLen;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );

        if ( logLen > 1 )
        {
            std::vector< char > logInfo( logLen );
            glGetShaderInfoLog( shader, logLen, NULL, &logInfo[ 0 ] );

            noolog::debug( logInfo.data() );
        }
    }

    /// @brief Contains all uniforms of a shader program.
    std::vector< UniformDesc > m_Uniforms;

    /// @brief Stores the handles to the OpenGL shader objects.
    GLuint m_VertexShader;
    GLuint m_TessCtrlShader;
    GLuint m_TessEvalShader;
    GLuint m_GeometryShader;
    GLuint m_FragmentShader;

    /// @brief Stores the handle to the OpenGL shader program.
    GLuint m_ProgramHandle;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_SHADER_HPP__INCLUDED__ */

