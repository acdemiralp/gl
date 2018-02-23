//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/program.hpp>

#include <utility>

namespace gl
{
// 7.3 Program objects.
program::program (): id_(glCreateProgram())
{

}
program::program (GLuint id): id_(id), managed_(false)
{

}
program::program (program&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
{
  temp.id_      = invalid_id;
  temp.managed_ = false;
}
program::~program()
{
  if (managed_ && id_ != invalid_id)
    glDeleteProgram(id_);
}

program& program::operator=(program&& temp) noexcept
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

void program::attach_shader(const shader& shader) const
{
  glAttachShader(id_, shader.id());
}
void program::detach_shader(const shader& shader) const
{
  glDetachShader(id_, shader.id());
}
bool program::link         () const
{
  glLinkProgram(id_);
  return link_status();
}
void program::use          () const
{
  glUseProgram(id_);
}
void program::unuse        ()
{
  glUseProgram(0);
}
bool program::is_valid     () const
{
  return glIsProgram(id_) != 0;
}

void program::set_separable         (bool separable)
{
  glProgramParameteri(id_, GL_PROGRAM_SEPARABLE, separable);
}
void program::set_binary_retrievable(bool binary_retrievable)
{
  glProgramParameteri(id_, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, binary_retrievable);
}

// 7.3.1 Program interfaces.
GLsizei     program::interface_active_resources               (GLenum interface) const
{
  return get_interface_parameter(interface, GL_ACTIVE_RESOURCES);
}
GLsizei     program::interface_max_name_length                (GLenum interface) const
{
  return get_interface_parameter(interface, GL_MAX_NAME_LENGTH);
}
GLsizei     program::interface_max_active_variable_count      (GLenum interface) const
{
  return get_interface_parameter(interface, GL_MAX_NUM_ACTIVE_VARIABLES);
}
GLsizei     program::interface_max_compatible_subroutine_count(GLenum interface) const
{
  return get_interface_parameter(interface, GL_MAX_NUM_COMPATIBLE_SUBROUTINES);
}
GLuint      program::resource_index                           (GLenum interface, const std::string& name) const
{
  return glGetProgramResourceIndex(id_, interface, name.c_str());
}
std::string program::resource_name                            (GLenum interface, GLuint index) const
{
  std::string name;
  GLsizei length;
  name.resize(interface_max_name_length(interface));
  glGetProgramResourceName(id_, interface, index, static_cast<GLsizei>(name.size()), &length, &name[0]);
  name.resize(length);
  return name;
}
GLint       program::resource_location                        (GLenum interface, const std::string& name) const
{
  return glGetProgramResourceLocation(id_, interface, name.c_str());
}
GLint       program::resource_location_index                  (GLenum interface, const std::string& name) const
{
  return glGetProgramResourceLocationIndex(id_, interface, name.c_str());
}

GLint              program::resource_parameter (GLenum interface, GLuint index, GLenum parameter) const
{
  GLint result;
  glGetProgramResourceiv(id_, interface, index, 1, &parameter, 1, nullptr, &result);
  return result;
}
std::vector<GLint> program::resource_parameters(GLenum interface, GLuint index, const std::vector<GLenum>& parameters) const
{
  std::vector<GLint> result(parameters.size());
  glGetProgramResourceiv(id_, interface, index, static_cast<GLsizei>(parameters.size()), parameters.data(), static_cast<GLsizei>(result.size()), nullptr, result.data());
  return result;
}

// 7.6 Uniform variables.
GLint program::uniform_location(const std::string& name) const
{
  return glGetUniformLocation(id_, name.c_str());
}

std::string program::active_uniform_name(GLuint index) const
{
  std::string result;
  result.resize(active_uniform_name_length(index));
  glGetActiveUniformName(id_, index, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
  return result;
}

std::vector<GLuint> program::uniform_indices(GLsizei count, const std::vector<std::string>& names) const
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

std::tuple<std::string, GLenum, GLsizei> program::active_uniform(GLuint index) const
{
  std::tuple<std::string, GLenum, GLsizei> result;
  auto& name = std::get<0>(result);
  auto& type = std::get<1>(result);
  auto& size = std::get<2>(result);
  name.resize(active_uniform_max_length());
  glGetActiveUniform(id_, index, static_cast<GLsizei>(name.size()), nullptr, &size, &type, &name[0]);
  return result;
}

GLsizei program::active_uniform_name_length                (GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_NAME_LENGTH);
}
GLenum  program::active_uniform_type                       (GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_TYPE);
}
GLuint  program::active_uniform_offset                     (GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_OFFSET);
}
GLsizei program::active_uniform_size                       (GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_SIZE);
}
GLuint  program::active_uniform_block_index                (GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_BLOCK_INDEX);
}
GLsizei program::active_uniform_array_stride               (GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_ARRAY_STRIDE);
}
GLsizei program::active_uniform_matrix_stride              (GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_MATRIX_STRIDE);
}
bool    program::active_uniform_is_row_major               (GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_IS_ROW_MAJOR) != 0;
}
GLuint  program::active_uniform_atomic_counter_buffer_index(GLuint index) const
{
  return get_active_uniform_parameter(index, GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX);
}

