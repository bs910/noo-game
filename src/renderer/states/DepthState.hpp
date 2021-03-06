///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_STATE_DEPTHSTATE_HPP_INCLUDED
#define NOO_RENDERER_STATE_DEPTHSTATE_HPP_INCLUDED


/// Includes


namespace noo {
namespace renderer {
namespace state {

enum class EEnableDepthTest
{
    ENABLE,
    DISABLE
};

enum class EEnableDepthWrite
{
    ENABLE,
    DISABLE
};

enum class EDepthFunc
{
    ALWAYS,
    NEVER,
    LESS,
    EQUAL,
    LESS_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL
};

struct DepthState
{
    EEnableDepthTest EnableDepthTesting;
    EEnableDepthWrite EnableDepthWriting;
    EDepthFunc CompareFunc;

    /// @brief Per default, depth testing and writing is ENABLEd, depth func is LESS.
    DepthState()
        : EnableDepthTesting( EEnableDepthTest::ENABLE )
        , EnableDepthWriting( EEnableDepthWrite::ENABLE )
        , CompareFunc( EDepthFunc::LESS )
    { }

    DepthState( EEnableDepthTest dt, EEnableDepthWrite dw, EDepthFunc comp )
        : EnableDepthTesting( dt )
        , EnableDepthWriting( dw )
        , CompareFunc( comp )
    { }

    static DepthState
    Disabled()
    {
        return { EEnableDepthTest::DISABLE, EEnableDepthWrite::DISABLE, EDepthFunc::ALWAYS };
    }

    static DepthState
    ReadOnly()
    {
        return { EEnableDepthTest::ENABLE, EEnableDepthWrite::DISABLE, EDepthFunc::LESS };
    }

    static DepthState
    WriteOnly()
    {
        return { EEnableDepthTest::ENABLE, EEnableDepthWrite::ENABLE, EDepthFunc::ALWAYS };
    }
};

} // - namespace state
} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_STATE_DEPTHSTATE_HPP_INCLUDED */

