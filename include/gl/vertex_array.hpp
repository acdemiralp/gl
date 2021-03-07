//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_VERTEX_ARRAY_HPP
#define GL_VERTEX_ARRAY_HPP

#include <gl/opengl.hpp>
#include <gl/buffer.hpp>

namespace gl
{
class vertex_array
{
public:
  // 10.3.1 Vertex array objects.
  vertex_array           ()
  {
    glCreateVertexArrays(1, &id_);
  }
  explicit vertex_array  (const GLuint id) : id_(id), managed_(false)
  {

  }
  vertex_array           (const vertex_array&  that) = delete;
  vertex_array           (      vertex_array&& temp) noexcept : id_(temp.id_), managed_(temp.managed_)
  {
    temp.id_      = invalid_id;
    temp.managed_ = false;
  }
  virtual ~vertex_array  ()
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
  
      id_      = temp.id_;
      managed_ = temp.managed_;
  
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
  [[nodiscard]]
  bool        is_valid() const
  {
    return glIsVertexArray(id_) != 0;
  }

  void set_element_buffer(const buffer& buffer) const
  {
    glVertexArrayElementBuffer(id_, buffer.id());
  }

  // 10.3.2 Generic vertex attribute arrays.
  void set_vertex_buffer           (const GLuint binding_index, const buffer& buffer, const GLintptr offset = 0, const GLsizei stride = 1) const
  {
    glVertexArrayVertexBuffer(id_, binding_index, buffer.id(), offset, stride);
  }
  void set_attribute_enabled       (const GLuint index, const bool   enabled      ) const
  {
    enabled ? glEnableVertexArrayAttrib(id_, index) : glDisableVertexArrayAttrib(id_, index);
  }
  void set_attribute_binding       (const GLuint index, const GLuint binding_index) const
  {
    glVertexArrayAttribBinding(id_, index, binding_index);
  }
  void set_attribute_format        (const GLuint index, const GLint  size, const GLenum type, const bool normalized = false, const GLuint relative_offset = 0) const
  {
    glVertexArrayAttribFormat(id_, index, size, type, normalized, relative_offset);
  }
  void set_attribute_format_integer(const GLuint index, const GLint  size, const GLenum type, const GLuint relative_offset = 0) const
  {
    glVertexArrayAttribIFormat(id_, index, size, type, relative_offset);
  }
  void set_attribute_format_long   (const GLuint index, const GLint  size, const GLenum type, const GLuint relative_offset = 0) const
  {
    glVertexArrayAttribLFormat(id_, index, size, type, relative_offset);
  }

  // 10.3.4 Vertex attribute divisors.
  void set_binding_divisor(const GLuint binding_index, const GLuint divisor) const
  {
    glVertexArrayBindingDivisor(id_, binding_index, divisor);
  }

  // 10.5 Vertex array queries.
  [[nodiscard]]
  buffer  element_buffer                 () const
  {
    return buffer(get_parameter(GL_ELEMENT_ARRAY_BUFFER_BINDING));
  }
  [[nodiscard]]
  GLint   attribute_relative_offset      (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_RELATIVE_OFFSET);
  }
  [[nodiscard]]
  bool    attribute_enabled              (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED) != 0;
  }
  [[nodiscard]]
  GLsizei attribute_size                 (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_SIZE);
  }
  [[nodiscard]]
  GLsizei attribute_stride               (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE);
  }
  [[nodiscard]]
  GLenum  attribute_type                 (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_TYPE);
  }
  [[nodiscard]]
  bool    attribute_normalized           (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED) != 0;
  }
  [[nodiscard]]
  bool    attribute_integer              (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_INTEGER) != 0;
  }
  [[nodiscard]]
  bool    attribute_long                 (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_LONG) != 0;
  }
  [[nodiscard]]
  GLint   attribute_divisor              (const GLuint index) const
  {
    return get_attribute_parameter(index, GL_VERTEX_ATTRIB_ARRAY_DIVISOR);
  }
  [[nodiscard]]
  GLint64 attribute_vertex_binding_offset(const GLuint index) const
  {
    return get_attribute_parameter_64(index, GL_VERTEX_BINDING_OFFSET);
  }

  static const GLenum native_type = GL_VERTEX_ARRAY;

  [[nodiscard]]
  GLuint id() const
  {
    return id_;
  }

private:
  [[nodiscard]]
  GLint   get_parameter             (                    const GLenum parameter) const
  {
    GLint result;
    glGetVertexArrayiv(id_, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint   get_attribute_parameter   (const GLuint index, const GLenum parameter) const
  {
    GLint result;
    glGetVertexArrayIndexediv(id_, index, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint64 get_attribute_parameter_64(const GLuint index, const GLenum parameter) const
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
