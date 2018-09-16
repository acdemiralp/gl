//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/sampler.hpp>

#include <utility>

namespace gl
{
// 8.2 Sampler objects.
sampler::sampler()
{
  glCreateSamplers(1, &id_);
}
sampler::sampler(GLuint id): id_(id), managed_(false)
{

}
sampler::sampler(const sampler&  that): sampler()
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
sampler::sampler(      sampler&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
{
  temp.id_      = invalid_id;
  temp.managed_ = false;
}
sampler::~sampler()
{
  if (managed_ && id_ != invalid_id)
    glDeleteSamplers(1, &id_);
}
sampler& sampler::operator=(const sampler&  that)
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
sampler& sampler::operator=(      sampler&& temp) noexcept
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

void sampler::bind    (GLuint unit) const
{
  glBindSampler(unit, id_);
}
void sampler::unbind  (GLuint unit)
{
  glBindSampler(unit, 0);
}
bool sampler::is_valid() const
{
  return glIsSampler(id_) != 0;
}

void sampler::set_wrap_s        (GLenum  mode)
{
  glSamplerParameteri(id_, GL_TEXTURE_WRAP_S, mode);
}
void sampler::set_wrap_t        (GLenum  mode)
{
  glSamplerParameteri(id_, GL_TEXTURE_WRAP_T, mode);
}
void sampler::set_wrap_r        (GLenum  mode)
{
  glSamplerParameteri(id_, GL_TEXTURE_WRAP_R, mode);
}
void sampler::set_min_filter    (GLenum  mode)
{
  glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, mode);
}
void sampler::set_mag_filter    (GLenum  mode)
{
  glSamplerParameteri(id_, GL_TEXTURE_MAG_FILTER, mode);
}
void sampler::set_min_lod       (GLfloat value)
{
  glSamplerParameterf(id_, GL_TEXTURE_MIN_LOD, value);
}
void sampler::set_max_lod       (GLfloat value)
{
  glSamplerParameterf(id_, GL_TEXTURE_MAX_LOD, value);
}
void sampler::set_border_color  (const std::array<GLint  , 4>& color)
{
  glSamplerParameterIiv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
}
void sampler::set_border_color  (const std::array<GLuint , 4>& color)
{
  glSamplerParameterIuiv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
}
void sampler::set_border_color  (const std::array<GLfloat, 4>& color)
{
  glSamplerParameterfv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
}
void sampler::set_lod_bias      (GLfloat bias)
{
  glSamplerParameterf(id_, GL_TEXTURE_LOD_BIAS, bias);
}
void sampler::set_max_anisotropy(GLfloat max_anisotropy)
{
  glSamplerParameterf(id_, GL_TEXTURE_MAX_ANISOTROPY, max_anisotropy);
}
void sampler::set_compare_mode  (GLenum  mode)
{
  glSamplerParameteri(id_, GL_TEXTURE_COMPARE_MODE, mode);
}
void sampler::set_compare_func  (GLenum  function)
{
  glSamplerParameteri(id_, GL_TEXTURE_COMPARE_FUNC, function);
}

// 8.3 Sampler queries.
GLenum  sampler::wrap_s        () const
{
  return get_int_parameter(GL_TEXTURE_WRAP_S);
}
GLenum  sampler::wrap_t        () const
{
  return get_int_parameter(GL_TEXTURE_WRAP_T);
}
GLenum  sampler::wrap_r        () const
{
  return get_int_parameter(GL_TEXTURE_WRAP_R);
}
GLenum  sampler::min_filter    () const
{
  return get_int_parameter(GL_TEXTURE_MIN_FILTER);
}
GLenum  sampler::mag_filter    () const
{
  return get_int_parameter(GL_TEXTURE_MAG_FILTER);
}
GLfloat sampler::min_lod       () const
{
  return get_float_parameter(GL_TEXTURE_MIN_LOD);
}
GLfloat sampler::max_lod       () const
{
  return get_float_parameter(GL_TEXTURE_MAX_LOD);
}
std::array<GLfloat, 4> sampler::border_color    () const
{
  return get_float_parameter<4>(GL_TEXTURE_BORDER_COLOR);
}
std::array<GLint  , 4> sampler::border_color_int() const
{
  return get_int_parameter<4>(GL_TEXTURE_BORDER_COLOR);
}
GLfloat sampler::lod_bias      () const
{
  return get_float_parameter(GL_TEXTURE_LOD_BIAS);
}
GLfloat sampler::max_anisotropy() const
{
  return get_float_parameter(GL_TEXTURE_MAX_ANISOTROPY);
}
GLenum  sampler::compare_mode  () const
{
  return get_int_parameter(GL_TEXTURE_COMPARE_MODE);
}
GLenum  sampler::compare_func  () const
{
  return get_int_parameter(GL_TEXTURE_COMPARE_FUNC);
}

GLuint sampler::id() const
{
  return id_;
}

GLint   sampler::get_int_parameter  (GLenum parameter) const
{
  GLint result;
  glGetSamplerParameteriv(id_, parameter, &result);
  return result;
}
GLfloat sampler::get_float_parameter(GLenum parameter) const
{
  GLfloat result;
  glGetSamplerParameterfv(id_, parameter, &result);
  return result;
}
}