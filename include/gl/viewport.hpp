//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_VIEWPORT_HPP
#define GL_VIEWPORT_HPP

#include <array>

#include <gl/opengl.hpp>

#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif

namespace gl
{
// 13.6.1 Controlling viewport.
inline void set_depth_range        (                    const GLdouble near, const GLdouble far)
{
  glDepthRange(near, far);
}
inline void set_depth_range        (                    const GLfloat  near, const GLfloat  far)
{
  glDepthRangef(near, far);
}
inline void set_indexed_depth_range(const GLuint index, const GLdouble near, const GLdouble far)
{
  glDepthRangeIndexed(index, near, far);
}
inline void set_viewport           (                    const std::array<GLint  , 2>& offset, const std::array<GLsizei, 2>& size)
{
  glViewport(offset[0], offset[1], size[0], size[1]);
}
inline void set_indexed_viewport   (const GLuint index, const std::array<GLfloat, 2>& offset, const std::array<GLfloat, 2>& size)
{
  glViewportIndexedf(index, offset[0], offset[1], size[0], size[1]);
}
}

#endif