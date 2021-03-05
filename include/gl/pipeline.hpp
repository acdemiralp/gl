//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_PIPELINE_HPP_
#define GL_PIPELINE_HPP_

#include <gl/program.hpp>
#include <gl/opengl.hpp>

namespace gl
{
class pipeline
{
public:
  // 7.4 Program pipeline objects.
  pipeline();
  explicit pipeline(GLuint id);
  pipeline(const pipeline&  that) = delete;
  pipeline(      pipeline&& temp) noexcept;
  virtual ~pipeline();
  pipeline& operator=(const pipeline&  that) = delete;
  pipeline& operator=(      pipeline&& temp) noexcept;

  void        bind    () const;
  static void unbind  ();
  bool        is_valid() const;
  
  void use_program_stages       (GLbitfield stages, const program& program = gl::program(0)) const;
  void set_active_shader_program(                   const program& program)                  const;

  // 7.13 Program pipeline queries.
  bool    validation_status                     () const;
  program active_program                        () const;
  program vertex_shader_program                 () const;
  program fragment_shader_program               () const;
  program geometry_shader_program               () const;
  program tessellation_control_shader_program   () const;
  program tessellation_evaluation_shader_program() const;
  program compute_shader_program                () const;
  GLsizei info_log_length                       () const;

  std::string info_log() const;

  // 11.1.3 Shader execution.
  bool validate() const;

  static const GLenum native_type = GL_PROGRAM_PIPELINE;

  GLuint id() const;

protected:
  GLint get_parameter(GLenum parameter) const;

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};
}

#include <gl/implementation/pipeline.ipp>

#endif