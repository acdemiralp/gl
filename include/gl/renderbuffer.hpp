//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_RENDERBUFFER_HPP_
#define GL_RENDERBUFFER_HPP_

#include <gl/opengl.hpp>

#include <export.hpp>

namespace gl
{
template<GLenum type>
class texture;

class GL_EXPORT renderbuffer
{
public:
  // 9.2.4 Renderbuffer objects.
  renderbuffer();
  renderbuffer(GLuint id);
  renderbuffer(const renderbuffer&  that);
  renderbuffer(      renderbuffer&& temp) = default;
  virtual ~renderbuffer();
  renderbuffer& operator=(const renderbuffer&  that);
  renderbuffer& operator=(      renderbuffer&& temp) = default;

  void        bind    () const;
  static void unbind  ();
  bool        is_valid() const;

  void set_storage            (                 GLenum internal_format, GLsizei width, GLsizei height);
  void set_storage_multisample(GLsizei samples, GLenum internal_format, GLsizei width, GLsizei height);

  // 9.2.6 Renderbuffer object queries (bindless).
  GLsizei width          () const;
  GLsizei height         () const;
  GLenum  internal_format() const;
  GLsizei samples        () const;
  GLsizei red_size       () const;
  GLsizei green_size     () const;
  GLsizei blue_size      () const;
  GLsizei alpha_size     () const;
  GLsizei depth_size     () const;
  GLsizei stencil_size   () const;
  
  // 18.3 Copying pixels.
  template<GLenum source_target>
  void copy_image_sub_data(const texture<source_target>& source,
    GLint source_level, GLint source_x, GLint source_y, GLint source_z,
    GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth);
  void copy_image_sub_data(const renderbuffer& source,
    GLint source_level, GLint source_x, GLint source_y, GLint source_z,
    GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth);
  
  // 22.3 Internal format queries.
  static GLint   internal_format_info   (GLenum internal_format, GLenum parameter);
  static GLint64 internal_format_info_64(GLenum internal_format, GLenum parameter);

  static const GLenum native_type = GL_RENDERBUFFER;

  GLuint id() const;

protected:
  GLint get_parameter(GLenum parameter) const;

  GLuint id_      ;
  bool   managed_ = true;
};

template <GLenum source_target>
void renderbuffer::copy_image_sub_data(
  const texture<source_target>& source,
  GLint source_level, GLint source_x, GLint source_y, GLint source_z,
  GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth)
{
  glCopyImageSubData(source.id(), source_target, source_level, source_x, source_y, source_z, id_, GL_RENDERBUFFER, level, x, y, z, width, height, depth);
}
}

#endif
