//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP

#include <fstream>
#include <string>
#include <tuple>
#include <vector>

#include <gl/opengl.hpp>
#include <gl/unmanaged.hpp>

namespace gl
{
class shader
{
public:
  // 7.1 Shader objects.
  explicit shader(GLenum type)
  {
    id_ = glCreateShader(type);
  }
  shader(GLuint id, unmanaged_t unmanaged) : id_(id), managed_(false)
  {

  }
  shader(const shader&  that) = delete;
  shader(      shader&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
  {
    temp.id_ = invalid_id;
    temp.managed_ = false;
  }
  virtual ~shader()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteShader(id_);
  }
  shader& operator=(const shader&  that) = delete;
  shader& operator=(      shader&& temp) noexcept
  {
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteShader(id_);
  
      id_      = std::move(temp.id_);
      managed_ = std::move(temp.managed_);
  
      temp.id_      = invalid_id;
      temp.managed_ = false;    
    }
    return *this;
  }

  void set_source(const std::string& source) const
  {
    auto shader_cstring = source.c_str();
    glShaderSource(id_, 1, &shader_cstring, nullptr);
  }
  void set_binary(const std::vector<uint8_t>& binary, GLenum format = GL_SHADER_BINARY_FORMAT_SPIR_V) const
  {
    glShaderBinary(1, &id_, format, static_cast<const void*>(binary.data()), static_cast<GLsizei>(binary.size()));
  }
  void specialize(const std::string& entry_point, const std::vector<std::tuple<GLuint, GLuint>>& index_value_pairs) const
  {
    std::vector<GLuint> indices;
    std::vector<GLuint> values ;
    indices.reserve(index_value_pairs.size());
    values .reserve(index_value_pairs.size());
    for (const auto& constant : index_value_pairs)
    {
      indices.push_back(std::get<0>(constant));
      values .push_back(std::get<1>(constant));
    }
    glSpecializeShader(id_, entry_point.c_str(), static_cast<GLuint>(index_value_pairs.size()), indices.data(), values.data());
  }
  bool compile   () const
  {
    glCompileShader(id_);
    return compile_status();
  }
  bool is_valid  () const
  {
    return glIsShader(id_) != 0;
  }

  static void set_binaries    (const std::vector<shader>& shaders, const std::vector<uint8_t>& binary, GLenum format = GL_SHADER_BINARY_FORMAT_SPIR_V)
  {
    std::vector<GLuint> ids;
    for(auto& shader : shaders)
      ids.push_back(shader.id());
    glShaderBinary(static_cast<GLsizei>(ids.size()), ids.data(), format, static_cast<const void*>(binary.data()), static_cast<GLsizei>(binary.size()));
  }
  static void release_compiler()
  {
    glReleaseShaderCompiler();
  }

  // 7.13 Shader queries.
  GLenum      type            () const
  {
    return get_parameter(GL_SHADER_TYPE);
  }
  bool        compile_status  () const
  {
    return get_parameter(GL_COMPILE_STATUS) != 0;
  }
  bool        delete_status   () const
  {
    return get_parameter(GL_DELETE_STATUS) != 0;
  }
  bool        is_spir_v_binary() const
  {
    return get_parameter(GL_SPIR_V_BINARY) != 0;
  }
  GLsizei     source_length   () const
  {
    return get_parameter(GL_SHADER_SOURCE_LENGTH);
  }
  GLsizei     info_log_length () const
  {
    return get_parameter(GL_INFO_LOG_LENGTH);
  }
  std::string source          () const
  {
    std::string result;
    result.resize(source_length());
    glGetShaderSource(id_, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
    return result;
  }
  std::string info_log        () const
  {
    std::string result;
    result.resize(info_log_length());
    glGetShaderInfoLog(id_, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
    return result;
  }

  static const GLenum native_type = GL_SHADER;

  GLuint id() const
  {
    return id_;
  }

  // Extended functionality.
  void load_source(const std::string& filename) const
  {
    std::ifstream filestream(filename);
    set_source(std::string(std::istreambuf_iterator<char>(filestream), std::istreambuf_iterator<char>()));
  }

protected:
  GLint get_parameter(GLenum parameter) const
  {
    GLint result;
    glGetShaderiv(id_, parameter, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};
}

#endif
