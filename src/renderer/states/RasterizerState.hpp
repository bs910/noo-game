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

    RasterizerState()
        : FillMode( EPolygonFillMode::SOLID )
    { }

    RasterizerState( EPolygonFillMode fillMode )
        : FillMode( fillMode )
    { }

    static RasterizerState
    Wireframe()
    {
        return { EPolygonFillMode::LINE };
    }
};

} // - namespace state
} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_STATE_RASTERIZERSTATE_HPP_INCLUDED */

