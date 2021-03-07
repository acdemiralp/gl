//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_FRAMEBUFFER_HPP
#define GL_FRAMEBUFFER_HPP

#include <array>
#include <cassert>
#include <vector>

#include <gl/texture.hpp>
#include <gl/opengl.hpp>

namespace gl
{
class framebuffer
{
public:
  // 9.0 Framebuffer objects.
  framebuffer           ()
  {
    glCreateFramebuffers(1, &id_);
  }
  explicit framebuffer  (const GLuint id) : id_(id), managed_(false)
  {

  }
  framebuffer           (const framebuffer&  that) = delete;
  framebuffer           (      framebuffer&& temp) noexcept : id_(temp.id_), managed_(temp.managed_)
  {
    temp.id_      = invalid_id;
    temp.managed_ = false;
  }
  virtual ~framebuffer  ()
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
  
      id_      = temp.id_;
      managed_ = temp.managed_;
  
      temp.id_      = invalid_id;
      temp.managed_ = false;    
    }
    return *this;
  }

  // 9.2 Binding and managing.
  void        bind    (const GLenum target = GL_FRAMEBUFFER) const
  {
    glBindFramebuffer(target, id_);
  }
  static void unbind  (const GLenum target = GL_FRAMEBUFFER)
  {
    glBindFramebuffer(target, 0);
  }
  [[nodiscard]]
  bool        is_valid() const
  {
    return glIsFramebuffer(id_) != 0;
  }

  // 9.2.1 Framebuffer object parameters (bindless).
  void set_default_width                 (const GLsizei width  ) const
  {
    assert(width <= GL_MAX_FRAMEBUFFER_WIDTH);
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
  }
  void set_default_height                (const GLsizei height ) const
  {
    assert(height <= GL_MAX_FRAMEBUFFER_HEIGHT);
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);
  }
  void set_default_fixed_sample_locations(const bool    fixed  ) const
  {
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS, fixed);
  }
  void set_default_samples               (const GLsizei samples) const
  {
    assert(samples <= GL_MAX_FRAMEBUFFER_SAMPLES);
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_SAMPLES, samples);
  }
  void set_default_layers                (const GLsizei layers ) const
  {
    assert(layers <= GL_MAX_FRAMEBUFFER_LAYERS);
    glNamedFramebufferParameteri(id_, GL_FRAMEBUFFER_DEFAULT_LAYERS, layers);
  }
  
  // 9.2.3 Framebuffer object queries (bindless).
  [[nodiscard]]
  GLsizei default_width                 () const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_WIDTH);
  }
  [[nodiscard]]
  GLsizei default_height                () const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_HEIGHT);
  }
  [[nodiscard]]
  bool    default_fixed_sample_locations() const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS) != 0;
  }
  [[nodiscard]]
  GLsizei default_samples               () const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_SAMPLES);
  }
  [[nodiscard]]
  GLsizei default_layers                () const
  {
    return get_parameter(GL_FRAMEBUFFER_DEFAULT_LAYERS);
  }
  [[nodiscard]]
  bool    doublebuffer                  () const
  {
    return get_parameter(GL_DOUBLEBUFFER) != 0;
  }
  [[nodiscard]]
  GLsizei samples                       () const
  {
    return get_parameter(GL_SAMPLES);
  }
  [[nodiscard]]
  GLsizei sample_buffers                () const
  {
    return get_parameter(GL_SAMPLE_BUFFERS);
  }
  [[nodiscard]]
  bool    stereo                        () const
  {
    return get_parameter(GL_STEREO) != 0;
  }
  [[nodiscard]]
  GLenum  color_read_format             () const
  {
    return get_parameter(GL_IMPLEMENTATION_COLOR_READ_FORMAT);
  }
  [[nodiscard]]
  GLenum  color_read_type               () const
  {
    return get_parameter(GL_IMPLEMENTATION_COLOR_READ_TYPE);
  }

  [[nodiscard]]
  GLenum  object_type          (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
  }
  [[nodiscard]]
  GLuint  object_name          (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
  }
  [[nodiscard]]
  GLenum  component_type       (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE);
  }
  [[nodiscard]]
  GLsizei red_size             (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE);
  }
  [[nodiscard]]
  GLsizei green_size           (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE);
  }
  [[nodiscard]]
  GLsizei blue_size            (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE);
  }
  [[nodiscard]]
  GLsizei alpha_size           (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE);
  }
  [[nodiscard]]
  GLsizei depth_size           (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
  }
  [[nodiscard]]
  GLsizei stencil_size         (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE);
  }
  [[nodiscard]]
  GLenum  color_encoding       (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING);
  }
  [[nodiscard]]
  GLuint  texture_layer        (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER);
  }
  [[nodiscard]]
  GLuint  texture_level        (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
  }
  [[nodiscard]]
  bool    layered              (const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_LAYERED) != 0;
  }
  [[nodiscard]]
  GLenum  texture_cube_map_face(const GLenum attachment) const
  {
    return get_attachment_parameter(attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);
  }
  
  // 9.2.7 Attaching renderbuffer images (bindless).
  void attach_renderbuffer(const GLenum attachment, const renderbuffer& renderbuffer) const
  {
    glNamedFramebufferRenderbuffer(id_, attachment, GL_RENDERBUFFER, renderbuffer.id());
  }

  // 9.2.8 Attaching texture images (bindless).
  template<GLenum type>
  void attach_texture      (const GLenum attachment, const texture<type>& texture, const GLint level = 0)
  {
    glNamedFramebufferTexture(id_, attachment, texture.id(), level);
  }
  template<GLenum type>
  void attach_texture_layer(const GLenum attachment, const texture<type>& texture, const GLint level = 0, const GLint layer = 0)
  {
    glNamedFramebufferTextureLayer(id_, attachment, texture.id(), level, layer);
  }

  // 9.4.2 Framebuffer completeness (bindless).
  [[nodiscard]]
  GLenum status(const GLenum target = GL_FRAMEBUFFER) const
  {
    return glCheckNamedFramebufferStatus(id_, target);
  }


  // 17.4.1 Selecting buffers for writing (bindless).
  void set_draw_buffer (const GLenum buffer) const
  {
    glNamedFramebufferDrawBuffer(id_, buffer);
  }
  void set_draw_buffers(const std::vector<GLenum>& buffers) const
  {
    glNamedFramebufferDrawBuffers(id_, static_cast<GLsizei>(buffers.size()), buffers.data());
  }

  // 17.4.3 Clearing the buffers (bindless).
  void clear_color            (const std::array<GLfloat, 4>& color, const GLint draw_buffer_index = 0) const
  {
    glClearNamedFramebufferfv(id_, GL_COLOR, draw_buffer_index, const_cast<GLfloat*>(color.data()));
  }
  void clear_color            (const std::array<GLint  , 4>& color, const GLint draw_buffer_index = 0) const
  {
    glClearNamedFramebufferiv(id_, GL_COLOR, draw_buffer_index, const_cast<GLint*>(color.data()));
  }
  void clear_color            (const std::array<GLuint , 4>& color, const GLint draw_buffer_index = 0) const
  {
    glClearNamedFramebufferuiv(id_, GL_COLOR, draw_buffer_index, const_cast<GLuint*>(color.data()));
  }
  void clear_depth            (GLfloat depth   = 0.0F) const
  {
    glClearNamedFramebufferfv(id_, GL_DEPTH, 0, &depth);
  }
  void clear_stencil          (const GLint   stencil = 0) const
  {
    glClearNamedFramebufferiv(id_, GL_STENCIL, 0, &stencil);
  }
  void clear_depth_and_stencil(const GLfloat depth, const GLint stencil) const
  {
    glClearNamedFramebufferfi(id_, GL_DEPTH_STENCIL, 0, depth, stencil);
  }

  // 17.4.4 Invalidate framebuffers (bindless).
  void invalidate_sub_data(const std::vector<GLenum>& attachments, const GLint x, const GLint y, const GLsizei width, const GLsizei height) const
  {
    glInvalidateNamedFramebufferSubData(id_, static_cast<GLsizei>(attachments.size()), attachments.data(), x, y, width, height);
  }
  void invalidate         (const std::vector<GLenum>& attachments) const
  {
    glInvalidateNamedFramebufferData(id_, static_cast<GLsizei>(attachments.size()), attachments.data());
  }

  // 18.2 Reading pixels.
  void set_read_buffer(const GLenum mode) const
  {
    glNamedFramebufferReadBuffer(id_, mode);
  }

  // 18.3 Copying pixels (bindless).
  void blit(const framebuffer& source, const GLint source_x, const GLint source_y, const GLint source_w, const GLint source_h, const GLint x, const GLint y, const GLint w, const GLint h, const GLbitfield mask, const GLenum filter) const
  {
    glBlitNamedFramebuffer(source.id(), id_, source_x, source_y, source_w, source_h, x, y, w, h, mask, filter);
  }

  // Extended functionality.
  [[nodiscard]]
  bool is_complete() const
  {
    return status() == GL_FRAMEBUFFER_COMPLETE;
  }

  static const GLenum native_type = GL_FRAMEBUFFER;

  [[nodiscard]]
  GLuint id() const
  {
    return id_;
  }

