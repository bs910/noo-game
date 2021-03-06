///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_RENDERER_STATE_STATESET_HPP_INCLUDED
#define NOO_RENDERER_STATE_STATESET_HPP_INCLUDED


/// Includes
#include "BlendState.hpp"
#include "CullState.hpp"
#include "DepthState.hpp"
#include "RasterizerState.hpp"
#include "StencilState.hpp"
#include "ViewportState.hpp"


namespace noo {
namespace renderer {
namespace state {

struct StateSet
{
    BlendState blend;
    CullState cull;
    DepthState depth;
    RasterizerState rasterizer;
    StencilState stencil;
    ViewportState viewport;
};

} // - namespace state
} // - namespace renderer
} // - namespace noo


#endif /* NOO_RENDERER_STATE_STATESET_HPP_INCLUDED */

