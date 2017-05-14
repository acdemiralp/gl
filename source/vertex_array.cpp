//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/vertex_array.hpp>

namespace gl
{
// 10.3.1 Vertex array objects.
vertex_array::vertex_array ()
{
  glCreateVertexArrays(1, &id_);
}
vertex_array::vertex_array (GLuint id): id_(id), managed_(false)
{

}
vertex_array::~vertex_array()
{
  if (managed_)
  glDeleteVertexArrays(1, &id_);
}

void vertex_array::bind    () const
{
  glBindVertexArray(id_);
}
void vertex_array::unbind  ()
{
  glBindVertexArray(0);
}
bool vertex_array::is_valid() const
{
  return glIsVertexArray(id_) != 0;
}

void vertex_array::set_element_buffer(const buffer& buffer)
{
  glVertexArrayElementBuffer(id_, buffer.id());
}

// 10.3.2 Generic vertex attribute arrays.
void vertex_array::set_vertex_buffer           (GLuint binding_index, const buffer& buffer, GLintptr offset, GLsizei stride)
{
  glVertexArrayVertexBuffer(id_, binding_index, buffer.id(), offset, stride);
}
void vertex_array::set_attribute_enabled       (GLuint index, bool   enabled      )
{
  enabled ? glEnableVertexArrayAttrib(id_, index) : glDisableVertexArrayAttrib(id_, index);
}
void vertex_array::set_attribute_binding       (GLuint index, GLuint binding_index)
{
  glVertexArrayAttribBinding(id_, index, binding_index);
}
void vertex_array::set_attribute_format        (GLuint index, GLint size, GLenum type, bool normalized, GLuint relative_offset)
{
  glVertexArrayAttribFormat(id_, index, size, type, normalized, relative_offset);
}
void vertex_array::set_attribute_format_integer(GLuint index, GLint size, GLenum type, GLuint relative_offset)
{
  glVertexArrayAttribIFormat(id_, index, size, type, relative_offset);
}
void vertex_array::set_attribute_format_long   (GLuint index, GLint size, GLenum type, GLuint relative_offset)
{
  glVertexArrayAttribLFormat(id_, index, size, type, relative_offset);
}

// 10.3.4 Vertex attribute divisors.
void vertex_array::set_binding_divisor(GLuint binding_index, GLuint divisor)
{
  glVertexArrayBindingDivisor(id_, binding_index, divisor);
}

// 10.5 Vertex array queries.
buffer  vertex_array::element_buffer                 () const
{
  return buffer(get_parameter(GL_ELEMENT_ARRAY_BUFFER_BINDING));
}
GLint   vertex_array::attribute_relative_offset      (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_RELATIVE_OFFSET);
}
bool    vertex_array::attribute_enabled              (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED) != 0;
}
GLsizei vertex_array::attribute_size                 (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_SIZE);
}
GLsizei vertex_array::attribute_stride               (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE);
}
GLenum  vertex_array::attribute_type                 (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_TYPE);
}
bool    vertex_array::attribute_normalized           (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED) != 0;
}
bool    vertex_array::attribute_integer              (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_INTEGER) != 0;
}
bool    vertex_array::attribute_long                 (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_LONG) != 0;
}
GLint   vertex_array::attribute_divisor              (GLuint index) const
{
  return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_DIVISOR);
}
GLint64 vertex_array::attribute_vertex_binding_offset(GLuint index) const
{
  return get_attribute_parameter_64(index, GL_VERTEX_BINDING_OFFSET);
}

GLuint vertex_array::id() const
{
  return id_;
}

GLint   vertex_array::get_parameter             (              GLenum parameter) const
{
  GLint result;
  glGetVertexArrayiv(id_, parameter, &result);
  return result;
}
GLint   vertex_array::get_attribute_parameter   (GLuint index, GLenum parameter) const
{
  GLint result;
  glGetVertexArrayIndexediv(id_, index, parameter, &result);
  return result;
}
GLint64 vertex_array::get_attribute_parameter_64(GLuint index, GLenum parameter) const
{
  GLint64 result;
  glGetVertexArrayIndexed64iv(id_, index, parameter, &result);
  return result;
}
}