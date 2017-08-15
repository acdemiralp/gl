//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/opengl.hpp>

#include <cassert>
#include <iostream>

#include <gl/command_execution.hpp>

namespace gl
{
bool initialize ()
{
  glewExperimental = true;
  return glewInit() == GLEW_OK;
}
void print_error(const char* prefix)
{
  GLenum err = GL_NO_ERROR;
  do
  {
    err = error();
    if (err != GL_NO_ERROR)
      std::cout << prefix << ": " << err << std::endl;
  }
  while (err != GL_NO_ERROR);
}

GLsizei format_component_count(GLenum format)
{
  switch(format)
  {
  case GL_STENCIL_INDEX:
  case GL_DEPTH_COMPONENT:
  case GL_RED:
  case GL_RED_INTEGER:
  case GL_GREEN:
  case GL_GREEN_INTEGER:
  case GL_BLUE:
  case GL_BLUE_INTEGER:
    return 1;

  case GL_DEPTH_STENCIL:
  case GL_RG:
  case GL_RG_INTEGER:
    return 2;

  case GL_RGB:
  case GL_RGB_INTEGER:
  case GL_BGR:
  case GL_BGR_INTEGER:
    return 3;

  case GL_RGBA:
  case GL_RGBA_INTEGER:
  case GL_BGRA:
  case GL_BGRA_INTEGER:
    return 4;

  default:
    assert("Unsupported OpenGL format!");
    return 0;
  }
}
GLsizei type_size             (GLenum type  )
{
  switch(type)
  {
  case GL_UNSIGNED_BYTE: 
  case GL_UNSIGNED_BYTE_3_3_2:
  case GL_UNSIGNED_BYTE_2_3_3_REV:
    return sizeof(GLubyte);

  case GL_BYTE: 
    return sizeof(GLbyte);

  case GL_UNSIGNED_SHORT:
  case GL_UNSIGNED_SHORT_5_6_5:
  case GL_UNSIGNED_SHORT_5_6_5_REV:
  case GL_UNSIGNED_SHORT_4_4_4_4:
  case GL_UNSIGNED_SHORT_4_4_4_4_REV:
  case GL_UNSIGNED_SHORT_5_5_5_1:
  case GL_UNSIGNED_SHORT_1_5_5_5_REV:
    return sizeof(GLushort);

  case GL_SHORT: 
    return sizeof(GLshort);

  case GL_UNSIGNED_INT: 
  case GL_UNSIGNED_INT_8_8_8_8:
  case GL_UNSIGNED_INT_8_8_8_8_REV:
  case GL_UNSIGNED_INT_10_10_10_2:
  case GL_UNSIGNED_INT_2_10_10_10_REV:
  case GL_UNSIGNED_INT_24_8:
  case GL_UNSIGNED_INT_10F_11F_11F_REV:
  case GL_UNSIGNED_INT_5_9_9_9_REV:
    return sizeof(GLuint);

  case GL_INT: 
    return sizeof(GLint);

  case GL_HALF_FLOAT:
    return sizeof(GLhalf);

  case GL_FLOAT: 
    return sizeof(GLfloat);

  default:
    assert("Unsupported OpenGL type!");
    return 0;
  }
}
}