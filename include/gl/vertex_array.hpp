//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_VERTEX_ARRAY_HPP
#define GL_VERTEX_ARRAY_HPP

#include <gl/buffer.hpp>
#include <gl/opengl.hpp>

namespace gl
{
class vertex_array
{
public:
  // 10.3.1 Vertex array objects.
  vertex_array()
  {
    glCreateVertexArrays(1, &id_);
  }
  explicit vertex_array(GLuint id) : id_(id), managed_(false)
  {

  }
  vertex_array(const vertex_array&  that) = delete;
  vertex_array(      vertex_array&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
  {
    temp.id_ = invalid_id;
    temp.managed_ = false;
  }
  virtual ~vertex_array()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteVertexArrays(1, &id_);
  }
  vertex_array& operator=(const vertex_array&  that) = delete;
  vertex_array& operator=(      vertex_array&& temp) noexcept
  {
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteVertexArrays(1, &id_);
  
      id_      = std::move(temp.id_);
      managed_ = std::move(temp.managed_);
  
      temp.id_      = invalid_id;
      temp.managed_ = false;    
    }
    return *this;
  }

  void        bind    () const
  {
    glBindVertexArray(id_);
  }
  static void unbind  ()
  {
    glBindVertexArray(0);
  }
  bool        is_valid() const
  {
    return glIsVertexArray(id_) != 0;
  }

  void set_element_buffer(const buffer& buffer)
  {
    glVertexArrayElementBuffer(id_, buffer.id());
  }

  // 10.3.2 Generic vertex attribute arrays.
  void set_vertex_buffer           (GLuint binding_index, const buffer& buffer, GLintptr offset = 0, GLsizei stride = 1)
  {
    glVertexArrayVertexBuffer(id_, binding_index, buffer.id(), offset, stride);
  }
  void set_attribute_enabled       (GLuint index, bool   enabled      )
  {
    enabled ? glEnableVertexArrayAttrib(id_, index) : glDisableVertexArrayAttrib(id_, index);
  }
  void set_attribute_binding       (GLuint index, GLuint binding_index)
  {
    glVertexArrayAttribBinding(id_, index, binding_index);
  }
  void set_attribute_format        (GLuint index, GLint  size, GLenum type, bool normalized = false, GLuint relative_offset = 0)
  {
    glVertexArrayAttribFormat(id_, index, size, type, normalized, relative_offset);
  }
  void set_attribute_format_integer(GLuint index, GLint  size, GLenum type, GLuint relative_offset = 0)
  {
    glVertexArrayAttribIFormat(id_, index, size, type, relative_offset);
  }
  void set_attribute_format_long   (GLuint index, GLint  size, GLenum type, GLuint relative_offset = 0)
  {
    glVertexArrayAttribLFormat(id_, index, size, type, relative_offset);
  }

  // 10.3.4 Vertex attribute divisors.
  void set_binding_divisor(GLuint binding_index, GLuint divisor)
  {
    glVertexArrayBindingDivisor(id_, binding_index, divisor);
  }

  // 10.5 Vertex array queries.
  buffer  element_buffer                 () const
  {
    return buffer(get_parameter(GL_ELEMENT_ARRAY_BUFFER_BINDING));
  }
  GLint   attribute_relative_offset      (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_RELATIVE_OFFSET);
  }
  bool    attribute_enabled              (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED) != 0;
  }
  GLsizei attribute_size                 (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_SIZE);
  }
  GLsizei attribute_stride               (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE);
  }
  GLenum  attribute_type                 (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_TYPE);
  }
  bool    attribute_normalized           (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED) != 0;
  }
  bool    attribute_integer              (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_INTEGER) != 0;
  }
  bool    attribute_long                 (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_LONG) != 0;
  }
  GLint   attribute_divisor              (GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_DIVISOR);
  }
  GLint64 attribute_vertex_binding_offset(GLuint index) const
  {
    return get_attribute_parameter_64(index, GL_VERTEX_BINDING_OFFSET);
  }

  static const GLenum native_type = GL_VERTEX_ARRAY;

  GLuint id() const
  {
    return id_;
  }

private:
  GLint   get_parameter             (              GLenum parameter) const
  {
    GLint result;
    glGetVertexArrayiv(id_, parameter, &result);
    return result;
  }
  GLint   get_attribute_parameter   (GLuint index, GLenum parameter) const
  {
    GLint result;
    glGetVertexArrayIndexediv(id_, index, parameter, &result);
    return result;
  }
  GLint64 get_attribute_parameter_64(GLuint index, GLenum parameter) const
  {
    GLint64 result;
    glGetVertexArrayIndexed64iv(id_, index, parameter, &result);
    return result;
  }

  GLuint id_      = 0;
  bool   managed_ = true;
};
}

#endif
