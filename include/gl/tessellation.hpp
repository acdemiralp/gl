//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_TESSELLATION_HPP_
#define GL_TESSELLATION_HPP_

#include <array>

#include <gl/opengl.hpp>

namespace gl
{
// 10.1.15 Separate patches.
void set_patch_vertex_count(GLint count);

// 11.2.2 Tessellation primitive generation.
void set_patch_default_inner_level(const std::array<GLfloat, 2>& inner_level);
void set_patch_default_outer_level(const std::array<GLfloat, 4>& outer_level);

// 22.1 Simple queries.
GLint                  patch_vertex_count       ();
std::array<GLfloat, 2> patch_default_inner_level();
std::array<GLfloat, 4> patch_default_outer_level();
}

#include <gl/implementation/tessellation.ipp>

#endif
