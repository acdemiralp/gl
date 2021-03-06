//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_TEXTURE_VIEW_HPP
#define GL_TEXTURE_VIEW_HPP

#include <gl/opengl.hpp>
#include <gl/texture.hpp>

namespace gl
{
template<GLenum target>
class texture_view : public texture<target>
{
public:
  // 8.18 Texture views.
  texture_view(const texture<target>& original, GLenum internal_format, GLuint min_level, GLuint num_levels, GLuint min_layer, GLuint num_layers) : texture<target>()
  {
    glTextureView(texture<target>::id_, target, original.id(), internal_format, min_level, num_levels, min_layer, num_layers);
  }
  texture_view(const texture_view&  that) = delete ;
  texture_view(      texture_view&& temp) = default;
  virtual ~texture_view()                 = default;

  texture_view& operator=(const texture_view&  that) = delete ;
  texture_view& operator=(      texture_view&& temp) = default;

  // 8.11 Texture queries.
  GLint   min_level       () const
  {
    return texture<target>::get_int_parameter(GL_TEXTURE_VIEW_MIN_LEVEL);
  }
  GLsizei num_levels      () const
  {
    return texture<target>::get_int_parameter(GL_TEXTURE_VIEW_NUM_LEVELS);
  }
  GLint   min_layer       () const
  {
    return texture<target>::get_int_parameter(GL_TEXTURE_VIEW_MIN_LAYER);
  }
  GLsizei num_layers      () const
  {
    return texture<target>::get_int_parameter(GL_TEXTURE_VIEW_NUM_LAYERS);
  }
  GLsizei immutable_levels() const
  {
    return texture<target>::get_int_parameter(GL_TEXTURE_IMMUTABLE_LEVELS);
  }
};
}

#endif
