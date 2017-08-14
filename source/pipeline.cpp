//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/pipeline.hpp>

#include <utility>

namespace gl
{
// 7.4 Program pipeline objects.
pipeline::pipeline ()
{
  glCreateProgramPipelines(1, &id_);
}
pipeline::pipeline (GLuint id) : id_(id), managed_(false)
{

}
pipeline::pipeline (pipeline&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
{
  temp.id_      = invalid_id;
  temp.managed_ = false;
}
pipeline::~pipeline()
{
  if (managed_ && id_ != invalid_id)
    glDeleteProgramPipelines(1, &id_);
}

pipeline& pipeline::operator=(pipeline&& temp) noexcept
{
  if (this != &temp)
  {
    id_      = std::move(temp.id_);
    managed_ = std::move(temp.managed_);

    temp.id_      = invalid_id;
    temp.managed_ = false;
  }
  return *this;
}

void pipeline::bind    () const
{
  glBindProgramPipeline(id_);
}
void pipeline::unbind  ()
{
  glBindProgramPipeline(0);
}
bool pipeline::is_valid() const
{
  return glIsProgramPipeline(id_) != 0;
}

void pipeline::use_program_stages       (GLbitfield stages, const program& program) const
{
  glUseProgramStages   (id_, stages, program.id());
}
void pipeline::set_active_shader_program(                   const program& program) const
{
  glActiveShaderProgram(id_,         program.id());
}

// 7.13 Program pipeline queries.
bool    pipeline::validation_status                     () const
{
  return get_parameter(GL_VALIDATE_STATUS) != 0;
}
program pipeline::active_program                        () const
{
  return program(get_parameter(GL_ACTIVE_PROGRAM));
}
program pipeline::vertex_shader_program                 () const
{
  return program(get_parameter(GL_VERTEX_SHADER));
}
program pipeline::fragment_shader_program               () const
{
  return program(get_parameter(GL_FRAGMENT_SHADER));
}
program pipeline::geometry_shader_program               () const
{
  return program(get_parameter(GL_GEOMETRY_SHADER));
}
program pipeline::tessellation_control_shader_program   () const
{
  return program(get_parameter(GL_TESS_CONTROL_SHADER));
}
program pipeline::tessellation_evaluation_shader_program() const
{
  return program(get_parameter(GL_TESS_EVALUATION_SHADER));
}
program pipeline::compute_shader_program                () const
{
  return program(get_parameter(GL_COMPUTE_SHADER));
}
GLsizei pipeline::info_log_length                       () const
{
  return get_parameter(GL_INFO_LOG_LENGTH);
}

std::string pipeline::info_log() const
{
  std::string result;
  result.resize(info_log_length());
  glGetProgramInfoLog(id_, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
  return result;
}

// 11.1.3 Shader execution.
bool pipeline::validate() const
{
  glValidateProgram(id_);
  return validation_status();
}

GLuint pipeline::id() const
{
  return id_;
}

GLint pipeline::get_parameter(GLenum parameter) const
{
  GLint result;
  glGetProgramPipelineiv(id_, parameter, &result);
  return result;
}
}
