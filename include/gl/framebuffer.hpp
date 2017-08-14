//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_FRAMEBUFFER_HPP_
#define GL_FRAMEBUFFER_HPP_

#include <array>
#include <vector>

#include <gl/export.hpp>
#include <gl/texture.hpp>
#include <gl/opengl.hpp>

namespace gl
{
class GL_EXPORT framebuffer
{
public:
  // 9.0 Framebuffer objects.
  framebuffer();
  framebuffer(GLuint id);
  framebuffer(const framebuffer&  that) = delete;
  framebuffer(      framebuffer&& temp) noexcept;
  virtual ~framebuffer();
  framebuffer& operator=(const framebuffer&  that) = delete;
  framebuffer& operator=(      framebuffer&& temp) noexcept;

  // 9.2 Binding and managing.
  template<GLenum target = GL_FRAMEBUFFER>
  void        bind    () const;
  template<GLenum target = GL_FRAMEBUFFER>
  static void unbind  ();
  bool        is_valid() const;

  // 9.2.1 Framebuffer object parameters (bindless).
  void set_default_width                 (GLsizei width  );
  void set_default_height                (GLsizei height );
  void set_default_fixed_sample_locations(bool    fixed  );
  void set_default_samples               (GLsizei samples);
  void set_default_layers                (GLsizei layers );

  // 9.2.3 Framebuffer object queries (bindless).
  GLsizei default_width                 () const;
  GLsizei default_height                () const;
  bool    default_fixed_sample_locations() const;
  GLsizei default_samples               () const;
  GLsizei default_layers                () const;
  bool    doublebuffer                  () const;
  GLsizei samples                       () const;
  GLsizei sample_buffers                () const;
  bool    stereo                        () const;
  GLenum  color_read_format             () const;
  GLenum  color_read_type               () const;

  GLenum  object_type          (GLenum attachment) const;
  GLuint  object_name          (GLenum attachment) const;
  GLenum  component_type       (GLenum attachment) const;
  GLsizei red_size             (GLenum attachment) const;
  GLsizei green_size           (GLenum attachment) const;
  GLsizei blue_size            (GLenum attachment) const;
  GLsizei alpha_size           (GLenum attachment) const;
  GLsizei depth_size           (GLenum attachment) const;
  GLsizei stencil_size         (GLenum attachment) const;
  GLenum  color_encoding       (GLenum attachment) const;
  GLuint  texture_layer        (GLenum attachment) const;
  GLuint  texture_level        (GLenum attachment) const;
  bool    layered              (GLenum attachment) const;
  GLenum  texture_cube_map_face(GLenum attachment) const;

  // 9.2.7 Attaching renderbuffer images (bindless).
  void attach_renderbuffer(GLenum attachment, const renderbuffer& renderbuffer);

  // 9.2.8 Attaching texture images (bindless).
  template<GLenum type>
  void attach_texture      (GLenum attachment, const texture<type>& texture, GLint level = 0);
  template<GLenum type>
  void attach_texture_layer(GLenum attachment, const texture<type>& texture, GLint level = 0, GLint layer = 0);

  // 9.4.2 Framebuffer completeness (bindless).
  template<GLenum target = GL_FRAMEBUFFER>
  GLenum status() const;

  // 17.4.1 Selecting buffers for writing (bindless).
  void set_draw_buffer (GLenum buffer);
  void set_draw_buffers(const std::vector<GLenum>& buffers);

  // 17.4.3 Clearing the buffers (bindless).
  void clear_color            (const std::array<GLfloat, 4>& color, GLint draw_buffer_index = 0) const;
  void clear_color            (const std::array<GLint  , 4>& color, GLint draw_buffer_index = 0) const;
  void clear_color            (const std::array<GLuint , 4>& color, GLint draw_buffer_index = 0) const;
  void clear_depth            (GLfloat depth   = 0.0F) const;
  void clear_stencil          (GLint   stencil = 0) const;
  void clear_depth_and_stencil(GLfloat depth, GLint stencil) const;

  // 17.4.4 Invalidate framebuffers (bindless).
  void invalidate_sub_data(const std::vector<GLenum>& attachments, GLint x, GLint y, GLsizei width, GLsizei height);
  void invalidate         (const std::vector<GLenum>& attachments);

  // 18.2 Reading pixels.
  void set_read_buffer(GLenum mode);

  // 18.3 Copying pixels (bindless).
  void blit(const framebuffer& source, GLint source_x, GLint source_y, GLint source_w, GLint source_h, GLint x, GLint y, GLint w, GLint h, GLbitfield mask, GLenum filter);

  // Extended functionality.
  bool is_complete() const;

  static const GLenum native_type = GL_FRAMEBUFFER;

  GLuint id() const;

protected:
  GLint get_parameter           (                   GLenum parameter) const;
  GLint get_attachment_parameter(GLenum attachment, GLenum parameter) const;

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};

// 9.2 Binding and managing.
template <GLenum target>
void framebuffer::bind  () const
{
  glBindFramebuffer(target, id_);
}
template <GLenum target>
void framebuffer::unbind()
{
  glBindFramebuffer(target, 0);
}

// 9.2.8 Attaching texture images (bindless).
template <GLenum type>
void framebuffer::attach_texture      (GLenum attachment, const texture<type>& texture, GLint level)
{
  glNamedFramebufferTexture(id_, attachment, texture.id(), level);
}
template <GLenum type>
void framebuffer::attach_texture_layer(GLenum attachment, const texture<type>& texture, GLint level, GLint layer)
{
  glNamedFramebufferTextureLayer(id_, attachment, texture.id(), level, layer);
}

// 9.4.2 Framebuffer completeness (bindless).
template <GLenum target>
GLenum framebuffer::status() const
{
  return glCheckNamedFramebufferStatus(id_, target);
}

// 17.4.2 Fine control of buffer updates.
GL_EXPORT void set_color_mask  (const std::array<bool, 4>& mask);
GL_EXPORT void set_color_mask  (GLuint index, const std::array<bool, 4>& mask);
GL_EXPORT void set_depth_mask  (bool   mask );
GL_EXPORT void set_stencil_mask(GLuint mask , GLenum face = GL_FRONT_AND_BACK);

// 17.4.3 Clearing the buffers.
GL_EXPORT void set_clear_color  (const std::array<float, 4>& color);
GL_EXPORT void set_clear_depth  (GLdouble   depth  );
GL_EXPORT void set_clear_depth  (GLfloat    depth  );
GL_EXPORT void set_clear_stencil(GLint      stencil);
GL_EXPORT void clear            (GLbitfield buffer_bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 18.2 Reading pixels.
GL_EXPORT std::vector<GLuint> read_pixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type);

// 18.2.8 Final conversion.
GL_EXPORT void clamp_read_color(GLenum clamp = GL_FIXED_ONLY);
}

#endif
