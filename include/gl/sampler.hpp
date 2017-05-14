//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_SAMPLER_HPP_
#define GL_SAMPLER_HPP_

#include <array>
#include <cstddef>

#include <gl/opengl.hpp>

#include <export.hpp>

namespace gl
{
class GL_EXPORT sampler
{
public:
  // 8.2 Sampler objects.
  sampler();
  sampler(GLuint id);
  sampler(const sampler&  that);
  sampler(      sampler&& temp) = default;
  virtual ~sampler();
  sampler& operator=(const sampler&  that);
  sampler& operator=(      sampler&& temp) = default;

  void        bind    (GLuint unit) const;
  static void unbind  (GLuint unit);
  bool        is_valid() const;

  void set_wrap_s      (GLenum  mode );
  void set_wrap_t      (GLenum  mode );
  void set_wrap_r      (GLenum  mode );
  void set_min_filter  (GLenum  mode );
  void set_mag_filter  (GLenum  mode );
  void set_min_lod     (GLfloat value);
  void set_max_lod     (GLfloat value);
  void set_border_color(const std::array<GLint  , 4>& color);
  void set_border_color(const std::array<GLuint , 4>& color);
  void set_border_color(const std::array<GLfloat, 4>& color);
  void set_lod_bias    (GLfloat bias);
  void set_compare_mode(GLenum  mode);
  void set_compare_func(GLenum  function);

  // 8.3 Sampler queries.
  GLenum  wrap_s    () const;
  GLenum  wrap_t    () const;
  GLenum  wrap_r    () const;
  GLenum  min_filter() const;
  GLenum  mag_filter() const;
  GLfloat min_lod   () const;
  GLfloat max_lod   () const;
  std::array<GLfloat, 4> border_color    () const;
  std::array<GLint  , 4> border_color_int() const;
  GLfloat lod_bias    () const;
  GLenum  compare_mode() const;
  GLenum  compare_func() const;

  static const GLenum native_type = GL_SAMPLER;

  GLuint id() const;

protected:
  template<std::size_t count>
  std::array<GLint, count>   get_int_parameter  (GLenum parameter) const;
  GLint                      get_int_parameter  (GLenum parameter) const;
  template<std::size_t count>
  std::array<GLfloat, count> get_float_parameter(GLenum parameter) const;
  GLfloat                    get_float_parameter(GLenum parameter) const;

  GLuint id_;
  bool   managed_ = true;
};

template <std::size_t count>
std::array<GLint  , count> sampler::get_int_parameter  (GLenum parameter) const
{
  std::array<GLint, count> result;
  glGetSamplerParameteriv(id_, parameter, result.data());
  return result;
}
template <std::size_t count>
std::array<GLfloat, count> sampler::get_float_parameter(GLenum parameter) const
{
  std::array<GLfloat, count> result;
  glGetSamplerParameterfv(id_, parameter, result.data());
  return result;
}
}

#endif
