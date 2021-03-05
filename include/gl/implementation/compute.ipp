//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_IMPLEMENTATION_COMPUTE_IPP_
#define GL_IMPLEMENTATION_COMPUTE_IPP_

namespace gl
{
void dispatch_compute         (GLuint grid_x, GLuint grid_y, GLuint grid_z)
{
  glDispatchCompute(grid_x, grid_y, grid_z);
}
void dispatch_compute_indirect(GLintptr offset)
{
  glDispatchComputeIndirect(offset);
}
}

#endif