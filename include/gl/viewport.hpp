//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_VIEWPORT_HPP_
#define GL_VIEWPORT_HPP_

#include <array>

#include <gl/opengl.hpp>

#include <export.hpp>

namespace gl
{
// 13.6.1 Controlling viewport.
GL_EXPORT void set_depth_range        (              double near, double far);
GL_EXPORT void set_depth_range        (              float  near, float  far);
GL_EXPORT void set_indexed_depth_range(GLuint index, double near, double far);
GL_EXPORT void set_viewport           (              const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size);
GL_EXPORT void set_indexed_viewport   (GLuint index, const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size);
}

#endif