//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/framebuffer.hpp>

#include <cassert>
#include <utility>

namespace gl
{
// 9.0 Framebuffer objects.
framebuffer::framebuffer ()
{
  glCreateFramebuffers(1, &id_);
}
framebuffer::framebuffer (GLuint id): id_(id), managed_(false)
{

}
framebuffer::framebuffer (framebuffer&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
{
  temp.id_      = invalid_id;
  temp.managed_ = false;
}
framebuffer::~framebuffer()
{
  if (managed_ && id_ != invalid_id)
    glDeleteFramebuffers(1, &id_);
}

framebuffer& framebuffer::operator=(framebuffer&& temp) noexcept
{
  if (this != &temp)
  {
    if (managed_ && id_ != invalid_id)
      glDeleteFramebuffers(1, &id_);

    id_      = std::move(temp.id_);
    managed_ = std::move(temp.managed_);

    temp.id_      = invalid_id;
    temp.managed_ = false;    
  }
  return *this;
}

// 9.2 Binding and managing.
void framebuffer::bind  (GLenum target) const
{
  glBindFramebuffer(target, id_);
}
void framebuffer::unbind(GLenum target)
{
  glBindFramebuffer(target, 0);
}

bool framebuffer::is_valid() const
{
  return glIsFramebuffer(id_) != 0;
}

// 9.2.1 Framebuffer object parameters (bindless).
void framebuffer::set_default_width                 (GLsizei width  )
{
  assert(width <= GL_MAX_FRAMEBUFFER_WIDTH);
  glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
}
void framebuffer::set_default_height                (GLsizei height )
{
  assert(height <= GL_MAX_FRAMEBUFFER_HEIGHT);
  glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);
}
void framebuffer::set_default_fixed_sample_locations(bool    fixed  )
{
  glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS, fixed);
}
void framebuffer::set_default_samples               (GLsizei samples)
{
  assert(samples <= GL_MAX_FRAMEBUFFER_SAMPLES);
  glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_SAMPLES, samples);
}
void framebuffer::set_default_layers                (GLsizei layers )
{
  assert(layers <= GL_MAX_FRAMEBUFFER_LAYERS);
  glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_LAYERS, layers);
}

// 9.2.3 Framebuffer object queries (bindless).
GLsizei framebuffer::default_width                 () const
{
  return get_parameter(GL_FRAMEBUFFER_DEFAULT_WIDTH);
}
GLsizei framebuffer::default_height                () const
{
  return get_parameter(GL_FRAMEBUFFER_DEFAULT_HEIGHT);
}
bool    framebuffer::default_fixed_sample_locations() const
{
  return get_parameter(GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS) != 0;
}
GLsizei framebuffer::default_samples               () const
{
  return get_parameter(GL_FRAMEBUFFER_DEFAULT_SAMPLES);
}
GLsizei framebuffer::default_layers                () const
{
  return get_parameter(GL_FRAMEBUFFER_DEFAULT_LAYERS);
}
bool    framebuffer::doublebuffer                  () const
{
  return get_parameter(GL_DOUBLEBUFFER) != 0;
}
GLsizei framebuffer::samples                       () const
{
  return get_parameter(GL_SAMPLES);
}
GLsizei framebuffer::sample_buffers                () const
{
  return get_parameter(GL_SAMPLE_BUFFERS);
}
bool    framebuffer::stereo                        () const
{
  return get_parameter(GL_STEREO) != 0;
}
GLenum  framebuffer::color_read_format             () const
{
  return get_parameter(GL_IMPLEMENTATION_COLOR_READ_FORMAT);
}
GLenum  framebuffer::color_read_type               () const
{
  return get_parameter(GL_IMPLEMENTATION_COLOR_READ_TYPE);
}

GLenum  framebuffer::object_type          (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
}
GLuint  framebuffer::object_name          (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
}
GLenum  framebuffer::component_type       (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE);
}
GLsizei framebuffer::red_size             (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE);
}
GLsizei framebuffer::green_size           (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE);
}
GLsizei framebuffer::blue_size            (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE);
}
GLsizei framebuffer::alpha_size           (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE);
}
GLsizei framebuffer::depth_size           (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
}
GLsizei framebuffer::stencil_size         (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE);
}
GLenum  framebuffer::color_encoding       (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING);
}
GLuint  framebuffer::texture_layer        (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER);
}
GLuint  framebuffer::texture_level        (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
}
bool    framebuffer::layered              (GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_LAYERED) != 0;
}
GLenum  framebuffer::texture_cube_map_face(GLenum attachment) const
{
  return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);
}

// 9.2.7 Attaching renderbuffer images (bindless).
void framebuffer::attach_renderbuffer(GLenum attachment, const renderbuffer& renderbuffer)
{
  glNamedFramebufferRenderbuffer(id_, attachment, GL_RENDERBUFFER, renderbuffer.id());
}

// 9.4.2 Framebuffer completeness (bindless).
GLenum framebuffer::status(GLenum target) const
{
  return glCheckNamedFramebufferStatus(id_, target);
}

