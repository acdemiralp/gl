//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_SAMPLER_HPP
#define GL_SAMPLER_HPP

#include <array>
#include <cstddef>

#include <gl/opengl.hpp>

namespace gl
{
class sampler
{
public:
  // 8.2 Sampler objects.
  sampler()
  {
    glCreateSamplers(1, &id_);
  }
  explicit sampler(GLuint id) : id_(id), managed_(false)
  {

  }
  sampler(const sampler&  that) : sampler()
  {
    set_wrap_s      (that.wrap_s      ());
    set_wrap_t      (that.wrap_t      ());
    set_wrap_r      (that.wrap_r      ());
    set_min_filter  (that.min_filter  ());
    set_mag_filter  (that.mag_filter  ());
    set_min_lod     (that.min_lod     ());
    set_max_lod     (that.max_lod     ());
    set_border_color(that.border_color());
    set_lod_bias    (that.lod_bias    ());
    set_compare_mode(that.compare_mode());
    set_compare_func(that.compare_func());
  }
  sampler(      sampler&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
  {
    temp.id_      = invalid_id;
    temp.managed_ = false;
  }
  virtual ~sampler()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteSamplers(1, &id_);
  }
  sampler& operator=(const sampler&  that)
  {
    set_wrap_s      (that.wrap_s      ());
    set_wrap_t      (that.wrap_t      ());
    set_wrap_r      (that.wrap_r      ());
    set_min_filter  (that.min_filter  ());
    set_mag_filter  (that.mag_filter  ());
    set_min_lod     (that.min_lod     ());
    set_max_lod     (that.max_lod     ());
    set_border_color(that.border_color());
    set_lod_bias    (that.lod_bias    ());
    set_compare_mode(that.compare_mode());
    set_compare_func(that.compare_func());
    return *this;
  }
  sampler& operator=(      sampler&& temp) noexcept
  {
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteSamplers(1, &id_);
  
      id_      = std::move(temp.id_);
      managed_ = std::move(temp.managed_);
  
      temp.id_      = invalid_id;
      temp.managed_ = false;    
    }
    return *this;
  }

  void        bind    (GLuint unit) const
  {
    glBindSampler(unit, id_);
  }
  static void unbind  (GLuint unit)
  {
    glBindSampler(unit, 0);
  }
  bool        is_valid() const
  {
    return glIsSampler(id_) != 0;
  }

  void set_wrap_s        (GLenum  mode)
  {
    glSamplerParameteri(id_, GL_TEXTURE_WRAP_S, mode);
  }
  void set_wrap_t        (GLenum  mode)
  {
    glSamplerParameteri(id_, GL_TEXTURE_WRAP_T, mode);
  }
  void set_wrap_r        (GLenum  mode)
  {
    glSamplerParameteri(id_, GL_TEXTURE_WRAP_R, mode);
  }
  void set_min_filter    (GLenum  mode)
  {
    glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, mode);
  }
  void set_mag_filter    (GLenum  mode)
  {
    glSamplerParameteri(id_, GL_TEXTURE_MAG_FILTER, mode);
  }
  void set_min_lod       (GLfloat value)
  {
    glSamplerParameterf(id_, GL_TEXTURE_MIN_LOD, value);
  }
  void set_max_lod       (GLfloat value)
  {
    glSamplerParameterf(id_, GL_TEXTURE_MAX_LOD, value);
  }
  void set_border_color  (const std::array<GLint  , 4>& color)
  {
    glSamplerParameterIiv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_border_color  (const std::array<GLuint , 4>& color)
  {
    glSamplerParameterIuiv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_border_color  (const std::array<GLfloat, 4>& color)
  {
    glSamplerParameterfv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_lod_bias      (GLfloat bias)
  {
    glSamplerParameterf(id_, GL_TEXTURE_LOD_BIAS, bias);
  }
  void set_max_anisotropy(GLfloat max_anisotropy)
  {
    glSamplerParameterf(id_, GL_TEXTURE_MAX_ANISOTROPY, max_anisotropy);
  }
  void set_compare_mode  (GLenum  mode)
  {
    glSamplerParameteri(id_, GL_TEXTURE_COMPARE_MODE, mode);
  }
  void set_compare_func  (GLenum  function)
  {
    glSamplerParameteri(id_, GL_TEXTURE_COMPARE_FUNC, function);
  }
  
  // 8.3 Sampler queries.
  GLenum  wrap_s        () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_S);
  }
  GLenum  wrap_t        () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_T);
  }
  GLenum  wrap_r        () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_R);
  }
  GLenum  min_filter    () const
  {
    return get_int_parameter(GL_TEXTURE_MIN_FILTER);
  }
  GLenum  mag_filter    () const
  {
    return get_int_parameter(GL_TEXTURE_MAG_FILTER);
  }
  GLfloat min_lod       () const
  {
    return get_float_parameter(GL_TEXTURE_MIN_LOD);
  }
  GLfloat max_lod       () const
  {
    return get_float_parameter(GL_TEXTURE_MAX_LOD);
  }
  std::array<GLfloat, 4> border_color    () const
  {
    return get_float_parameter<4>(GL_TEXTURE_BORDER_COLOR);
  }
  std::array<GLint  , 4> border_color_int() const
  {
    return get_int_parameter<4>(GL_TEXTURE_BORDER_COLOR);
  }
  GLfloat lod_bias      () const
  {
    return get_float_parameter(GL_TEXTURE_LOD_BIAS);
  }
  GLfloat max_anisotropy() const
  {
    return get_float_parameter(GL_TEXTURE_MAX_ANISOTROPY);
  }
  GLenum  compare_mode  () const
  {
    return get_int_parameter(GL_TEXTURE_COMPARE_MODE);
  }
  GLenum  compare_func  () const
  {
    return get_int_parameter(GL_TEXTURE_COMPARE_FUNC);
  }

  static const GLenum native_type = GL_SAMPLER;

  GLuint id() const
  {
    return id_;
  }

protected:
  template<std::size_t count>
  std::array<GLint, count>   get_int_parameter  (GLenum parameter) const
  {
    std::array<GLint, count> result;
    glGetSamplerParameteriv(id_, parameter, result.data());
    return result;
  }
  GLint                      get_int_parameter  (GLenum parameter) const
  {
    GLint result;
    glGetSamplerParameteriv(id_, parameter, &result);
    return result;
  }
  template<std::size_t count>
  std::array<GLfloat, count> get_float_parameter(GLenum parameter) const
  {
    std::array<GLfloat, count> result;
    glGetSamplerParameterfv(id_, parameter, result.data());
    return result;
  }
  GLfloat                    get_float_parameter(GLenum parameter) const
  {
    GLfloat result;
    glGetSamplerParameterfv(id_, parameter, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};
}

#endif