protected:
  [[nodiscard]]
  GLint get_parameter           (                         const GLenum parameter) const
  {
    GLint result;
    glGetNamedFramebufferParameteriv(id_, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint get_attachment_parameter(const GLenum attachment, const GLenum parameter) const
  {
    GLint result;
    glGetNamedFramebufferAttachmentParameteriv(id_, attachment, parameter, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};

// 17.4.2 Fine control of buffer updates.
inline void set_color_mask  (const std::array<bool, 4>& mask)
{
  glColorMask(mask[0], mask[1], mask[2], mask[3]);
}
inline void set_color_mask  (const GLuint index, const std::array<bool, 4>& mask)
{
  glColorMaski(index, mask[0], mask[1], mask[2], mask[3]);
}
inline void set_depth_mask  (const bool   mask )
{
  glDepthMask(mask);
}
inline void set_stencil_mask(const GLuint mask , const GLenum face = GL_FRONT_AND_BACK)
{
  glStencilMaskSeparate(face, mask);
}

// 17.4.3 Clearing the buffers.
inline void set_clear_color  (const std::array<GLfloat, 4>& color)
{
  glClearColor(color[0], color[1], color[2], color[3]);
}
inline void set_clear_depth  (const GLdouble   depth  )
{
  glClearDepth(depth);
}
inline void set_clear_depth  (const GLfloat    depth  )
{
  glClearDepthf(depth);
}
inline void set_clear_stencil(const GLint      stencil)
{
  glClearStencil(stencil);
}
inline void clear            (const GLbitfield buffer_bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
{
  glClear(buffer_bits);
}

// 18.2 Reading pixels.
inline std::vector<GLuint> read_pixels(const GLint x, const GLint y, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type)
{
  std::vector<GLuint> pixels(width * height * format_component_count(format) * type_size(type));
  glReadPixels(x, y, width, height, format, type, pixels.data());
  return pixels;
}

// 18.2.8 Final conversion.
inline void clamp_read_color(const GLenum clamp = GL_FIXED_ONLY)
{
  glClampColor(GL_CLAMP_READ_COLOR, clamp);
}
}

#endif
