//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/renderbuffer.hpp>

namespace gl
{
// 9.2.4 Renderbuffer objects.
renderbuffer::renderbuffer()
{
  glCreateRenderbuffers(1, &id_);
}
renderbuffer::renderbuffer(GLuint id): id_(id), managed_(false)
{

}
renderbuffer::renderbuffer(const renderbuffer& that): renderbuffer()
{
  set_storage_multisample(that.samples(), that.internal_format(), that.width(), that.height());
}
renderbuffer::~renderbuffer()
{
  if (managed_)
    glDeleteRenderbuffers(1, &id_);
}
renderbuffer& renderbuffer::operator=(const renderbuffer& that)
{
  set_storage_multisample(that.samples(), that.internal_format(), that.width(), that.height());
  return *this;
}

void renderbuffer::bind    () const
{
  glBindRenderbuffer(GL_RENDERBUFFER, id_);
}
void renderbuffer::unbind  ()
{
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
bool renderbuffer::is_valid() const
{
  return glIsRenderbuffer(id_) != 0;
}

void renderbuffer::set_storage            (                 GLenum internal_format, GLsizei width, GLsizei height)
{
  glNamedRenderbufferStorage(id_, internal_format, width, height);
}
void renderbuffer::set_storage_multisample(GLsizei samples, GLenum internal_format, GLsizei width, GLsizei height)
{
  glNamedRenderbufferStorageMultisample(id_, samples, internal_format, width, height);
}

// 9.2.6 Renderbuffer object queries (bindless).
GLsizei renderbuffer::width          () const
{
  return get_parameter(GL_RENDERBUFFER_WIDTH);
}
GLsizei renderbuffer::height         () const
{
  return get_parameter(GL_RENDERBUFFER_HEIGHT);
}
GLenum  renderbuffer::internal_format() const
{
  return get_parameter(GL_RENDERBUFFER_INTERNAL_FORMAT);
}
GLsizei renderbuffer::samples        () const
{
  return get_parameter(GL_RENDERBUFFER_SAMPLES);
}
GLsizei renderbuffer::red_size       () const
{
  return get_parameter(GL_RENDERBUFFER_RED_SIZE);
}
GLsizei renderbuffer::green_size     () const
{
  return get_parameter(GL_RENDERBUFFER_GREEN_SIZE);
}
GLsizei renderbuffer::blue_size      () const
{
  return get_parameter(GL_RENDERBUFFER_BLUE_SIZE);
}
GLsizei renderbuffer::alpha_size     () const
{
  return get_parameter(GL_RENDERBUFFER_ALPHA_SIZE);
}
GLsizei renderbuffer::depth_size     () const
{
  return get_parameter(GL_RENDERBUFFER_DEPTH_SIZE);
}
GLsizei renderbuffer::stencil_size   () const
{
  return get_parameter(GL_RENDERBUFFER_STENCIL_SIZE);
}

void renderbuffer::copy_image_sub_data(const renderbuffer& source, 
  GLint source_level, GLint source_x, GLint source_y, GLint source_z, 
  GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth)
{
  glCopyImageSubData(source.id(), GL_RENDERBUFFER, source_level, source_x, source_y, source_z, id_, GL_RENDERBUFFER, level, x, y, z, width, height, depth);
}

GLint   renderbuffer::internal_format_info   (GLenum internal_format, GLenum parameter)
{
  GLint result;
  glGetInternalformativ(GL_RENDERBUFFER, internal_format, parameter, 1, &result);
  return result;
}
GLint64 renderbuffer::internal_format_info_64(GLenum internal_format, GLenum parameter)
{
  GLint64 result;
  glGetInternalformati64v(GL_RENDERBUFFER, internal_format, parameter, 1, &result);
  return result;
}

GLuint renderbuffer::id() const
{
  return id_;
}

GLint renderbuffer::get_parameter(GLenum parameter) const
{
  GLint result;
  glGetNamedRenderbufferParameteriv(id_, parameter, &result);
  return result;
}
}