std::vector<GLsizei> program::active_uniforms_name_lengths                 (const std::vector<GLuint>& indices) const
{
  return get_active_uniform_parameters(indices, GL_UNIFORM_NAME_LENGTH);
}
std::vector<GLenum>  program::active_uniforms_types                        (const std::vector<GLuint>& indices) const
{
  auto result = get_active_uniform_parameters(indices, GL_UNIFORM_TYPE);
  return std::vector<GLenum>(result.begin(), result.end());
}
std::vector<GLuint>  program::active_uniforms_offsets                      (const std::vector<GLuint>& indices) const
{
  auto result = get_active_uniform_parameters(indices, GL_UNIFORM_OFFSET);
  return std::vector<GLuint>(result.begin(), result.end());
}
std::vector<GLsizei> program::active_uniforms_sizes                        (const std::vector<GLuint>& indices) const
{
  return get_active_uniform_parameters(indices, GL_UNIFORM_SIZE);
}
std::vector<GLuint>  program::active_uniforms_block_indices                (const std::vector<GLuint>& indices) const
{
  auto result = get_active_uniform_parameters(indices, GL_UNIFORM_BLOCK_INDEX);
  return std::vector<GLuint>(result.begin(), result.end());
}
std::vector<GLsizei> program::active_uniforms_array_strides                (const std::vector<GLuint>& indices) const
{
  return get_active_uniform_parameters(indices, GL_UNIFORM_ARRAY_STRIDE);
}
std::vector<GLsizei> program::active_uniforms_matrix_strides               (const std::vector<GLuint>& indices) const
{
  return get_active_uniform_parameters(indices, GL_UNIFORM_MATRIX_STRIDE);
}
std::vector<bool>    program::active_uniforms_are_row_majors               (const std::vector<GLuint>& indices) const
{
  auto result = get_active_uniform_parameters(indices, GL_UNIFORM_IS_ROW_MAJOR);
  return std::vector<bool>(result.begin(), result.end()); // Warning C4800 inside this line.
}
std::vector<GLuint>  program::active_uniforms_atomic_counter_buffer_indices(const std::vector<GLuint>& indices) const
{
  auto result = get_active_uniform_parameters(indices, GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX);
  return std::vector<GLuint>(result.begin(), result.end());
}

GLuint program::uniform_block_index(const std::string& name) const
{
  return glGetUniformBlockIndex(id_, name.c_str());
}

std::string program::active_uniform_block_name(GLuint index) const
{
  std::string result;
  result.resize(active_uniform_block_name_length(index));
  glGetActiveUniformBlockName(id_, index, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
  return result;
}

GLuint  program::active_uniform_block_binding                                        (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_BINDING);
}
GLuint  program::active_uniform_block_data_size                                      (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_DATA_SIZE);
}
GLuint  program::active_uniform_block_name_length                                    (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_NAME_LENGTH);
}
GLsizei program::active_uniform_block_uniform_count                                  (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS);
}
bool    program::active_uniform_block_is_referenced_by_vertex_shader                 (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER) != 0;
}
bool    program::active_uniform_block_is_referenced_by_fragment_shader               (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER) != 0;
}
bool    program::active_uniform_block_is_referenced_by_compute_shader                (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER) != 0;
}
bool    program::active_uniform_block_is_referenced_by_geometry_shader               (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER) != 0;
}
bool    program::active_uniform_block_is_referenced_by_tessellation_control_shader   (GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER) != 0;
}
bool    program::active_uniform_block_is_referenced_by_tessellation_evaluation_shader(GLuint index) const
{
  return get_active_uniform_block_parameter(index, GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER) != 0;
}

