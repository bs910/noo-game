///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_STATE_BLENDSTATE_HPP_INCLUDED
#define NOO_RENDERER_STATE_BLENDSTATE_HPP_INCLUDED


/// Includes


namespace noo {
namespace renderer {
namespace state {

enum class EBlendEnable
{
    ENABLE,
    DISABLE
};

enum class EBlendEquation
{
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT,
    MIN,
    MAX
};


enum class EBlendFunc
{
    ZERO,
    ONE,
    SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_SRC_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    ONE_MINUS_DST_ALPHA
};


struct BlendState
{
    EBlendEnable Enabled;
    EBlendEquation BlendEq;
    EBlendFunc SourceBlendFunc;
    EBlendFunc DestBlendFunc;

    /// @brief Per default blending is DISABLED.
    BlendState()
        : Enabled( EBlendEnable::DISABLE )
        , BlendEq( EBlendEquation::ADD )
        , SourceBlendFunc( EBlendFunc::ONE )
        , DestBlendFunc( EBlendFunc::ZERO )
    { }

    BlendState( EBlendEnable en, EBlendEquation eq, EBlendFunc src, EBlendFunc dst )
        : Enabled( en )
        , BlendEq( eq )
        , SourceBlendFunc( src )
        , DestBlendFunc( dst )
    { }

    /// @brief Typical alpha blending setup used for rendering order-dependent transparency.
    static BlendState AlphaBlend()
    {
        return { EBlendEnable::ENABLE, EBlendEquation::ADD, EBlendFunc::SRC_ALPHA, EBlendFunc::ONE_MINUS_SRC_ALPHA };
    }
};

} // - namespace state
} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_STATE_BLENDSTATE_HPP_INCLUDED */