// 17.4.1 Selecting buffers for writing (bindless).
void framebuffer::set_draw_buffer (GLenum buffer)
{
  glNamedFramebufferDrawBuffer(id_, buffer);
}
void framebuffer::set_draw_buffers(const std::vector<GLenum>& buffers)
{
  glNamedFramebufferDrawBuffers(id_, static_cast<GLsizei>(buffers.size()), buffers.data());
}

// 17.4.3 Clearing the buffers (bindless).
void framebuffer::clear_color            (const std::array<GLfloat, 4>& color, GLint draw_buffer_index) const
{
  glClearNamedFramebufferfv(id_, GL_COLOR, draw_buffer_index, const_cast<GLfloat*>(color.data()));
}
void framebuffer::clear_color            (const std::array<GLint  , 4>& color, GLint draw_buffer_index) const
{
  glClearNamedFramebufferiv(id_, GL_COLOR, draw_buffer_index, const_cast<GLint*>(color.data()));
}
void framebuffer::clear_color            (const std::array<GLuint , 4>& color, GLint draw_buffer_index) const
{
  glClearNamedFramebufferuiv(id_, GL_COLOR, draw_buffer_index, const_cast<GLuint*>(color.data()));
}
void framebuffer::clear_depth            (GLfloat depth  ) const
{
  glClearNamedFramebufferfv(id_, GL_DEPTH, 0, &depth);
}
void framebuffer::clear_stencil          (GLint   stencil) const
{
  glClearNamedFramebufferiv(id_, GL_STENCIL, 0, &stencil);
}
void framebuffer::clear_depth_and_stencil(GLfloat depth, GLint stencil) const
{
  glClearNamedFramebufferfi(id_, GL_DEPTH_STENCIL, 0, depth, stencil);
}

// 17.4.4 Invalidate framebuffers (bindless).
void framebuffer::invalidate_sub_data(const std::vector<GLenum>& attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
  glInvalidateNamedFramebufferSubData(id_, static_cast<GLsizei>(attachments.size()), attachments.data(), x, y, width, height);
}
void framebuffer::invalidate         (const std::vector<GLenum>& attachments)
{
  glInvalidateNamedFramebufferData(id_, static_cast<GLsizei>(attachments.size()), attachments.data());
}

// 18.2 Reading pixels.
void framebuffer::set_read_buffer(GLenum mode)
{
  glNamedFramebufferReadBuffer(id_, mode);
}

// 18.3 Copying pixels (bindless).
void framebuffer::blit(const framebuffer& source, GLint source_x, GLint source_y, GLint source_w, GLint source_h, GLint x, GLint y, GLint w, GLint h, GLbitfield mask, GLenum filter)
{
  glBlitNamedFramebuffer(source.id(), id_, source_x, source_y, source_w, source_h, x, y, w, h, mask, filter);
}

// Extended functionality.
bool framebuffer::is_complete() const
{
  return status() == GL_FRAMEBUFFER_COMPLETE;
}

GLuint framebuffer::id() const
{
  return id_;
}

GLint framebuffer::get_parameter           (                   GLenum parameter) const
{
  GLint result;
  glGetNamedFramebufferParameteriv(id_, parameter, &result);
  return result;
}
GLint framebuffer::get_attachment_parameter(GLenum attachment, GLenum parameter) const
{
  GLint result;
  glGetNamedFramebufferAttachmentParameteriv(id_, attachment, parameter, &result);
  return result;
}

// 17.4.2 Fine control of buffer updates.
void set_color_mask  (const std::array<bool, 4>& mask)
{
  glColorMask(mask[0], mask[1], mask[2], mask[3]);
}
void set_color_mask  (GLuint index, const std::array<bool, 4>& mask)
{
  glColorMaski(index, mask[0], mask[1], mask[2], mask[3]);
}
void set_depth_mask  (bool   mask )
{
  glDepthMask(mask);
}
void set_stencil_mask(GLuint mask , GLenum face)
{
  glStencilMaskSeparate(face, mask);
}

// 17.4.3 Clearing the buffers.
void set_clear_color  (const std::array<float, 4>& color)
{
  glClearColor(color[0], color[1], color[2], color[3]);
}
void set_clear_depth  (GLdouble   depth      )
{
  glClearDepth(depth);
}
void set_clear_depth  (GLfloat    depth      )
{
  glClearDepthf(depth);
}
void set_clear_stencil(GLint      stencil    )
{
  glClearStencil(stencil);
}
void clear            (GLbitfield buffer_bits)
{
  glClear(buffer_bits);
}

// 18.2 Reading pixels.
std::vector<GLuint> read_pixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type)
{
  std::vector<GLuint> pixels(width * height * format_component_count(format) * type_size(type));
  glReadPixels(x, y, width, height, format, type, pixels.data());
  return pixels;
}

// 18.2.8 Final conversion.
void clamp_read_color(GLenum clamp)
{
  glClampColor(GL_CLAMP_READ_COLOR, clamp);
}
}