std::vector<GLuint> program::active_uniform_block_uniform_indices(GLuint index) const
{
  std::vector<GLint> result(active_uniform_block_uniform_count(index));
  glGetActiveUniformBlockiv(id_, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &result[0]);
  return std::vector<GLuint>(result.begin(), result.end());
}

GLuint  program::active_atomic_counter_buffer_data_size                                      (GLuint index) const
{
  return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE);
}
GLsizei program::active_atomic_counter_buffer_counters                                       (GLuint index) const
{
  return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS);
}
bool    program::active_atomic_counter_buffer_is_referenced_by_vertex_shader                 (GLuint index) const
{
  return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER) != 0;
}
bool    program::active_atomic_counter_buffer_is_referenced_by_fragment_shader               (GLuint index) const
{
  return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER) != 0;
}
bool    program::active_atomic_counter_buffer_is_referenced_by_compute_shader                (GLuint index) const
{
  return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER) != 0;
}
bool    program::active_atomic_counter_buffer_is_referenced_by_geometry_shader               (GLuint index) const
{
  return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER) != 0;
}
bool    program::active_atomic_counter_buffer_is_referenced_by_tessellation_control_shader   (GLuint index) const
{
  return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER) != 0;
}
bool    program::active_atomic_counter_buffer_is_referenced_by_tessellation_evaluation_shader(GLuint index) const
{
  return get_active_atomic_counter_buffer_parameter(index, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER) != 0;
}

std::vector<GLuint> program::active_atomic_counter_buffer_counter_indices(GLuint index) const
{
  std::vector<GLint> result(active_uniform_block_uniform_count(index));
  glGetActiveUniformBlockiv(id_, index, GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES, &result[0]);
  return std::vector<GLuint>(result.begin(), result.end());
}

