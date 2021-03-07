//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_PROGRAM_HPP
#define GL_PROGRAM_HPP

#include <algorithm>
#include <array>
#include <string>
#include <tuple>
#include <vector>

#include <gl/image_handle.hpp>
#include <gl/opengl.hpp>
#include <gl/shader.hpp>
#include <gl/texture_handle.hpp>

#ifdef interface 
#undef interface 
#endif

namespace gl
{
// Next steps: Modularize uniform, uniform block, atomic counter, attribute accesses.
class program
{
public:
  // 7.3 Program objects.
  program() : id_(glCreateProgram())
  {

  }
  explicit program(const GLuint id) : id_(id), managed_(false)
  {

  }
  program(const program&  that) = delete;
  program(      program&& temp) noexcept : id_(temp.id_), managed_(temp.managed_)
  {
    temp.id_ = invalid_id;
    temp.managed_ = false;
  }
  virtual ~program()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteProgram(id_);
  }
  program& operator=(const program&  that) = delete;
  program& operator=(      program&& temp) noexcept
  {
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteProgram(id_);
  
      id_      = temp.id_;
      managed_ = temp.managed_;
  
      temp.id_      = invalid_id;
      temp.managed_ = false;    
    }
    return *this;
  }

  static program create_shader_program(const GLenum type, const std::string&              shader_string )
  {
    const auto* shader_string_c = shader_string.c_str();
    return program(glCreateShaderProgramv(type, 1, &shader_string_c));
  }
  static program create_shader_program(const GLenum type, const std::vector<std::string>& shader_strings)
  {
    std::vector<char*> shader_strings_c(shader_strings.size());
    std::transform(shader_strings.begin(), shader_strings.end(), shader_strings_c.begin(), 
    [&](const std::string& shader_string)
    {
      return const_cast<char*>(shader_string.c_str());
    });
    return program(glCreateShaderProgramv(type, static_cast<GLsizei>(shader_strings.size()), shader_strings_c.data()));
  }

  void        attach_shader(const shader& shader) const
  {
    glAttachShader(id_, shader.id());
  }
  void        detach_shader(const shader& shader) const
  {
    glDetachShader(id_, shader.id());
  }
  [[nodiscard]]
  bool        link         () const
  {
    glLinkProgram(id_);
    return link_status();
  }
  void        use          () const
  {
    glUseProgram(id_);
  }

  static void unuse        ()
  {
    glUseProgram(0);
  }
  [[nodiscard]]
  bool        is_valid     () const
  {
    return glIsProgram(id_) != 0;
  }
  
  void set_separable         (const bool separable) const
  {
    glProgramParameteri(id_, GL_PROGRAM_SEPARABLE, separable);
  }
  void set_binary_retrievable(const bool binary_retrievable) const
  {
    glProgramParameteri(id_, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, binary_retrievable);
  }

  // 7.3.1 Program interfaces.
  [[nodiscard]]
  GLsizei     interface_active_resources               (const GLenum interface) const
  {
    return get_interface_parameter(interface, GL_ACTIVE_RESOURCES);
  }
  [[nodiscard]]
  GLsizei     interface_max_name_length                (const GLenum interface) const
  {
    return get_interface_parameter(interface, GL_MAX_NAME_LENGTH);
  }
  [[nodiscard]]
  GLsizei     interface_max_active_variable_count      (const GLenum interface) const
  {
    return get_interface_parameter(interface, GL_MAX_NUM_ACTIVE_VARIABLES);
  }
  [[nodiscard]]
  GLsizei     interface_max_compatible_subroutine_count(const GLenum interface) const
  {
    return get_interface_parameter(interface, GL_MAX_NUM_COMPATIBLE_SUBROUTINES);
  }
  [[nodiscard]]
  GLuint      resource_index                           (const GLenum interface, const std::string& name) const
  {
    return glGetProgramResourceIndex(id_, interface, name.c_str());
  }
  [[nodiscard]]
  std::string resource_name                            (const GLenum interface, const GLuint index) const
  {
    std::string name;
    GLsizei length;
    name.resize(interface_max_name_length(interface));
    glGetProgramResourceName(id_, interface, index, static_cast<GLsizei>(name.size()), &length, &name[0]);
    name.resize(length);
    return name;
  }
  [[nodiscard]]
  GLint       resource_location                        (const GLenum interface, const std::string& name) const
  {
    return glGetProgramResourceLocation(id_, interface, name.c_str());
  }
  [[nodiscard]]
  GLint       resource_location_index                  (const GLenum interface, const std::string& name) const
  {
    return glGetProgramResourceLocationIndex(id_, interface, name.c_str());
  }

  [[nodiscard]]
  GLint              resource_parameter (const GLenum interface, const GLuint index, GLenum parameter) const
  {
    GLint result;
    glGetProgramResourceiv(id_, interface, index, 1, &parameter, 1, nullptr, &result);
    return result;
  }
  [[nodiscard]]
  std::vector<GLint> resource_parameters(const GLenum interface, const GLuint index, const std::vector<GLenum>& parameters) const
  {
    std::vector<GLint> result(parameters.size());
    glGetProgramResourceiv(id_, interface, index, static_cast<GLsizei>(parameters.size()), parameters.data(), static_cast<GLsizei>(result.size()), nullptr, result.data());
    return result;
  }

  // 7.5 Program binaries.
  template<typename type = GLbyte>
  std::vector<type> program_binary    (const GLenum format) const
  {
    std::vector<type> result(binary_length());
    glGetProgramBinary(id_, static_cast<GLsizei>(result.size()), nullptr, &format, static_cast<void*>(result.data()));
    return result;
  }
  template<typename type = GLbyte>
  void              set_program_binary(const GLenum format, const std::vector<type>& binary)
  {
    glProgramBinary(id_, format, static_cast<const void*>(binary.data()), static_cast<GLsizei>(binary.size()));
  }

  // 7.6 Uniform variables.
  [[nodiscard]]
  GLint uniform_location(const std::string& name) const
  {
    return glGetUniformLocation(id_, name.c_str());
  }

  [[nodiscard]]
  std::string active_uniform_name(const GLuint index) const
  {
    std::string result;
    result.resize(active_uniform_name_length(index));
    glGetActiveUniformName(id_, index, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
    return result;
  }

  [[nodiscard]]
  std::vector<GLuint> uniform_indices(const GLsizei count, const std::vector<std::string>& names) const
  {
    std::vector<GLuint> indices(names.size());
    std::vector<const char*> names_c(names.size());
    std::transform(names.begin(), names.end(), names_c.begin(), 
    [&](const std::string& varying)
    {
      return varying.c_str();
    });
    glGetUniformIndices(id_, count, names_c.data(), indices.data());
    return indices;
  }

  [[nodiscard]]
  std::tuple<std::string, GLenum, GLsizei> active_uniform(const GLuint index) const
  {
    std::tuple<std::string, GLenum, GLsizei> result;
    auto& name = std::get<0>(result);
    auto& type = std::get<1>(result);
    auto& size = std::get<2>(result);
    name.resize(active_uniform_max_length());
    glGetActiveUniform(id_, index, static_cast<GLsizei>(name.size()), nullptr, &size, &type, &name[0]);
    return result;
  }

  [[nodiscard]]
  GLsizei active_uniform_name_length                (const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_NAME_LENGTH);
  }
  [[nodiscard]]
  GLenum  active_uniform_type                       (const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_TYPE);
  }
  [[nodiscard]]
  GLuint  active_uniform_offset                     (const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_OFFSET);
  }
  [[nodiscard]]
  GLsizei active_uniform_size                       (const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_SIZE);
  }
  [[nodiscard]]
  GLuint  active_uniform_block_index                (const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_BLOCK_INDEX);
  }
  [[nodiscard]]
  GLsizei active_uniform_array_stride               (const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_ARRAY_STRIDE);
  }
  [[nodiscard]]
  GLsizei active_uniform_matrix_stride              (const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_MATRIX_STRIDE);
  }
  [[nodiscard]]
  bool    active_uniform_is_row_major               (const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_IS_ROW_MAJOR) != 0;
  }
  [[nodiscard]]
  GLuint  active_uniform_atomic_counter_buffer_index(const GLuint index) const
  {
    return get_active_uniform_parameter(index, GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX);
  }

  [[nodiscard]]
  std::vector<GLsizei> active_uniforms_name_lengths                 (const std::vector<GLuint>& indices) const
  {
    return get_active_uniform_parameters(indices, GL_UNIFORM_NAME_LENGTH);
  }
  [[nodiscard]]
  std::vector<GLenum>  active_uniforms_types                        (const std::vector<GLuint>& indices) const
  {
    auto result = get_active_uniform_parameters(indices, GL_UNIFORM_TYPE);
    return std::vector<GLenum>(result.begin(), result.end());
  }
  [[nodiscard]]
  std::vector<GLuint>  active_uniforms_offsets                      (const std::vector<GLuint>& indices) const
  {
    auto result = get_active_uniform_parameters(indices, GL_UNIFORM_OFFSET);
    return std::vector<GLuint>(result.begin(), result.end());
  }
  [[nodiscard]]
  std::vector<GLsizei> active_uniforms_sizes                        (const std::vector<GLuint>& indices) const
  {
    return get_active_uniform_parameters(indices, GL_UNIFORM_SIZE);
  }
  [[nodiscard]]
  std::vector<GLuint>  active_uniforms_block_indices                (const std::vector<GLuint>& indices) const
  {
    auto result = get_active_uniform_parameters(indices, GL_UNIFORM_BLOCK_INDEX);
    return std::vector<GLuint>(result.begin(), result.end());
  }
  [[nodiscard]]
  std::vector<GLsizei> active_uniforms_array_strides                (const std::vector<GLuint>& indices) const
  {
    return get_active_uniform_parameters(indices, GL_UNIFORM_ARRAY_STRIDE);
  }
  [[nodiscard]]
  std::vector<GLsizei> active_uniforms_matrix_strides               (const std::vector<GLuint>& indices) const
  {
    return get_active_uniform_parameters(indices, GL_UNIFORM_MATRIX_STRIDE);
  }
  [[nodiscard]]
  std::vector<bool>    active_uniforms_are_row_majors               (const std::vector<GLuint>& indices) const
  {
    auto result = get_active_uniform_parameters(indices, GL_UNIFORM_IS_ROW_MAJOR);
    return std::vector<bool>(result.begin(), result.end()); // Warning C4800 inside this line.
  }
  [[nodiscard]]
  std::vector<GLuint>  active_uniforms_atomic_counter_buffer_indices(const std::vector<GLuint>& indices) const
  {
    auto result = get_active_uniform_parameters(indices, GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX);
    return std::vector<GLuint>(result.begin(), result.end());
  }

  [[nodiscard]]
  GLuint uniform_block_index(const std::string& name) const
  {
    return glGetUniformBlockIndex(id_, name.c_str());
  }

  [[nodiscard]]
  std::string active_uniform_block_name(const GLuint index) const
  {
    std::string result;
    result.resize(active_uniform_block_name_length(index));
    glGetActiveUniformBlockName(id_, index, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
    return result;
  }

  [[nodiscard]]
  GLuint  active_uniform_block_binding                                        (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_BINDING);
  }
  [[nodiscard]]
  GLuint  active_uniform_block_data_size                                      (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_DATA_SIZE);
  }
  [[nodiscard]]
  GLuint  active_uniform_block_name_length                                    (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_NAME_LENGTH);
  }
  [[nodiscard]]
  GLsizei active_uniform_block_uniform_count                                  (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS);
  }
  [[nodiscard]]
  bool    active_uniform_block_is_referenced_by_vertex_shader                 (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_uniform_block_is_referenced_by_fragment_shader               (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_uniform_block_is_referenced_by_compute_shader                (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_uniform_block_is_referenced_by_geometry_shader               (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_uniform_block_is_referenced_by_tessellation_control_shader   (const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_uniform_block_is_referenced_by_tessellation_evaluation_shader(const GLuint index) const
  {
    return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER) != 0;
  }

  [[nodiscard]]
  std::vector<GLuint> active_uniform_block_uniform_indices(const GLuint index) const
  {
    std::vector<GLint> result(active_uniform_block_uniform_count(index));
    glGetActiveUniformBlockiv(id_, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &result[0]);
    return std::vector<GLuint>(result.begin(), result.end());
  }

  [[nodiscard]]
  GLuint  active_atomic_counter_buffer_data_size                                      (const GLuint index) const
  {
    return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE);
  }
  [[nodiscard]]
  GLsizei active_atomic_counter_buffer_counters                                       (const GLuint index) const
  {
    return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS);
  }
  [[nodiscard]]
  bool    active_atomic_counter_buffer_is_referenced_by_vertex_shader                 (const GLuint index) const
  {
    return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_atomic_counter_buffer_is_referenced_by_fragment_shader               (const GLuint index) const
  {
    return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_atomic_counter_buffer_is_referenced_by_compute_shader                (const GLuint index) const
  {
    return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_atomic_counter_buffer_is_referenced_by_geometry_shader               (const GLuint index) const
  {
    return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_atomic_counter_buffer_is_referenced_by_tessellation_control_shader   (const GLuint index) const
  {
    return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER) != 0;
  }
  [[nodiscard]]
  bool    active_atomic_counter_buffer_is_referenced_by_tessellation_evaluation_shader(const GLuint index) const
  {
    return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER) != 0;
  }

  [[nodiscard]]
  std::vector<GLuint> active_atomic_counter_buffer_counter_indices(const GLuint index) const
  {
    std::vector<GLint> result(active_uniform_block_uniform_count(index));
    glGetActiveUniformBlockiv(id_, index, GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES, &result[0]);
    return std::vector<GLuint>(result.begin(), result.end());
  }

  void set_uniform_1i(const GLint location, const GLint value) const
  {
    glProgramUniform1i(id_, location, value);
  }
  void set_uniform_1i(const GLint location, const std::vector<GLint>&   value) const
  {
    glProgramUniform1iv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_2i(const GLint location, const std::array<GLint, 2>& value) const
  {
    glProgramUniform2i(id_, location, value[0], value[1]);
  }
  void set_uniform_2i(const GLint location, const std::vector<GLint>&   value) const
  {
    glProgramUniform2iv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_3i(const GLint location, const std::array<GLint, 3>& value) const
  {
    glProgramUniform3i(id_, location, value[0], value[1], value[2]);
  }
  void set_uniform_3i(const GLint location, const std::vector<GLint>&   value) const
  {
    glProgramUniform3iv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_4i(const GLint location, const std::array<GLint, 4>& value) const
  {
    glProgramUniform4i(id_, location, value[0], value[1], value[2], value[3]);
  }
  void set_uniform_4i(const GLint location, const std::vector<GLint>&   value) const
  {
    glProgramUniform4iv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  
  void set_uniform_1ui(const GLint location, const GLuint value) const
  {
    glProgramUniform1ui(id_, location, value);
  }
  void set_uniform_1ui(const GLint location, const std::vector<GLuint>&   value) const
  {
    glProgramUniform1uiv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_2ui(const GLint location, const std::array<GLuint, 2>& value) const
  {
    glProgramUniform2ui(id_, location, value[0], value[1]);
  }
  void set_uniform_2ui(const GLint location, const std::vector<GLuint>&   value) const
  {
    glProgramUniform2uiv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_3ui(const GLint location, const std::array<GLuint, 3>& value) const
  {
    glProgramUniform3ui(id_, location, value[0], value[1], value[2]);
  }
  void set_uniform_3ui(const GLint location, const std::vector<GLuint>&   value) const
  {
    glProgramUniform3uiv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_4ui(const GLint location, const std::array<GLuint, 4>& value) const
  {
    glProgramUniform4ui(id_, location, value[0], value[1], value[2], value[3]);
  }
  void set_uniform_4ui(const GLint location, const std::vector<GLuint>&   value) const
  {
    glProgramUniform4uiv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  
  void set_uniform_1f(const GLint location, const GLfloat value) const
  {
    glProgramUniform1f(id_, location, value);
  }
  void set_uniform_1f(const GLint location, const std::vector<GLfloat>&   value) const
  {
    glProgramUniform1fv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_2f(const GLint location, const std::array<GLfloat, 2>& value) const
  {
    glProgramUniform2f(id_, location, value[0], value[1]);
  }
  void set_uniform_2f(const GLint location, const std::vector<GLfloat>&   value) const
  {
    glProgramUniform2fv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_3f(const GLint location, const std::array<GLfloat, 3>& value) const
  {
    glProgramUniform3f(id_, location, value[0], value[1], value[2]);
  }
  void set_uniform_3f(const GLint location, const std::vector<GLfloat>&   value) const
  {
    glProgramUniform3fv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_4f(const GLint location, const std::array<GLfloat, 4>& value) const
  {
    glProgramUniform4f(id_, location, value[0], value[1], value[2], value[3]);
  }
  void set_uniform_4f(const GLint location, const std::vector<GLfloat>&   value) const
  {
    glProgramUniform4fv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  
  void set_uniform_1d(const GLint location, const GLdouble value) const
  {
    glProgramUniform1d(id_, location, value);
  }
  void set_uniform_1d(const GLint location, const std::vector<GLdouble>&   value) const
  {
    glProgramUniform1dv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_2d(const GLint location, const std::array<GLdouble, 2>& value) const
  {
    glProgramUniform2d(id_, location, value[0], value[1]);
  }
  void set_uniform_2d(const GLint location, const std::vector<GLdouble>&   value) const
  {
    glProgramUniform2dv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_3d(const GLint location, const std::array<GLdouble, 3>& value) const
  {
    glProgramUniform3d(id_, location, value[0], value[1], value[2]);
  }
  void set_uniform_3d(const GLint location, const std::vector<GLdouble>&   value) const
  {
    glProgramUniform3dv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  void set_uniform_4d(const GLint location, const std::array<GLdouble, 4>& value) const
  {
    glProgramUniform4d(id_, location, value[0], value[1], value[2], value[3]);
  }
  void set_uniform_4d(const GLint location, const std::vector<GLdouble>&   value) const
  {
    glProgramUniform4dv(id_, location, static_cast<GLsizei>(value.size()), value.data());
  }
  
  void set_uniform_matrix_22f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix2fv(id_, location, static_cast<GLsizei>(value.size() / 4), transpose, value.data());
  }
  void set_uniform_matrix_33f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix3fv(id_, location, static_cast<GLsizei>(value.size() / 9), transpose, value.data());
  }
  void set_uniform_matrix_44f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix4fv(id_, location, static_cast<GLsizei>(value.size() / 16), transpose, value.data());
  }
  void set_uniform_matrix_23f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix2x3fv(id_, location, static_cast<GLsizei>(value.size() / 6), transpose, value.data());
  }
  void set_uniform_matrix_32f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix3x2fv(id_, location, static_cast<GLsizei>(value.size() / 6), transpose, value.data());
  }
  void set_uniform_matrix_24f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix2x4fv(id_, location, static_cast<GLsizei>(value.size() / 8), transpose, value.data());
  }
  void set_uniform_matrix_42f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix4x2fv(id_, location, static_cast<GLsizei>(value.size() / 8), transpose, value.data());
  }
  void set_uniform_matrix_34f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix3x4fv(id_, location, static_cast<GLsizei>(value.size() / 12), transpose, value.data());
  }
  void set_uniform_matrix_43f(const GLint location, const std::vector<GLfloat>& value, const bool transpose) const
  {
    glProgramUniformMatrix4x3fv(id_, location, static_cast<GLsizei>(value.size() / 12), transpose, value.data());
  }
  
  void set_uniform_matrix_22d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix2dv(id_, location, static_cast<GLsizei>(value.size() / 4), transpose, value.data());
  }
  void set_uniform_matrix_33d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix3dv(id_, location, static_cast<GLsizei>(value.size() / 9), transpose, value.data());
  }
  void set_uniform_matrix_44d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix4dv(id_, location, static_cast<GLsizei>(value.size() / 16), transpose, value.data());
  }
  void set_uniform_matrix_23d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix2x3dv(id_, location, static_cast<GLsizei>(value.size() / 6), transpose, value.data());
  }
  void set_uniform_matrix_32d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix3x2dv(id_, location, static_cast<GLsizei>(value.size() / 6), transpose, value.data());
  }
  void set_uniform_matrix_24d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix2x4dv(id_, location, static_cast<GLsizei>(value.size() / 8), transpose, value.data());
  }
  void set_uniform_matrix_42d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix4x2dv(id_, location, static_cast<GLsizei>(value.size() / 8), transpose, value.data());
  }
  void set_uniform_matrix_34d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix3x4dv(id_, location, static_cast<GLsizei>(value.size() / 12), transpose, value.data());
  }
  void set_uniform_matrix_43d(const GLint location, const std::vector<GLdouble>& value, const bool transpose) const
  {
    glProgramUniformMatrix4x3dv(id_, location, static_cast<GLsizei>(value.size() / 12), transpose, value.data());
  }

#ifdef GL_ARB_bindless_texture
  void set_uniform_handle(const GLint location, const texture_handle&              value) const
  {
    glProgramUniformHandleui64ARB(id_, location, value.id());
  }
  void set_uniform_handle(const GLint location, const std::vector<texture_handle>& value) const
  {
    std::vector<GLuint64> ids(value.size());
    std::transform(value.begin(), value.end(), ids.begin(), [&](const texture_handle& iteratee)
    {
      return iteratee.id();
    });
    glProgramUniformHandleui64vARB(id_, location, static_cast<GLsizei>(ids.size()), ids.data());
  }
  void set_uniform_handle(const GLint location, const image_handle&                value) const
  {
    glProgramUniformHandleui64ARB(id_, location, value.id());
  }
  void set_uniform_handle(const GLint location, const std::vector<image_handle>&   value) const
  {
    std::vector<GLuint64> ids(value.size());
    std::transform(value.begin(), value.end(), ids.begin(), [&](const image_handle& iteratee)
    {
      return iteratee.id();
    });
    glProgramUniformHandleui64vARB(id_, location, static_cast<GLsizei>(ids.size()), ids.data());
  }
#endif

  void set_uniform_block_binding(const GLuint index, const GLuint binding) const
  {
    glUniformBlockBinding(id_, index, binding);
  }
  
  // 7.8 Shader buffer variables.
  void set_shader_storage_block_binding(const GLuint index, const GLuint binding) const
  {
    glShaderStorageBlockBinding(id_, index, binding);
  }
  
  // 7.9 Subroutine uniform variables.
  [[nodiscard]]
  GLint               subroutine_uniform_location                          (const GLenum shader_type, const std::string& name) const
  {
    return glGetSubroutineUniformLocation(id_, shader_type, name.c_str());
  }
  [[nodiscard]]
  GLuint              subroutine_index                                     (const GLenum shader_type, const std::string& name) const
  {
    return glGetSubroutineIndex(id_, shader_type, name.c_str());
  }
  [[nodiscard]]
  std::string         active_subroutine_name                               (const GLenum shader_type, const GLuint subroutine_index) const
  {
    std::string result;
    result.resize(active_subroutine_max_length(shader_type));
    glGetActiveSubroutineName(id_, shader_type, subroutine_index, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
    return result;
  }
  [[nodiscard]]
  std::string         active_subroutine_uniform_name                       (const GLenum shader_type, const GLuint subroutine_index) const
  {
    std::string result;
    result.resize(active_subroutine_uniform_max_length(shader_type));
    glGetActiveSubroutineUniformName(id_, shader_type, subroutine_index, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
    return result;
  }
  [[nodiscard]]
  std::vector<GLuint> active_subroutine_uniform_compatible_subroutines     (const GLenum shader_type, const GLuint subroutine_index) const
  {
    std::vector<GLint> result(active_subroutine_uniform_compatible_subroutine_count(shader_type, subroutine_index));
    glGetActiveSubroutineUniformiv(id_, shader_type, subroutine_index, GL_COMPATIBLE_SUBROUTINES, result.data());
    return std::vector<GLuint>(result.begin(), result.end());
  }
  [[nodiscard]]
  GLsizei             active_subroutine_uniform_compatible_subroutine_count(const GLenum shader_type, const GLuint subroutine_index) const
  {
    GLint result;
    glGetActiveSubroutineUniformiv(id_, shader_type, subroutine_index, GL_NUM_COMPATIBLE_SUBROUTINES, &result);
    return result;
  }
  void                set_active_subroutine_uniforms                       (const GLenum shader_type, const std::vector<GLuint>& subroutine_indices) const
  {
    glUniformSubroutinesuiv(shader_type, active_subroutine_uniform_location_count(shader_type), subroutine_indices.data());
  }

  // 7.13 Program queries.
  [[nodiscard]]
  bool    delete_status                           () const
  {
    return get_parameter(GL_DELETE_STATUS) != 0;
  }
  [[nodiscard]]
  bool    link_status                             () const
  {
    return get_parameter(GL_LINK_STATUS) != 0;
  }
  [[nodiscard]]
  bool    validation_status                       () const
  {
    return get_parameter(GL_VALIDATE_STATUS) != 0;
  }
  [[nodiscard]]
  GLsizei info_log_length                         () const
  {
    return get_parameter(GL_INFO_LOG_LENGTH);
  }
  [[nodiscard]]
  GLsizei attached_shader_count                   () const
  {
    return get_parameter(GL_ATTACHED_SHADERS);
  }
  [[nodiscard]]
  GLsizei active_attribute_count                  () const
  {
    return get_parameter(GL_ACTIVE_ATTRIBUTES);
  }
  [[nodiscard]]
  GLsizei active_attribute_max_length             () const
  {
    return get_parameter(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
  }
  [[nodiscard]]
  GLsizei active_uniform_count                    () const
  {
    return get_parameter(GL_ACTIVE_UNIFORMS);
  }
  [[nodiscard]]
  GLsizei active_uniform_max_length               () const
  {
    return get_parameter(GL_ACTIVE_UNIFORM_MAX_LENGTH);
  }
  [[nodiscard]]
  GLsizei active_uniform_block_count              () const
  {
    return get_parameter(GL_ACTIVE_UNIFORM_BLOCKS);
  }
  [[nodiscard]]
  GLsizei active_uniform_block_max_name_length    () const
  {
    return get_parameter(GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH);
  }
  [[nodiscard]]
  GLenum  transform_feedback_buffer_mode          () const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_BUFFER_MODE);
  }
  [[nodiscard]]
  GLsizei transform_feedback_varying_count        () const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_VARYINGS);
  }
  [[nodiscard]]
  GLsizei transform_feedback_varying_max_length   () const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH);
  }
  [[nodiscard]]
  GLsizei geometry_vertices_out                   () const
  {
    return get_parameter(GL_GEOMETRY_VERTICES_OUT);
  }
  [[nodiscard]]
  GLenum  geometry_input_type                     () const
  {
    return get_parameter(GL_GEOMETRY_INPUT_TYPE);
  }
  [[nodiscard]]
  GLenum  geometry_output_type                    () const
  {
    return get_parameter(GL_GEOMETRY_OUTPUT_TYPE);
  }
  [[nodiscard]]
  GLsizei geometry_shader_invocations             () const
  {
    return get_parameter(GL_GEOMETRY_SHADER_INVOCATIONS);
  }
  [[nodiscard]]
  GLsizei tessellation_control_output_vertex_count() const
  {
    return get_parameter(GL_TESS_CONTROL_OUTPUT_VERTICES);
  }
  [[nodiscard]]
  GLenum  tessellation_generation_mode            () const
  {
    return get_parameter(GL_TESS_GEN_MODE);
  }
  [[nodiscard]]
  GLenum  tessellation_generation_spacing         () const
  {
    return get_parameter(GL_TESS_GEN_SPACING);
  }
  [[nodiscard]]
  GLenum  tessellation_generation_vertex_order    () const
  {
    return get_parameter(GL_TESS_GEN_VERTEX_ORDER);
  }
  [[nodiscard]]
  bool    tessellation_generation_point_mode      () const
  {
    return get_parameter(GL_TESS_GEN_POINT_MODE) != 0;
  }
  [[nodiscard]]
  bool    is_separable                            () const
  {
    return get_parameter(GL_PROGRAM_SEPARABLE) != 0;
  }
  [[nodiscard]]
  bool    is_binary_retrievable                   () const
  {
    return get_parameter(GL_PROGRAM_BINARY_RETRIEVABLE_HINT) != 0;
  }
  [[nodiscard]]
  GLsizei active_atomic_counter_buffer_count      () const
  {
    return get_parameter(GL_ACTIVE_ATOMIC_COUNTER_BUFFERS);
  }
  [[nodiscard]]
  GLsizei binary_length                           () const
  {
    return get_parameter(GL_PROGRAM_BINARY_LENGTH);
  }

  [[nodiscard]]
  std::array<GLint, 3> compute_work_group_size() const
  {
    std::array<GLint, 3> result {};
    glGetProgramiv(id_, GL_COMPUTE_WORK_GROUP_SIZE, result.data());
    return result;
  }

  [[nodiscard]]
  std::vector<shader> attached_shaders() const
  {
    std::vector<shader> shaders;
    std::vector<GLuint> shader_ids(attached_shader_count());
    glGetAttachedShaders(id_, static_cast<GLsizei>(shader_ids.size()), nullptr, shader_ids.data());
    for (auto shader_id : shader_ids)
      shaders.emplace_back(shader(shader_id));
    return shaders;
  }

  [[nodiscard]]
  std::string info_log() const
  {
    std::string result;
    result.resize(info_log_length());
    glGetProgramInfoLog(id_, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
    return result;
  }

  [[nodiscard]]
  GLint                    uniform_1i (const GLint location) const
  {
    GLint value;
    glGetUniformiv(id_, location, &value);
    return value;
  }
  [[nodiscard]]
  std::array<GLint, 2>     uniform_2i (const GLint location) const
  {
    std::array<GLint, 2> value {};
    glGetUniformiv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLint, 3>     uniform_3i (const GLint location) const
  {
    std::array<GLint, 3> value {};
    glGetUniformiv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLint, 4>     uniform_4i (const GLint location) const
  {
    std::array<GLint, 4> value {};
    glGetUniformiv(id_, location, &value[0]);
    return value;
  }

  [[nodiscard]]
  GLuint                   uniform_1ui(const GLint location) const
  {
    GLuint value;
    glGetUniformuiv(id_, location, &value);
    return value;
  }
  [[nodiscard]]
  std::array<GLuint, 2>    uniform_2ui(const GLint location) const
  {
    std::array<GLuint, 2> value {};
    glGetUniformuiv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLuint, 3>    uniform_3ui(const GLint location) const
  {
    std::array<GLuint, 3> value {};
    glGetUniformuiv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLuint, 4>    uniform_4ui(const GLint location) const
  {
    std::array<GLuint, 4> value {};
    glGetUniformuiv(id_, location, &value[0]);
    return value;
  }

  [[nodiscard]]
  GLfloat                  uniform_1f (const GLint location) const
  {
    GLfloat value;
    glGetUniformfv(id_, location, &value);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 2>   uniform_2f (const GLint location) const
  {
    std::array<GLfloat, 2> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 3>   uniform_3f (const GLint location) const
  {
    std::array<GLfloat, 3> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 4>   uniform_4f (const GLint location) const
  {
    std::array<GLfloat, 4> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }

  [[nodiscard]]
  GLdouble                 uniform_1d (const GLint location) const
  {
    GLdouble value;
    glGetUniformdv(id_, location, &value);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 2>  uniform_2d (const GLint location) const
  {
    std::array<GLdouble, 2> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 3>  uniform_3d (const GLint location) const
  {
    std::array<GLdouble, 3> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 4>  uniform_4d (const GLint location) const
  {
    std::array<GLdouble, 4> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }

  [[nodiscard]]
  std::array<GLfloat, 4>   uniform_matrix_22f(const GLint location) const
  {
    std::array<GLfloat, 4> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 9>   uniform_matrix_33f(const GLint location) const
  {
    std::array<GLfloat, 9> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 16>  uniform_matrix_44f(const GLint location) const
  {
    std::array<GLfloat, 16> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 6>   uniform_matrix_23f(const GLint location) const
  {
    std::array<GLfloat, 6> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 6>   uniform_matrix_32f(const GLint location) const
  {
    std::array<GLfloat, 6> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 8>   uniform_matrix_24f(const GLint location) const
  {
    std::array<GLfloat, 8> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 8>   uniform_matrix_42f(const GLint location) const
  {
    std::array<GLfloat, 8> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 12>  uniform_matrix_34f(const GLint location) const
  {
    std::array<GLfloat, 12> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLfloat, 12>  uniform_matrix_43f(const GLint location) const
  {
    std::array<GLfloat, 12> value {};
    glGetUniformfv(id_, location, &value[0]);
    return value;
  }

  [[nodiscard]]
  std::array<GLdouble, 4>  uniform_matrix_22d(const GLint location) const
  {
    std::array<GLdouble, 4> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 9>  uniform_matrix_33d(const GLint location) const
  {
    std::array<GLdouble, 9> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 16> uniform_matrix_44d(const GLint location) const
  {
    std::array<GLdouble, 16> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 6>  uniform_matrix_23d(const GLint location) const
  {
    std::array<GLdouble, 6> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 6>  uniform_matrix_32d(const GLint location) const
  {
    std::array<GLdouble, 6> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 8>  uniform_matrix_24d(const GLint location) const
  {
    std::array<GLdouble, 8> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 8>  uniform_matrix_42d(const GLint location) const
  {
    std::array<GLdouble, 8> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 12> uniform_matrix_34d(const GLint location) const
  {
    std::array<GLdouble, 12> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }
  [[nodiscard]]
  std::array<GLdouble, 12> uniform_matrix_43d(const GLint location) const
  {
    std::array<GLdouble, 12> value {};
    glGetUniformdv(id_, location, &value[0]);
    return value;
  }

  static GLuint subroutine_uniform(const GLenum shader_type, const GLint location)
  {
    GLuint result;
    glGetUniformSubroutineuiv(shader_type, location, &result);
    return result;
  }

  [[nodiscard]]
  GLsizei active_subroutine_count                 (const GLenum shader_type) const
  {
    return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINES);
  }
  [[nodiscard]]
  GLsizei active_subroutine_max_length            (const GLenum shader_type) const
  {
    return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINE_MAX_LENGTH);
  }
  [[nodiscard]]
  GLsizei active_subroutine_uniform_count         (const GLenum shader_type) const
  {
    return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINE_UNIFORMS);
  }
  [[nodiscard]]
  GLsizei active_subroutine_uniform_max_length    (const GLenum shader_type) const
  {
    return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH);
  }
  [[nodiscard]]
  GLsizei active_subroutine_uniform_location_count(const GLenum shader_type) const
  {
    return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS);
  }
  
  // 11.1.1 Vertex attributes.
  void   set_attribute_location(const std::string& attribute_name, const GLuint location) const
  {
    // Note: Link must be called after. Always prefer to specify this explicitly in glsl whenever possible.
    glBindAttribLocation(id_, location, attribute_name.c_str());
  }
  [[nodiscard]]
  GLuint attribute_location    (const std::string& attribute_name) const
  {
    return glGetAttribLocation(id_, attribute_name.c_str());
  }

  [[nodiscard]]
  std::tuple<std::string, GLenum, GLsizei> active_attribute(const GLuint location) const
  {
    std::tuple<std::string, GLenum, GLsizei> result;
    auto& name = std::get<0>(result);
    auto& type = std::get<1>(result);
    auto& size = std::get<2>(result);
    name.resize(active_attribute_max_length());
    glGetActiveAttrib(id_, location, static_cast<GLsizei>(name.size()), nullptr, &size, &type, &name[0]);
    return result;
  }
  
  // 11.1.2 Transform feedback variables.
  void set_transform_feedback_varyings(const std::vector<std::string>& varyings, const GLenum buffer_mode)
  {
    std::vector<const char*> varyings_c(varyings.size());
    std::transform(varyings.begin(), varyings.end(), varyings_c.begin(), [&](const std::string& varying)
    {
      return varying.c_str();
    });
    glTransformFeedbackVaryings(id_, static_cast<GLsizei>(varyings.size()), varyings_c.data(), buffer_mode);
  }

  [[nodiscard]]
  std::tuple<std::string, GLenum, GLsizei> transform_feedback_varying(const GLuint index) const
  {
    std::tuple<std::string, GLenum, GLsizei> result;
    auto& name = std::get<0>(result);
    auto& type = std::get<1>(result);
    auto& size = std::get<2>(result);
    name.resize(transform_feedback_varying_max_length());
    glGetTransformFeedbackVarying(id_, index, static_cast<GLsizei>(name.size()), nullptr, &size, &type, &name[0]);
    return result;
  }
  
  // 11.1.3 Shader execution.
  [[nodiscard]]
  bool validate() const
  {
    glValidateProgram(id_);
    return validation_status();
  }
  
  // 15.2 Fragment shaders.
  void   set_frag_data_location        (const GLuint color_number,                     const std::string& name) const
  {
    glBindFragDataLocation(id_, color_number, name.c_str());
  }
  void   set_frag_data_location_indexed(const GLuint color_number, const GLuint index, const std::string& name) const
  {
    glBindFragDataLocationIndexed(id_, color_number, index, name.c_str());
  }
  [[nodiscard]]
  GLuint frag_data_location            (const std::string& name) const
  {
    return glGetFragDataLocation(id_, name.c_str());
  }
  [[nodiscard]]
  GLuint frag_data_index               (const std::string& name) const
  {
    return glGetFragDataIndex(id_, name.c_str());
  }

  // X Extended Functionality - Type-inferring uniform setters.
  template<typename type>
  void set_uniform       (GLint location, const type& value)
  {
    static_assert(sizeof(type) == 0, "Type not allowed.");
  }
  template<typename type>
  void set_uniform_vector(GLint location, const std::vector<type>& values)
  {
    static_assert(sizeof(type) == 0, "Type not allowed.");
  }

  static const GLenum native_type = GL_PROGRAM;

  [[nodiscard]]
  GLuint id() const
  {
    return id_;
  }

protected:
  [[nodiscard]]
  GLint              get_parameter                             (                                    const GLenum parameter) const
  {
    GLint result;
    glGetProgramiv(id_, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint              get_interface_parameter                   (const GLenum interface,             const GLenum parameter) const
  {
    GLint result;
    glGetProgramInterfaceiv(id_, interface, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint              get_active_uniform_parameter              (const GLuint index,                 const GLenum parameter) const
  {
    GLint result;
    glGetActiveUniformsiv(id_, 1, &index, parameter, &result);
    return result;
  }
  [[nodiscard]]
  std::vector<GLint> get_active_uniform_parameters             (const std::vector<GLuint>& indices, const GLenum parameter) const
  {
    std::vector<GLint> result(indices.size());
    glGetActiveUniformsiv(id_, static_cast<GLsizei>(indices.size()), indices.data(), parameter, result.data());
    return result;
  }
  [[nodiscard]]
  GLint              get_active_uniform_block_parameter        (const GLuint index,                 const GLenum parameter) const
  {
    GLint result;
    glGetActiveUniformBlockiv(id_, index, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint              get_active_atomic_counter_buffer_parameter(const GLuint index,                 const GLenum parameter) const
  {
    GLint result;
    glGetActiveAtomicCounterBufferiv(id_, index, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint              get_program_stage_parameter               (const GLenum shader_type,           const GLenum parameter) const
  {
    GLint result;
    glGetProgramStageiv(id_, shader_type, parameter, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};

// X Extended Functionality - Type-inferring uniform setters.
template <>                                                                          
inline void program::set_uniform       (const GLint location, const bool&                  value )
{
  set_uniform_1i(location, value);
}
template <>                                                                          
inline void program::set_uniform       (const GLint location, const GLint&                 value )
{
  set_uniform_1i(location, value);
}
template <>                                                                          
inline void program::set_uniform_vector(const GLint location, const std::vector<GLint>&    values)
{
  set_uniform_1i(location, values);
}
template <>                                                                          
inline void program::set_uniform       (const GLint location, const GLuint&                value )
{
  set_uniform_1ui(location, value);
}
template <>                                                                          
inline void program::set_uniform_vector(const GLint location, const std::vector<GLuint>&   values)
{
  set_uniform_1ui(location, values);
}
template <>                                                                          
inline void program::set_uniform       (const GLint location, const GLfloat&               value )
{
  set_uniform_1f(location, value);
}
template <>                                                                          
inline void program::set_uniform_vector(const GLint location, const std::vector<GLfloat>&  values)
{
  set_uniform_1f(location, values);
}
template <>                                                                          
inline void program::set_uniform       (const GLint location, const GLdouble&              value )
{
  set_uniform_1d(location, value);
}
template <>
inline void program::set_uniform_vector(const GLint location, const std::vector<GLdouble>& values)
{
  set_uniform_1d(location, values);
}
}

#endif
