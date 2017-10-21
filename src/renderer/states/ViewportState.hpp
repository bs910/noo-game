///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_STATE_VIEWPORTSTATE_HPP_INCLUDED
#define NOO_RENDERER_STATE_VIEWPORTSTATE_HPP_INCLUDED


/// Includes


namespace noo {
namespace renderer {
namespace state {

struct ViewportState
{
    /// @brief The lower left corner of the viewport rectangle.
    int X;
    int Y;

    /// @brief The size of the viewport rectangle.
    int Width;
    int Height;

    /// @brief The default viewport state indicating that the viewport rectangle
    ///        shall cover the whole extent of the render target being drawn to.
    ViewportState()
        : X( -1 ), Y( -1 ), Width( -1 ), Height( -1 )
    { }

    ViewportState( int x, int y, int w, int h )
        : X( x ), Y( y ), Width( w ), Height( h )
    { }

    bool
    isDefault() const
    {
        return Width == -1;
    }
};

} // - namespace state
} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_STATE_VIEWPORTSTATE_HPP_INCLUDED */