void program::set_uniform_1i(GLint location, GLint value)
{
  glProgramUniform1i(id_, location, value);
}
void program::set_uniform_1i(GLint location, const std::vector<GLint>&   value)
{
  glProgramUniform1iv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_2i(GLint location, const std::array<GLint, 2>& value)
{
  glProgramUniform2i(id_, location, value[0], value[1]);
}
void program::set_uniform_2i(GLint location, const std::vector<GLint>&   value)
{
  glProgramUniform2iv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_3i(GLint location, const std::array<GLint, 3>& value)
{
  glProgramUniform3i(id_, location, value[0], value[1], value[2]);
}
void program::set_uniform_3i(GLint location, const std::vector<GLint>&   value)
{
  glProgramUniform3iv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_4i(GLint location, const std::array<GLint, 4>& value)
{
  glProgramUniform4i(id_, location, value[0], value[1], value[2], value[3]);
}
void program::set_uniform_4i(GLint location, const std::vector<GLint>&   value)
{
  glProgramUniform4iv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}

void program::set_uniform_1ui(GLint location, GLuint value)
{
  glProgramUniform1ui(id_, location, value);
}
void program::set_uniform_1ui(GLint location, const std::vector<GLuint>&   value)
{
  glProgramUniform1uiv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_2ui(GLint location, const std::array<GLuint, 2>& value)
{
  glProgramUniform2ui(id_, location, value[0], value[1]);
}
void program::set_uniform_2ui(GLint location, const std::vector<GLuint>&   value)
{
  glProgramUniform2uiv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_3ui(GLint location, const std::array<GLuint, 3>& value)
{
  glProgramUniform3ui(id_, location, value[0], value[1], value[2]);
}
void program::set_uniform_3ui(GLint location, const std::vector<GLuint>&   value)
{
  glProgramUniform3uiv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_4ui(GLint location, const std::array<GLuint, 4>& value)
{
  glProgramUniform4ui(id_, location, value[0], value[1], value[2], value[3]);
}
void program::set_uniform_4ui(GLint location, const std::vector<GLuint>&   value)
{
  glProgramUniform4uiv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}

void program::set_uniform_1f(GLint location, GLfloat value)
{
  glProgramUniform1f(id_, location, value);
}
void program::set_uniform_1f(GLint location, const std::vector<GLfloat>&   value)
{
  glProgramUniform1fv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_2f(GLint location, const std::array<GLfloat, 2>& value)
{
  glProgramUniform2f(id_, location, value[0], value[1]);
}
void program::set_uniform_2f(GLint location, const std::vector<GLfloat>&   value)
{
  glProgramUniform2fv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_3f(GLint location, const std::array<GLfloat, 3>& value)
{
  glProgramUniform3f(id_, location, value[0], value[1], value[2]);
}
void program::set_uniform_3f(GLint location, const std::vector<GLfloat>&   value)
{
  glProgramUniform3fv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_4f(GLint location, const std::array<GLfloat, 4>& value)
{
  glProgramUniform4f(id_, location, value[0], value[1], value[2], value[3]);
}
void program::set_uniform_4f(GLint location, const std::vector<GLfloat>&   value)
{
  glProgramUniform4fv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}

void program::set_uniform_1d(GLint location, GLdouble value)
{
  glProgramUniform1d(id_, location, value);
}
void program::set_uniform_1d(GLint location, const std::vector<GLdouble>&   value)
{
  glProgramUniform1dv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_2d(GLint location, const std::array<GLdouble, 2>& value)
{
  glProgramUniform2d(id_, location, value[0], value[1]);
}
void program::set_uniform_2d(GLint location, const std::vector<GLdouble>&   value)
{
  glProgramUniform2dv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_3d(GLint location, const std::array<GLdouble, 3>& value)
{
  glProgramUniform3d(id_, location, value[0], value[1], value[2]);
}
void program::set_uniform_3d(GLint location, const std::vector<GLdouble>&   value)
{
  glProgramUniform3dv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}
void program::set_uniform_4d(GLint location, const std::array<GLdouble, 4>& value)
{
  glProgramUniform4d(id_, location, value[0], value[1], value[2], value[3]);
}
void program::set_uniform_4d(GLint location, const std::vector<GLdouble>&   value)
{
  glProgramUniform4dv(id_, location, static_cast<GLsizei>(value.size()), value.data());
}

void program::set_uniform_matrix_22f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix2fv(id_, location, static_cast<GLsizei>(value.size() / 4), transpose, value.data());
}
void program::set_uniform_matrix_33f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix3fv(id_, location, static_cast<GLsizei>(value.size() / 9), transpose, value.data());
}
void program::set_uniform_matrix_44f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix4fv(id_, location, static_cast<GLsizei>(value.size() / 16), transpose, value.data());
}
void program::set_uniform_matrix_23f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix2x3fv(id_, location, static_cast<GLsizei>(value.size() / 6), transpose, value.data());
}
void program::set_uniform_matrix_32f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix3x2fv(id_, location, static_cast<GLsizei>(value.size() / 6), transpose, value.data());
}
void program::set_uniform_matrix_24f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix2x4fv(id_, location, static_cast<GLsizei>(value.size() / 8), transpose, value.data());
}
void program::set_uniform_matrix_42f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix4x2fv(id_, location, static_cast<GLsizei>(value.size() / 8), transpose, value.data());
}
void program::set_uniform_matrix_34f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix3x4fv(id_, location, static_cast<GLsizei>(value.size() / 12), transpose, value.data());
}
void program::set_uniform_matrix_43f(GLint location, const std::vector<GLfloat>& value, bool transpose)
{
  glProgramUniformMatrix4x3fv(id_, location, static_cast<GLsizei>(value.size() / 12), transpose, value.data());
}

void program::set_uniform_matrix_22d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix2dv(id_, location, static_cast<GLsizei>(value.size() / 4), transpose, value.data());
}
void program::set_uniform_matrix_33d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix3dv(id_, location, static_cast<GLsizei>(value.size() / 9), transpose, value.data());
}
void program::set_uniform_matrix_44d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix4dv(id_, location, static_cast<GLsizei>(value.size() / 16), transpose, value.data());
}
void program::set_uniform_matrix_23d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix2x3dv(id_, location, static_cast<GLsizei>(value.size() / 6), transpose, value.data());
}
void program::set_uniform_matrix_32d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix3x2dv(id_, location, static_cast<GLsizei>(value.size() / 6), transpose, value.data());
}
void program::set_uniform_matrix_24d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix2x4dv(id_, location, static_cast<GLsizei>(value.size() / 8), transpose, value.data());
}
void program::set_uniform_matrix_42d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix4x2dv(id_, location, static_cast<GLsizei>(value.size() / 8), transpose, value.data());
}
void program::set_uniform_matrix_34d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix3x4dv(id_, location, static_cast<GLsizei>(value.size() / 12), transpose, value.data());
}
void program::set_uniform_matrix_43d(GLint location, const std::vector<GLdouble>& value, bool transpose)
{
  glProgramUniformMatrix4x3dv(id_, location, static_cast<GLsizei>(value.size() / 12), transpose, value.data());
}

