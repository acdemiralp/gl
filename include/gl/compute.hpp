//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_COMPUTE_HPP_
#define GL_COMPUTE_HPP_

#include <gl/opengl.hpp>

#include <gl_export.hpp>

namespace gl
{
// 19.0 Compute shaders.
GL_EXPORT void dispatch_compute         (GLuint grid_x, GLuint grid_y, GLuint grid_z);
GL_EXPORT void dispatch_compute_indirect(GLintptr offset);
}

#endif