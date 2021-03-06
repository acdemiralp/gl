//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_RENDERBUFFER_HPP
#define GL_RENDERBUFFER_HPP

#include <gl/opengl.hpp>

namespace gl
{
template<GLenum type>
class texture;

class renderbuffer
{
public:
  // 9.2.4 Renderbuffer objects.
  renderbuffer()
  {
    glCreateRenderbuffers(1, &id_);
  }
  explicit renderbuffer(GLuint id) : id_(id), managed_(false)
  {

  }
  renderbuffer(const renderbuffer&  that) : renderbuffer()
  {
    set_storage_multisample(that.samples(), that.internal_format(), that.width(), that.height());
  }
  renderbuffer(      renderbuffer&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
  {
    temp.id_      = invalid_id;
    temp.managed_ = false;
  }
  virtual ~renderbuffer()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteRenderbuffers(1, &id_);
  }
  renderbuffer& operator=(const renderbuffer&  that)
  {
    set_storage_multisample(that.samples(), that.internal_format(), that.width(), that.height());
    return *this;
  }
  renderbuffer& operator=(      renderbuffer&& temp) noexcept
  {
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteRenderbuffers(1, &id_);
  
      id_      = std::move(temp.id_);
      managed_ = std::move(temp.managed_);
  
      temp.id_      = invalid_id;
      temp.managed_ = false;
    }
    return *this;
  }

  void        bind    () const
  {
    glBindRenderbuffer(GL_RENDERBUFFER, id_);
  }
  static void unbind  ()
  {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }
  bool        is_valid() const
  {
    return glIsRenderbuffer(id_) != 0;
  }
  
  void set_storage            (                 GLenum internal_format, GLsizei width, GLsizei height)
  {
    glNamedRenderbufferStorage(id_, internal_format, width, height);
  }
  void set_storage_multisample(GLsizei samples, GLenum internal_format, GLsizei width, GLsizei height)
  {
    glNamedRenderbufferStorageMultisample(id_, samples, internal_format, width, height);
  }
  
  // 9.2.6 Renderbuffer object queries (bindless).
  GLsizei width          () const
  {
    return get_parameter(GL_RENDERBUFFER_WIDTH);
  }
  GLsizei height         () const
  {
    return get_parameter(GL_RENDERBUFFER_HEIGHT);
  }
  GLenum  internal_format() const
  {
    return get_parameter(GL_RENDERBUFFER_INTERNAL_FORMAT);
  }
  GLsizei samples        () const
  {
    return get_parameter(GL_RENDERBUFFER_SAMPLES);
  }
  GLsizei red_size       () const
  {
    return get_parameter(GL_RENDERBUFFER_RED_SIZE);
  }
  GLsizei green_size     () const
  {
    return get_parameter(GL_RENDERBUFFER_GREEN_SIZE);
  }
  GLsizei blue_size      () const
  {
    return get_parameter(GL_RENDERBUFFER_BLUE_SIZE);
  }
  GLsizei alpha_size     () const
  {
    return get_parameter(GL_RENDERBUFFER_ALPHA_SIZE);
  }
  GLsizei depth_size     () const
  {
    return get_parameter(GL_RENDERBUFFER_DEPTH_SIZE);
  }
  GLsizei stencil_size   () const
  {
    return get_parameter(GL_RENDERBUFFER_STENCIL_SIZE);
  }

  // 18.3 Copying pixels.
  template<GLenum source_target>
  void copy_image_sub_data(const texture<source_target>& source,
    GLint source_level, GLint source_x, GLint source_y, GLint source_z,
    GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth)
  {
    glCopyImageSubData(source.id(), source_target, source_level, source_x, source_y, source_z, id_, GL_RENDERBUFFER, level, x, y, z, width, height, depth);
  }
  void copy_image_sub_data(const renderbuffer& source,
    GLint source_level, GLint source_x, GLint source_y, GLint source_z,
    GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth)
  {
    glCopyImageSubData(source.id(), GL_RENDERBUFFER, source_level, source_x, source_y, source_z, id_, GL_RENDERBUFFER, level, x, y, z, width, height, depth);
  }
  
  // 22.3 Internal format queries.
  static GLint   internal_format_info   (GLenum internal_format, GLenum parameter)
  {
    GLint result;
    glGetInternalformativ(GL_RENDERBUFFER, internal_format, parameter, 1, &result);
    return result;
  }
  static GLint64 internal_format_info_64(GLenum internal_format, GLenum parameter)
  {
    GLint64 result;
    glGetInternalformati64v(GL_RENDERBUFFER, internal_format, parameter, 1, &result);
    return result;
  }

  static const GLenum native_type = GL_RENDERBUFFER;

  GLuint id() const
  {
    return id_;
  }

protected:
  GLint get_parameter(GLenum parameter) const
  {
    GLint result;
    glGetNamedRenderbufferParameteriv(id_, parameter, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};
}

#endif