#ifdef GL_ARB_bindless_texture
void program::set_uniform_handle(GLint location, const texture_handle&              value)
{
  glProgramUniformHandleui64ARB(id_, location, value.id());
}
void program::set_uniform_handle(GLint location, const std::vector<texture_handle>& value)
{
  std::vector<GLuint64> ids(value.size());
  std::transform(value.begin(), value.end(), ids.begin(), [&](const texture_handle& iteratee)
  {
    return iteratee.id();
  });
  glProgramUniformHandleui64vARB(id_, location, static_cast<GLsizei>(ids.size()), ids.data());
}
void program::set_uniform_handle(GLint location, const image_handle&                value)
{
  glProgramUniformHandleui64ARB(id_, location, value.id());
}
void program::set_uniform_handle(GLint location, const std::vector<image_handle>&   value)
{
  std::vector<GLuint64> ids(value.size());
  std::transform(value.begin(), value.end(), ids.begin(), [&](const image_handle& iteratee)
  {
    return iteratee.id();
  });
  glProgramUniformHandleui64vARB(id_, location, static_cast<GLsizei>(ids.size()), ids.data());
}
#endif

void program::set_uniform_block_binding(GLuint index, GLuint binding)
{
  glUniformBlockBinding(id_, index, binding);
}

// 7.8 Shader buffer variables.
void program::set_shader_storage_block_binding(GLuint index, GLuint binding)
{
  glShaderStorageBlockBinding(id_, index, binding);
}

