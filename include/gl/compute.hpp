//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_COMPUTE_HPP
#define GL_COMPUTE_HPP

#include <gl/opengl.hpp>

namespace gl
{
// 19.0 Compute shaders.
inline void dispatch_compute         (const GLuint grid_x, const GLuint grid_y, const GLuint grid_z)
{
  glDispatchCompute(grid_x, grid_y, grid_z);
}
inline void dispatch_compute_indirect(const GLintptr offset)
{
  glDispatchComputeIndirect(offset);
}
}

#endif