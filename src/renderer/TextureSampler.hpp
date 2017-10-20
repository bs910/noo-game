///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_TEXTURESAMPLER_HPP_INCLUDED
#define NOO_RENDERER_TEXTURESAMPLER_HPP_INCLUDED


/// Forward declarations


/// Includes
#include "Texture2D.hpp"

/// Using declarations



namespace noo {
namespace renderer {


enum class EWrapMode
{
    CLAMP,
    REPEAT,
    MIRROR,
    BORDER
};

inline GLint
toGLWrapMode( EWrapMode w )
{
    switch ( w )
    {
        case EWrapMode::CLAMP : return GL_CLAMP_TO_EDGE;
        case EWrapMode::REPEAT: return GL_REPEAT;
        case EWrapMode::MIRROR: return GL_MIRRORED_REPEAT;
        case EWrapMode::BORDER: return GL_CLAMP_TO_BORDER;
    }
}


enum class EMinFilterMode
{
    NEAREST,
    LINEAR
};

enum class EMagFilterMode
{
    NEAREST,
    LINEAR
};

inline GLint
toGLMinFilter( EMinFilterMode f )
{
    switch ( f )
    {
        case EMinFilterMode::NEAREST: return GL_NEAREST;
        case EMinFilterMode::LINEAR : return GL_LINEAR;
    }
}

inline GLint
toGLMagFilter( EMagFilterMode f )
{
    switch ( f )
    {
        case EMagFilterMode::NEAREST: return GL_NEAREST;
        case EMagFilterMode::LINEAR : return GL_LINEAR;
    }
}

struct TextureSampler
{
    Texture2D * Texture;
    EWrapMode WrapS;
    EWrapMode WrapT;
    EMinFilterMode MinFilter;
    EMagFilterMode MagFilter;
};

} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_TEXTURESAMPLER_HPP_INCLUDED */

