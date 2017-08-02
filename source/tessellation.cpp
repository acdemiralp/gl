//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/tessellation.hpp>

namespace gl
{
// 10.1.15 Separate patches.
void set_patch_vertex_count(GLint count)
{
  glPatchParameteri(GL_PATCH_VERTICES, count);
}

// 11.2.2 Tessellation primitive generation.
void set_patch_default_inner_level(const std::array<GLfloat, 2>& inner_level)
{
  glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, inner_level.data());
}
void set_patch_default_outer_level(const std::array<GLfloat, 4>& outer_level)
{
  glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, outer_level.data());
}

// 22.1 Simple queries.
GLint                  patch_vertex_count       ()
{
  GLint result;
  glGetIntegerv(GL_PATCH_VERTICES, &result);
  return result;
}
std::array<GLfloat, 2> patch_default_inner_level()
{
  std::array<GLfloat, 2> result;
  glGetFloatv(GL_PATCH_DEFAULT_INNER_LEVEL, result.data());
  return result;
}
std::array<GLfloat, 4> patch_default_outer_level()
{
  std::array<GLfloat, 4> result;
  glGetFloatv(GL_PATCH_DEFAULT_OUTER_LEVEL, result.data());
  return result;
}
}
