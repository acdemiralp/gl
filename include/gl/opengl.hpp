//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_OPENGL_HPP_
#define GL_OPENGL_HPP_

#include <limits>

#include <gl/export.hpp>
#include <GL/glew.h>

namespace gl
{
GL_EXPORT bool initialize ();
GL_EXPORT void print_error(const char* prefix);

GL_EXPORT GLsizei format_component_count(GLenum format);
GL_EXPORT GLsizei type_size             (GLenum type  );

const GLuint invalid_id = std::numeric_limits<GLuint>::max();
}

#endif