#include "Renderer.hpp"
#include "../logging/Logger.hpp"
#include <iostream>
#include "glad/glad.h"

using noolog = noo::logging::Logger;

namespace noo {
namespace renderer {


void Renderer::initialize()
{
    noolog::info( "Initialized Renderer." );
}


void Renderer::destroy()
{
    noolog::info( "Destroyed Renderer." );
}


void Renderer::clear( RenderTarget & rt, glm::vec4 const & clearColor, float clearDepth, int clearStencil )
{
    rt.activate();
    glClearColor( clearColor.r, clearColor.g, clearColor.b, clearColor.a );
    glClearDepth( clearDepth );
    glClearStencil( clearStencil );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}


} // - namespace renderer
} // - namespace noo
