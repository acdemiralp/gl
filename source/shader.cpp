//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/shader.hpp>

namespace gl
{
// 7.1 Shader objects.
shader::shader(GLenum type)
{
  id_ = glCreateShader(type);
}
shader::shader(GLuint id, unmanaged_t unmanaged): id_(id), managed_(false)
{

}
shader::~shader()
{
  if (managed_)
  glDeleteShader(id_);
}

void shader::set_source(const std::string& source) const
{
  auto shader_cstring = source.c_str();
  glShaderSource(id_, 1, &shader_cstring, nullptr);
}
bool shader::compile   () const
{
  glCompileShader(id_);
  return compile_status();
}
bool shader::is_valid  () const
{
  return glIsShader(id_) != 0;
}

void shader::release_compiler()
{
  glReleaseShaderCompiler();
}

// 7.13 Shader queries.
GLenum      shader::type           () const
{
  return get_parameter(GL_SHADER_TYPE);
}
bool        shader::compile_status () const
{
  return get_parameter(GL_COMPILE_STATUS) != 0;
}
bool        shader::delete_status  () const
{
  return get_parameter(GL_DELETE_STATUS) != 0;
}
GLsizei     shader::source_length  () const
{
  return get_parameter(GL_SHADER_SOURCE_LENGTH);
}
GLsizei     shader::info_log_length() const
{
  return get_parameter(GL_INFO_LOG_LENGTH);
}
std::string shader::source         () const
{
  std::string result;
  result.resize(source_length());
  glGetShaderSource(id_, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
  return result;
}
std::string shader::info_log       () const
{
  std::string result;
  result.resize(info_log_length());
  glGetShaderInfoLog(id_, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
  return result;
}

GLuint shader::id() const
{
  return id_;
}

// Extended functionality.
void shader::load_source(const std::string& filename) const
{
  std::ifstream filestream(filename);
  set_source(std::string(std::istreambuf_iterator<char>(filestream), std::istreambuf_iterator<char>()));
}

GLint shader::get_parameter(GLenum parameter) const
{
  GLint result;
  glGetShaderiv(id_, parameter, &result);
  return result;
}
}