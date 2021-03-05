//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_COMPUTE_HPP_
#define GL_COMPUTE_HPP_

#include <gl/opengl.hpp>

namespace gl
{
// 19.0 Compute shaders.
void dispatch_compute         (GLuint grid_x, GLuint grid_y, GLuint grid_z);
void dispatch_compute_indirect(GLintptr offset);
}

#include <gl/implementation/compute.ipp>

#endif