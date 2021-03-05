//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_IMPLEMENTATION_VIEWPORT_IPP_
#define GL_IMPLEMENTATION_VIEWPORT_IPP_

namespace gl
{
// 13.6.1 Controlling viewport.
void set_depth_range        (              double near, double far)
{
  glDepthRange(near, far);
}
void set_depth_range        (              float  near, float  far)
{
  glDepthRangef(near, far);
}
void set_indexed_depth_range(GLuint index, double near, double far)
{
  glDepthRangeIndexed(index, near, far);
}
void set_viewport           (              const std::array<GLint  , 2>& offset, const std::array<GLsizei, 2>& size)
{
  glViewport(offset[0], offset[1], size[0], size[1]);
}
void set_indexed_viewport   (GLuint index, const std::array<GLfloat, 2>& offset, const std::array<GLfloat, 2>& size)
{
  glViewportIndexedf(index, offset[0], offset[1], size[0], size[1]);
}
}

#endif