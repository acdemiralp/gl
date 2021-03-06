//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_FRAMEBUFFER_HPP
#define GL_FRAMEBUFFER_HPP

#include <array>
#include <cassert>
#include <utility>
#include <vector>

#include <gl/texture.hpp>
#include <gl/opengl.hpp>

namespace gl
{
class framebuffer
{
public:
  // 9.0 Framebuffer objects.
  framebuffer()
  {
    glCreateFramebuffers(1, &id_);
  }
  explicit framebuffer(GLuint id) : id_(id), managed_(false)
  {

  }
  framebuffer(const framebuffer&  that) = delete;
  framebuffer(      framebuffer&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
  {
    temp.id_ = invalid_id;
    temp.managed_ = false;
  }
  virtual ~framebuffer()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteFramebuffers(1, &id_);
  }
  framebuffer& operator=(const framebuffer&  that) = delete;
  framebuffer& operator=(      framebuffer&& temp) noexcept
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
  void        bind    (GLenum target = GL_FRAMEBUFFER) const
  {
    glBindFramebuffer(target, id_);
  }
  static void unbind  (GLenum target = GL_FRAMEBUFFER)
  {
    glBindFramebuffer(target, 0);
  }
  bool        is_valid() const
  {
    return glIsFramebuffer(id_) != 0;
  }

