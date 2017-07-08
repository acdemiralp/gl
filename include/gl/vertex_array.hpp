//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_VERTEX_ARRAY_HPP_
#define GL_VERTEX_ARRAY_HPP_

#include <gl/buffer.hpp>
#include <gl/opengl.hpp>

#include <gl_export.hpp>

namespace gl
{
class GL_EXPORT vertex_array
{
public:
  // 10.3.1 Vertex array objects.
  vertex_array();
  vertex_array(GLuint id);
  vertex_array(const vertex_array&  that) = delete ;
  vertex_array(      vertex_array&& temp) = default;
  virtual ~vertex_array();
  vertex_array& operator=(const vertex_array&  that) = delete ;
  vertex_array& operator=(      vertex_array&& temp) = default;

  void        bind    () const;
  static void unbind  ();
  bool        is_valid() const;

  void set_element_buffer(const buffer& buffer);

  // 10.3.2 Generic vertex attribute arrays.
  void set_vertex_buffer           (GLuint binding_index, const buffer& buffer, GLintptr offset = 0, GLsizei stride = 1);
  void set_attribute_enabled       (GLuint index, bool   enabled      );
  void set_attribute_binding       (GLuint index, GLuint binding_index);
  void set_attribute_format        (GLuint index, GLint  size, GLenum type, bool normalized = false, GLuint relative_offset = 0);
  void set_attribute_format_integer(GLuint index, GLint  size, GLenum type, GLuint relative_offset = 0);
  void set_attribute_format_long   (GLuint index, GLint  size, GLenum type, GLuint relative_offset = 0);

  // 10.3.4 Vertex attribute divisors.
  void set_binding_divisor(GLuint binding_index, GLuint divisor);

  // 10.5 Vertex array queries.
  buffer  element_buffer                 () const;
  GLint   attribute_relative_offset      (GLuint index) const;
  bool    attribute_enabled              (GLuint index) const;
  GLsizei attribute_size                 (GLuint index) const;
  GLsizei attribute_stride               (GLuint index) const;
  GLenum  attribute_type                 (GLuint index) const;
  bool    attribute_normalized           (GLuint index) const;
  bool    attribute_integer              (GLuint index) const;
  bool    attribute_long                 (GLuint index) const;
  GLint   attribute_divisor              (GLuint index) const;
  GLint64 attribute_vertex_binding_offset(GLuint index) const;

  static const GLenum native_type = GL_VERTEX_ARRAY;

  GLuint id() const;

private:
  GLint   get_parameter             (              GLenum parameter) const;
  GLint   get_attribute_parameter   (GLuint index, GLenum parameter) const;
  GLint64 get_attribute_parameter_64(GLuint index, GLenum parameter) const;

  GLuint id_      = 0;
  bool   managed_ = true;
};
}

#endif