// 7.9 Subroutine uniform variables.
GLint               program::subroutine_uniform_location                          (GLenum shader_type, const std::string& name) const
{
  return glGetSubroutineUniformLocation(id_, shader_type, name.c_str());
}
GLuint              program::subroutine_index                                     (GLenum shader_type, const std::string& name) const
{
  return glGetSubroutineIndex(id_, shader_type, name.c_str());
}
std::string         program::active_subroutine_name                               (GLenum shader_type, GLuint subroutine_index) const
{
  std::string result;
  result.resize(active_subroutine_max_length(shader_type));
  glGetActiveSubroutineName(id_, shader_type, subroutine_index, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
  return result;
}
std::string         program::active_subroutine_uniform_name                       (GLenum shader_type, GLuint subroutine_index) const
{
  std::string result;
  result.resize(active_subroutine_uniform_max_length(shader_type));
  glGetActiveSubroutineUniformName(id_, shader_type, subroutine_index, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
  return result;
}
std::vector<GLuint> program::active_subroutine_uniform_compatible_subroutines     (GLenum shader_type, GLuint subroutine_index) const
{
  std::vector<GLint> result(active_subroutine_uniform_compatible_subroutine_count(shader_type, subroutine_index));
  glGetActiveSubroutineUniformiv(id_, shader_type, subroutine_index, GL_COMPATIBLE_SUBROUTINES, result.data());
  return std::vector<GLuint>(result.begin(), result.end());
}
GLsizei             program::active_subroutine_uniform_compatible_subroutine_count(GLenum shader_type, GLuint subroutine_index) const
{
  GLint result;
  glGetActiveSubroutineUniformiv(id_, shader_type, subroutine_index, GL_NUM_COMPATIBLE_SUBROUTINES, &result);
  return result;
}
void                program::set_active_subroutine_uniforms                       (GLenum shader_type, const std::vector<GLuint>& subroutine_indices)
{
  glUniformSubroutinesuiv(shader_type, active_subroutine_uniform_location_count(shader_type), subroutine_indices.data());
}

// 7.13 Program queries.
bool    program::delete_status                           () const
{
  return get_parameter(GL_DELETE_STATUS) != 0;
}
bool    program::link_status                             () const
{
  return get_parameter(GL_LINK_STATUS) != 0;
}
bool    program::validation_status                       () const
{
  return get_parameter(GL_VALIDATE_STATUS) != 0;
}
GLsizei program::info_log_length                         () const
{
  return get_parameter(GL_INFO_LOG_LENGTH);
}
GLsizei program::attached_shader_count                   () const
{
  return get_parameter(GL_ATTACHED_SHADERS);
}
GLsizei program::active_attribute_count                  () const
{
  return get_parameter(GL_ACTIVE_ATTRIBUTES);
}
GLsizei program::active_attribute_max_length             () const
{
  return get_parameter(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
}
GLsizei program::active_uniform_count                    () const
{
  return get_parameter(GL_ACTIVE_UNIFORMS);
}
GLsizei program::active_uniform_max_length               () const
{
  return get_parameter(GL_ACTIVE_UNIFORM_MAX_LENGTH);
}
GLsizei program::active_uniform_block_count              () const
{
  return get_parameter(GL_ACTIVE_UNIFORM_BLOCKS);
}
GLsizei program::active_uniform_block_max_name_length    () const
{
  return get_parameter(GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH);
}
GLenum  program::transform_feedback_buffer_mode          () const
{
  return get_parameter(GL_TRANSFORM_FEEDBACK_BUFFER_MODE);
}
GLsizei program::transform_feedback_varying_count        () const
{
  return get_parameter(GL_TRANSFORM_FEEDBACK_VARYINGS);
}
GLsizei program::transform_feedback_varying_max_length   () const
{
  return get_parameter(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH);
}
GLsizei program::geometry_vertices_out                   () const
{
  return get_parameter(GL_GEOMETRY_VERTICES_OUT);
}
GLenum  program::geometry_input_type                     () const
{
  return get_parameter(GL_GEOMETRY_INPUT_TYPE);
}
GLenum  program::geometry_output_type                    () const
{
  return get_parameter(GL_GEOMETRY_OUTPUT_TYPE);
}
GLsizei program::geometry_shader_invocations             () const
{
  return get_parameter(GL_GEOMETRY_SHADER_INVOCATIONS);
}
GLsizei program::tessellation_control_output_vertex_count() const
{
  return get_parameter(GL_TESS_CONTROL_OUTPUT_VERTICES);
}
GLenum  program::tessellation_generation_mode            () const
{
  return get_parameter(GL_TESS_GEN_MODE);
}
GLenum  program::tessellation_generation_spacing         () const
{
  return get_parameter(GL_TESS_GEN_SPACING);
}
GLenum  program::tessellation_generation_vertex_order    () const
{
  return get_parameter(GL_TESS_GEN_VERTEX_ORDER);
}
bool    program::tessellation_generation_point_mode      () const
{
  return get_parameter(GL_TESS_GEN_POINT_MODE) != 0;
}
bool    program::is_separable                            () const
{
  return get_parameter(GL_PROGRAM_SEPARABLE) != 0;
}
bool    program::is_binary_retrievable                   () const
{
  return get_parameter(GL_PROGRAM_BINARY_RETRIEVABLE_HINT) != 0;
}
GLsizei program::active_atomic_counter_buffer_count      () const
{
  return get_parameter(GL_ACTIVE_ATOMIC_COUNTER_BUFFERS);
}
GLsizei program::binary_length                           () const
{
  return get_parameter(GL_PROGRAM_BINARY_LENGTH);
}

std::array<GLint, 3> program::compute_work_group_size() const
{
  std::array<GLint, 3> result;
  glGetProgramiv(id_, GL_COMPUTE_WORK_GROUP_SIZE, result.data());
  return result;
}

std::vector<shader> program::attached_shaders() const
{
  std::vector<shader> shaders;
  std::vector<GLuint> shader_ids(attached_shader_count());
  glGetAttachedShaders(id_, static_cast<GLsizei>(shader_ids.size()), nullptr, shader_ids.data());
  for (auto shader_id : shader_ids)
    shaders.emplace_back(shader(shader_id));
  return shaders;
}

std::string program::info_log() const
{
  std::string result;
  result.resize(info_log_length());
  glGetProgramInfoLog(id_, static_cast<GLsizei>(result.size()), nullptr, &result[0]);
  return result;
}

GLint                    program::uniform_1i (GLint location) const
{
  GLint value;
  glGetUniformiv(id_, location, &value);
  return value;
}
std::array<GLint, 2>     program::uniform_2i (GLint location) const
{
  std::array<GLint, 2> value;
  glGetUniformiv(id_, location, &value[0]);
  return value;
}
std::array<GLint, 3>     program::uniform_3i (GLint location) const
{
  std::array<GLint, 3> value;
  glGetUniformiv(id_, location, &value[0]);
  return value;
}
std::array<GLint, 4>     program::uniform_4i (GLint location) const
{
  std::array<GLint, 4> value;
  glGetUniformiv(id_, location, &value[0]);
  return value;
}
                       
GLuint                   program::uniform_1ui(GLint location) const
{
  GLuint value;
  glGetUniformuiv(id_, location, &value);
  return value;
}
std::array<GLuint, 2>    program::uniform_2ui(GLint location) const
{
  std::array<GLuint, 2> value;
  glGetUniformuiv(id_, location, &value[0]);
  return value;
}
std::array<GLuint, 3>    program::uniform_3ui(GLint location) const
{
  std::array<GLuint, 3> value;
  glGetUniformuiv(id_, location, &value[0]);
  return value;
}
std::array<GLuint, 4>    program::uniform_4ui(GLint location) const
{
  std::array<GLuint, 4> value;
  glGetUniformuiv(id_, location, &value[0]);
  return value;
}

GLfloat                  program::uniform_1f (GLint location) const
{
  GLfloat value;
  glGetUniformfv(id_, location, &value);
  return value;
}
std::array<GLfloat, 2>   program::uniform_2f (GLint location) const
{
  std::array<GLfloat, 2> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 3>   program::uniform_3f (GLint location) const
{
  std::array<GLfloat, 3> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 4>   program::uniform_4f (GLint location) const
{
  std::array<GLfloat, 4> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}

GLdouble                 program::uniform_1d (GLint location) const
{
  GLdouble value;
  glGetUniformdv(id_, location, &value);
  return value;
}
std::array<GLdouble, 2>  program::uniform_2d (GLint location) const
{
  std::array<GLdouble, 2> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 3>  program::uniform_3d (GLint location) const
{
  std::array<GLdouble, 3> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 4>  program::uniform_4d (GLint location) const
{
  std::array<GLdouble, 4> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}

std::array<GLfloat, 4>   program::uniform_matrix_22f(GLint location) const
{
  std::array<GLfloat, 4> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 9>   program::uniform_matrix_33f(GLint location) const
{
  std::array<GLfloat, 9> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 16>  program::uniform_matrix_44f(GLint location) const
{
  std::array<GLfloat, 16> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 6>   program::uniform_matrix_23f(GLint location) const
{
  std::array<GLfloat, 6> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 6>   program::uniform_matrix_32f(GLint location) const
{
  std::array<GLfloat, 6> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 8>   program::uniform_matrix_24f(GLint location) const
{
  std::array<GLfloat, 8> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 8>   program::uniform_matrix_42f(GLint location) const
{
  std::array<GLfloat, 8> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 12>  program::uniform_matrix_34f(GLint location) const
{
  std::array<GLfloat, 12> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}
std::array<GLfloat, 12>  program::uniform_matrix_43f(GLint location) const
{
  std::array<GLfloat, 12> value;
  glGetUniformfv(id_, location, &value[0]);
  return value;
}

std::array<GLdouble, 4>  program::uniform_matrix_22d(GLint location) const
{
  std::array<GLdouble, 4> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 9>  program::uniform_matrix_33d(GLint location) const
{
  std::array<GLdouble, 9> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 16> program::uniform_matrix_44d(GLint location) const
{
  std::array<GLdouble, 16> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 6>  program::uniform_matrix_23d(GLint location) const
{
  std::array<GLdouble, 6> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 6>  program::uniform_matrix_32d(GLint location) const
{
  std::array<GLdouble, 6> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 8>  program::uniform_matrix_24d(GLint location) const
{
  std::array<GLdouble, 8> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 8>  program::uniform_matrix_42d(GLint location) const
{
  std::array<GLdouble, 8> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 12> program::uniform_matrix_34d(GLint location) const
{
  std::array<GLdouble, 12> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}
std::array<GLdouble, 12> program::uniform_matrix_43d(GLint location) const
{
  std::array<GLdouble, 12> value;
  glGetUniformdv(id_, location, &value[0]);
  return value;
}

GLuint program::subroutine_uniform(GLenum shader_type, GLint location)
{
  GLuint result;
  glGetUniformSubroutineuiv(shader_type, location, &result);
  return result;
}

GLsizei program::active_subroutine_count                 (GLenum shader_type) const
{
  return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINES);
}
GLsizei program::active_subroutine_max_length            (GLenum shader_type) const
{
  return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINE_MAX_LENGTH);
}
GLsizei program::active_subroutine_uniform_count         (GLenum shader_type) const
{
  return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINE_UNIFORMS);
}
GLsizei program::active_subroutine_uniform_max_length    (GLenum shader_type) const
{
  return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH);
}
GLsizei program::active_subroutine_uniform_location_count(GLenum shader_type) const
{
  return get_program_stage_parameter(shader_type, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS);
}

// 11.1.1 Vertex attributes.
void   program::set_attribute_location(const std::string& attribute_name, GLuint location)
{
  // Note: Link must be called after. Always prefer to specify this explicitly in glsl whenever possible.
  glBindAttribLocation(id_, location, attribute_name.c_str());
}
GLuint program::attribute_location    (const std::string& attribute_name) const
{
  return glGetAttribLocation(id_, attribute_name.c_str());
}

std::tuple<std::string, GLenum, GLsizei> program::active_attribute(GLuint location) const
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
void program::set_transform_feedback_varyings(const std::vector<std::string>& varyings, GLenum buffer_mode)
{
  std::vector<const char*> varyings_c(varyings.size());
  std::transform(varyings.begin(), varyings.end(), varyings_c.begin(), [&](const std::string& varying)
  {
    return varying.c_str();
  });
  glTransformFeedbackVaryings(id_, static_cast<GLsizei>(varyings.size()), varyings_c.data(), buffer_mode);
}

std::tuple<std::string, GLenum, GLsizei> program::transform_feedback_varying(GLuint index) const
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
bool program::validate() const
{
  glValidateProgram(id_);
  return validation_status();
}

// 15.2 Fragment shaders.
void   program::set_frag_data_location        (GLuint color_number,               const std::string& name)
{
  glBindFragDataLocation(id_, color_number, name.c_str());
}
void   program::set_frag_data_location_indexed(GLuint color_number, GLuint index, const std::string& name)
{
  glBindFragDataLocationIndexed(id_, color_number, index, name.c_str());
}
GLuint program::frag_data_location            (const std::string& name) const
{
  return glGetFragDataLocation(id_, name.c_str());
}
GLuint program::frag_data_index               (const std::string& name) const
{
  return glGetFragDataIndex(id_, name.c_str());
}

GLuint program::id() const
{
  return id_;
}

GLint              program::get_parameter                             (                                 GLenum parameter) const
{
  GLint result;
  glGetProgramiv(id_, parameter, &result);
  return result;
}
GLint              program::get_interface_parameter                   (GLenum              interface,   GLenum parameter) const
{
  GLint result;
  glGetProgramInterfaceiv(id_, interface, parameter, &result);
  return result;
}
GLint              program::get_active_uniform_parameter              (GLuint              index,       GLenum parameter) const
{
  GLint result;
  glGetActiveUniformsiv(id_, 1, &index, parameter, &result);
  return result;
}
std::vector<GLint> program::get_active_uniform_parameters             (std::vector<GLuint> indices,     GLenum parameter) const
{
  std::vector<GLint> result(indices.size());
  glGetActiveUniformsiv(id_, static_cast<GLsizei>(indices.size()), indices.data(), parameter, result.data());
  return result;
}
GLint              program::get_active_uniform_block_parameter        (GLuint              index,       GLenum parameter) const
{
  GLint result;
  glGetActiveUniformBlockiv(id_, index, parameter, &result);
  return result;
}
GLint              program::get_active_atomic_counter_buffer_parameter(GLuint              index,       GLenum parameter) const
{
  GLint result;
  glGetActiveAtomicCounterBufferiv(id_, index, parameter, &result);
  return result;
}
GLint              program::get_program_stage_parameter               (GLenum              shader_type, GLenum parameter) const
{
  GLint result;
  glGetProgramStageiv(id_, shader_type, parameter, &result);
  return result;
}
}