  // 9.2.1 Framebuffer object parameters (bindless).
  void set_default_width                 (GLsizei width  )
  {
    assert(width <= GL_MAX_FRAMEBUFFER_WIDTH);
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
  }
  void set_default_height                (GLsizei height )
  {
    assert(height <= GL_MAX_FRAMEBUFFER_HEIGHT);
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);
  }
  void set_default_fixed_sample_locations(bool    fixed  )
  {
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS, fixed);
  }
  void set_default_samples               (GLsizei samples)
  {
    assert(samples <= GL_MAX_FRAMEBUFFER_SAMPLES);
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_SAMPLES, samples);
  }
  void set_default_layers                (GLsizei layers )
  {
    assert(layers <= GL_MAX_FRAMEBUFFER_LAYERS);
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_LAYERS, layers);
  }
  
  // 9.2.3 Framebuffer object queries (bindless).
  GLsizei default_width                 () const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_WIDTH);
  }
  GLsizei default_height                () const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_HEIGHT);
  }
  bool    default_fixed_sample_locations() const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS) != 0;
  }
  GLsizei default_samples               () const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_SAMPLES);
  }
  GLsizei default_layers                () const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_LAYERS);
  }
  bool    doublebuffer                  () const
  {
    return get_parameter(GL_DOUBLEBUFFER) != 0;
  }
  GLsizei samples                       () const
  {
    return get_parameter(GL_SAMPLES);
  }
  GLsizei sample_buffers                () const
  {
    return get_parameter(GL_SAMPLE_BUFFERS);
  }
  bool    stereo                        () const
  {
    return get_parameter(GL_STEREO) != 0;
  }
  GLenum  color_read_format             () const
  {
    return get_parameter(GL_IMPLEMENTATION_COLOR_READ_FORMAT);
  }
  GLenum  color_read_type               () const
  {
    return get_parameter(GL_IMPLEMENTATION_COLOR_READ_TYPE);
  }
  
  GLenum  object_type          (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
  }
  GLuint  object_name          (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
  }
  GLenum  component_type       (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE);
  }
  GLsizei red_size             (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE);
  }
  GLsizei green_size           (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE);
  }
  GLsizei blue_size            (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE);
  }
  GLsizei alpha_size           (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE);
  }
  GLsizei depth_size           (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
  }
  GLsizei stencil_size         (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE);
  }
  GLenum  color_encoding       (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING);
  }
  GLuint  texture_layer        (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER);
  }
  GLuint  texture_level        (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
  }
  bool    layered              (GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_LAYERED) != 0;
  }
  GLenum  texture_cube_map_face(GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);
  }
  
  // 9.2.7 Attaching renderbuffer images (bindless).
  void attach_renderbuffer(GLenum attachment, const renderbuffer& renderbuffer)
  {
    glNamedFramebufferRenderbuffer(id_, attachment, GL_RENDERBUFFER, renderbuffer.id());
  }


  // 9.2.8 Attaching texture images (bindless).
  template<GLenum type>
  void attach_texture      (GLenum attachment, const texture<type>& texture, GLint level = 0)
  {
    glNamedFramebufferTexture(id_, attachment, texture.id(), level);
  }
  template<GLenum type>
  void attach_texture_layer(GLenum attachment, const texture<type>& texture, GLint level = 0, GLint layer = 0)
  {
    glNamedFramebufferTextureLayer(id_, attachment, texture.id(), level, layer);
  }

  // 9.4.2 Framebuffer completeness (bindless).
  GLenum status(GLenum target = GL_FRAMEBUFFER) const
  {
    return glCheckNamedFramebufferStatus(id_, target);
  }


  // 17.4.1 Selecting buffers for writing (bindless).
  void set_draw_buffer (GLenum buffer)
  {
    glNamedFramebufferDrawBuffer(id_, buffer);
  }
  void set_draw_buffers(const std::vector<GLenum>& buffers)
  {
    glNamedFramebufferDrawBuffers(id_, static_cast<GLsizei>(buffers.size()), buffers.data());
  }

  // 17.4.3 Clearing the buffers (bindless).
  void clear_color            (const std::array<GLfloat, 4>& color, GLint draw_buffer_index = 0) const
  {
    glClearNamedFramebufferfv(id_, GL_COLOR, draw_buffer_index, const_cast<GLfloat*>(color.data()));
  }
  void clear_color            (const std::array<GLint  , 4>& color, GLint draw_buffer_index = 0) const
  {
    glClearNamedFramebufferiv(id_, GL_COLOR, draw_buffer_index, const_cast<GLint*>(color.data()));
  }
  void clear_color            (const std::array<GLuint , 4>& color, GLint draw_buffer_index = 0) const
  {
    glClearNamedFramebufferuiv(id_, GL_COLOR, draw_buffer_index, const_cast<GLuint*>(color.data()));
  }
  void clear_depth            (GLfloat depth   = 0.0F) const
  {
    glClearNamedFramebufferfv(id_, GL_DEPTH, 0, &depth);
  }
  void clear_stencil          (GLint   stencil = 0) const
  {
    glClearNamedFramebufferiv(id_, GL_STENCIL, 0, &stencil);
  }
  void clear_depth_and_stencil(GLfloat depth, GLint stencil) const
  {
    glClearNamedFramebufferfi(id_, GL_DEPTH_STENCIL, 0, depth, stencil);
  }

  // 17.4.4 Invalidate framebuffers (bindless).
  void invalidate_sub_data(const std::vector<GLenum>& attachments, GLint x, GLint y, GLsizei width, GLsizei height)
  {
    glInvalidateNamedFramebufferSubData(id_, static_cast<GLsizei>(attachments.size()), attachments.data(), x, y, width, height);
  }
  void invalidate         (const std::vector<GLenum>& attachments)
  {
    glInvalidateNamedFramebufferData(id_, static_cast<GLsizei>(attachments.size()), attachments.data());
  }

  // 18.2 Reading pixels.
  void set_read_buffer(GLenum mode)
  {
    glNamedFramebufferReadBuffer(id_, mode);
  }

  // 18.3 Copying pixels (bindless).
  void blit(const framebuffer& source, GLint source_x, GLint source_y, GLint source_w, GLint source_h, GLint x, GLint y, GLint w, GLint h, GLbitfield mask, GLenum filter)
  {
    glBlitNamedFramebuffer(source.id(), id_, source_x, source_y, source_w, source_h, x, y, w, h, mask, filter);
  }

  // Extended functionality.
  bool is_complete() const
  {
    return status() == GL_FRAMEBUFFER_COMPLETE;
  }

  static const GLenum native_type = GL_FRAMEBUFFER;

  GLuint id() const
  {
    return id_;
  }

protected:
  GLint get_parameter           (                   GLenum parameter) const
  {
    GLint result;
    glGetNamedFramebufferParameteriv(id_, parameter, &result);
    return result;
  }
  GLint get_attachment_parameter(GLenum attachment, GLenum parameter) const
  {
    GLint result;
    glGetNamedFramebufferAttachmentParameteriv(id_, attachment, parameter, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};

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
void set_stencil_mask(GLuint mask , GLenum face = GL_FRONT_AND_BACK)
{
  glStencilMaskSeparate(face, mask);
}

// 17.4.3 Clearing the buffers.
void set_clear_color  (const std::array<GLfloat, 4>& color)
{
  glClearColor(color[0], color[1], color[2], color[3]);
}
void set_clear_depth  (GLdouble   depth  )
{
  glClearDepth(depth);
}
void set_clear_depth  (GLfloat    depth  )
{
  glClearDepthf(depth);
}
void set_clear_stencil(GLint      stencil)
{
  glClearStencil(stencil);
}
void clear            (GLbitfield buffer_bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
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
void clamp_read_color(GLenum clamp = GL_FIXED_ONLY)
{
  glClampColor(GL_CLAMP_READ_COLOR, clamp);
}
}

#endif
