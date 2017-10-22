///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_STATE_RASTERIZERSTATE_HPP_INCLUDED
#define NOO_RENDERER_STATE_RASTERIZERSTATE_HPP_INCLUDED


/// Includes


namespace noo {
namespace renderer {
namespace state {

enum class EPolygonFillMode
{
    SOLID,
    LINE
};

struct RasterizerState
{
    EPolygonFillMode FillMode;
    float LineWidth;

    RasterizerState()
        : FillMode( EPolygonFillMode::SOLID )
        , LineWidth( 1.0f )
    { }

    RasterizerState( EPolygonFillMode fillMode, float lineWidth )
        : FillMode( fillMode )
        , LineWidth( lineWidth )
    { }

    static RasterizerState
    Wireframe()
    {
        return { EPolygonFillMode::LINE, 2.0f };
    }
};

} // - namespace state
} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_STATE_RASTERIZERSTATE_HPP_INCLUDED */

