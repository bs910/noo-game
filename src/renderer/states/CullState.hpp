///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_STATE_CULLSTATE_HPP_INCLUDED
#define NOO_RENDERER_STATE_CULLSTATE_HPP_INCLUDED


/// Includes


namespace noo {
namespace renderer {
namespace state {

enum class ECullMode
{
    NONE,
    BACK,
    FRONT,
    BOTH
};

enum class EFrontFaceWinding
{
    CW,
    CCW
};

struct CullState
{
    ECullMode CullMode;
    EFrontFaceWinding FrontFaceWinding;

    /// @brief Default cull state, mode is BACK, front face winding is CCW.
    CullState()
        : CullMode( ECullMode::BACK )
        , FrontFaceWinding( EFrontFaceWinding::CCW )
    { }

    CullState( ECullMode cm, EFrontFaceWinding ffw )
        : CullMode( cm )
        , FrontFaceWinding( ffw )
    { }

    /// @brief Disabled cull state, mode is NONE, front face winding
    ///        is not relevant (but set to CCW as it has to have some value).
    static CullState Disabled()
    {
        return { ECullMode::NONE, EFrontFaceWinding::CCW };
    }

    /// @brief Cull front faces, mode is FRONT, front face winding is CCW.
    static CullState CullFrontFaces()
    {
        return { ECullMode::FRONT, EFrontFaceWinding::CCW };
    }
};

} // - namespace state
} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_STATE_CULLSTATE_HPP_INCLUDED */

