//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_PIPELINE_HPP
#define GL_PIPELINE_HPP

#include <string>

#include <gl/opengl.hpp>
#include <gl/program.hpp>

namespace gl
{
class pipeline
{
public:
  // 7.4 Program pipeline objects.
  pipeline           ()
  {
    glCreateProgramPipelines(1, &id_);
  }
  explicit pipeline  (const GLuint id) : id_(id), managed_(false)
  {

  }
  pipeline           (const pipeline&  that) = delete;
  pipeline           (      pipeline&& temp) noexcept : id_(temp.id_), managed_(temp.managed_)
  {
    temp.id_ = invalid_id;
    temp.managed_ = false;
  }
  virtual ~pipeline  ()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteProgramPipelines(1, &id_);
  }
  pipeline& operator=(const pipeline&  that) = delete;
  pipeline& operator=(      pipeline&& temp) noexcept
  {
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteProgramPipelines(1, &id_);
  
      id_      = temp.id_;
      managed_ = temp.managed_;
  
      temp.id_      = invalid_id;
      temp.managed_ = false;
    }
    return *this;
  }

  void        bind    () const
  {
    glBindProgramPipeline(id_);
  }
  static void unbind  ()
  {
    glBindProgramPipeline(0);
  }

  [[nodiscard]]
  bool        is_valid() const
  {
    return glIsProgramPipeline(id_) != 0;
  }

  void use_program_stages       (const GLbitfield stages, const program& program = gl::program(0)) const
  {
    glUseProgramStages(id_, stages, program.id());
  }
  void set_active_shader_program(                         const program& program)                  const
  {
    glActiveShaderProgram(id_, program.id());
  }

  // 7.13 Program pipeline queries.
  [[nodiscard]]
  bool    validation_status                     () const
  {
    return get_parameter(GL_VALIDATE_STATUS) != 0;
  }
  [[nodiscard]]
  program active_program                        () const
  {
    return program(get_parameter(GL_ACTIVE_PROGRAM));
  }
  [[nodiscard]]
  program vertex_shader_program                 () const
  {
    return program(get_parameter(GL_VERTEX_SHADER));
  }
  [[nodiscard]]
  program fragment_shader_program               () const
  {
    return program(get_parameter(GL_FRAGMENT_SHADER));
  }
  [[nodiscard]]
  program geometry_shader_program               () const
  {
    return program(get_parameter(GL_GEOMETRY_SHADER));
  }
  [[nodiscard]]
  program tessellation_control_shader_program   () const
  {
    return program(get_parameter(GL_TESS_CONTROL_SHADER));
  }
  [[nodiscard]]
  program tessellation_evaluation_shader_program() const
  {
    return program(get_parameter(GL_TESS_EVALUATION_SHADER));
  }
  [[nodiscard]]
  program compute_shader_program                () const
  {
    return program(get_parameter(GL_COMPUTE_SHADER));
  }
  [[nodiscard]]
  GLsizei info_log_length                       () const
  {
    return get_parameter(GL_INFO_LOG_LENGTH);
  }

  [[nodiscard]]
  std::string info_log() const
  {
    std::string result;
    result.resize(info_log_length());
    glGetProgramInfoLog(id_, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
    return result;
  }

  // 11.1.3 Shader execution.
  [[nodiscard]]
  bool validate() const
  {
    glValidateProgram(id_);
    return validation_status();
  }

  static const GLenum native_type = GL_PROGRAM_PIPELINE;

  [[nodiscard]]
  GLuint id() const
  {
    return id_;
  }

protected:
  [[nodiscard]]
  GLint get_parameter(const GLenum parameter) const
  {
    GLint result;
    glGetProgramPipelineiv(id_, parameter, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};
}

#endif