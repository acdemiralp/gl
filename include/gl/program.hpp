//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_PROGRAM_HPP_
#define GL_PROGRAM_HPP_

#include <algorithm>
#include <array>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <gl/export.hpp>
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
class GL_EXPORT program
{
public:
  // 7.3 Program objects.
  program();
  program(GLuint id);
  program(const program&  that) = delete;
  program(      program&& temp) noexcept;
  virtual ~program();
  program& operator=(const program&  that) = delete;
  program& operator=(      program&& temp) noexcept;
  
  template<GLenum shader_type>
  static program create_shader_program(const std::string&              shader_string );
  template<GLenum shader_type>
  static program create_shader_program(const std::vector<std::string>& shader_strings);

  void        attach_shader(const shader& shader) const;
  void        detach_shader(const shader& shader) const;
  bool        link         () const;
  void        use          () const;
  static void unuse        ();
  bool        is_valid     () const;

  void set_separable         (bool separable);
  void set_binary_retrievable(bool binary_retrievable);

  // 7.3.1 Program interfaces.
  GLsizei     interface_active_resources               (GLenum interface) const;
  GLsizei     interface_max_name_length                (GLenum interface) const;
  GLsizei     interface_max_active_variable_count      (GLenum interface) const;
  GLsizei     interface_max_compatible_subroutine_count(GLenum interface) const;
  GLuint      resource_index                           (GLenum interface, const std::string& name) const;
  std::string resource_name                            (GLenum interface, GLuint index) const;
  GLint       resource_location                        (GLenum interface, const std::string& name) const;
  GLint       resource_location_index                  (GLenum interface, const std::string& name) const;

  GLint              resource_parameter (GLenum interface, GLuint index, GLenum parameter) const;
  std::vector<GLint> resource_parameters(GLenum interface, GLuint index, const std::vector<GLenum>& parameters) const;

  // 7.5 Program binaries.
  template<typename type = GLbyte>
  std::vector<type> program_binary    (GLenum format) const;
  template<typename type = GLbyte>
  void              set_program_binary(GLenum format, const std::vector<type>& binary);

  // 7.6 Uniform variables.
  GLint uniform_location(const std::string& name) const;

  std::string active_uniform_name(GLuint index) const;
  
  std::vector<GLuint> uniform_indices(GLsizei count, const std::vector<std::string>& names) const;
  
  std::tuple<std::string, GLenum, GLsizei> active_uniform(GLuint index) const;
  
  GLsizei active_uniform_name_length                (GLuint index) const;
  GLenum  active_uniform_type                       (GLuint index) const;
  GLuint  active_uniform_offset                     (GLuint index) const;
  GLsizei active_uniform_size                       (GLuint index) const;
  GLuint  active_uniform_block_index                (GLuint index) const;
  GLsizei active_uniform_array_stride               (GLuint index) const;
  GLsizei active_uniform_matrix_stride              (GLuint index) const;
  bool    active_uniform_is_row_major               (GLuint index) const;
  GLuint  active_uniform_atomic_counter_buffer_index(GLuint index) const;

  std::vector<GLsizei> active_uniforms_name_lengths                 (const std::vector<GLuint>& indices) const;
  std::vector<GLenum>  active_uniforms_types                        (const std::vector<GLuint>& indices) const;
  std::vector<GLuint>  active_uniforms_offsets                      (const std::vector<GLuint>& indices) const;
  std::vector<GLsizei> active_uniforms_sizes                        (const std::vector<GLuint>& indices) const;
  std::vector<GLuint>  active_uniforms_block_indices                (const std::vector<GLuint>& indices) const;
  std::vector<GLsizei> active_uniforms_array_strides                (const std::vector<GLuint>& indices) const;
  std::vector<GLsizei> active_uniforms_matrix_strides               (const std::vector<GLuint>& indices) const;
  std::vector<bool>    active_uniforms_are_row_majors               (const std::vector<GLuint>& indices) const;
  std::vector<GLuint>  active_uniforms_atomic_counter_buffer_indices(const std::vector<GLuint>& indices) const;

  GLuint uniform_block_index(const std::string& name) const;

  std::string active_uniform_block_name(GLuint index) const;

  GLuint  active_uniform_block_binding                                        (GLuint index) const;
  GLuint  active_uniform_block_data_size                                      (GLuint index) const;
  GLuint  active_uniform_block_name_length                                    (GLuint index) const;
  GLsizei active_uniform_block_uniform_count                                  (GLuint index) const;
  bool    active_uniform_block_is_referenced_by_vertex_shader                 (GLuint index) const;
  bool    active_uniform_block_is_referenced_by_fragment_shader               (GLuint index) const;
  bool    active_uniform_block_is_referenced_by_compute_shader                (GLuint index) const;
  bool    active_uniform_block_is_referenced_by_geometry_shader               (GLuint index) const;
  bool    active_uniform_block_is_referenced_by_tessellation_control_shader   (GLuint index) const;
  bool    active_uniform_block_is_referenced_by_tessellation_evaluation_shader(GLuint index) const;

  std::vector<GLuint> active_uniform_block_uniform_indices(GLuint index) const;

  GLuint  active_atomic_counter_buffer_data_size                                       (GLuint index) const;
  GLsizei active_atomic_counter_buffer_counters                                        (GLuint index) const;
  bool    active_atomic_counter_buffer_is_referenced_by_vertex_shader                  (GLuint index) const;
  bool    active_atomic_counter_buffer_is_referenced_by_fragment_shader                (GLuint index) const;
  bool    active_atomic_counter_buffer_is_referenced_by_compute_shader                 (GLuint index) const;
  bool    active_atomic_counter_buffer_is_referenced_by_geometry_shader                (GLuint index) const;
  bool    active_atomic_counter_buffer_is_referenced_by_tessellation_control_shader    (GLuint index) const;
  bool    active_atomic_counter_buffer_is_referenced_by_tessellation_evaluation_shader (GLuint index) const;

  std::vector<GLuint> active_atomic_counter_buffer_counter_indices(GLuint index) const;

  void set_uniform_1i(GLint location, GLint value);
  void set_uniform_1i(GLint location, const std::vector<GLint>&   value);
  void set_uniform_2i(GLint location, const std::array<GLint, 2>& value);
  void set_uniform_2i(GLint location, const std::vector<GLint>&   value);
  void set_uniform_3i(GLint location, const std::array<GLint, 3>& value);
  void set_uniform_3i(GLint location, const std::vector<GLint>&   value);
  void set_uniform_4i(GLint location, const std::array<GLint, 4>& value);
  void set_uniform_4i(GLint location, const std::vector<GLint>&   value);

  void set_uniform_1ui(GLint location, GLuint value);
  void set_uniform_1ui(GLint location, const std::vector<GLuint>&   value);
  void set_uniform_2ui(GLint location, const std::array<GLuint, 2>& value);
  void set_uniform_2ui(GLint location, const std::vector<GLuint>&   value);
  void set_uniform_3ui(GLint location, const std::array<GLuint, 3>& value);
  void set_uniform_3ui(GLint location, const std::vector<GLuint>&   value);
  void set_uniform_4ui(GLint location, const std::array<GLuint, 4>& value);
  void set_uniform_4ui(GLint location, const std::vector<GLuint>&   value);

  void set_uniform_1f(GLint location, GLfloat value);
  void set_uniform_1f(GLint location, const std::vector<GLfloat>&   value);
  void set_uniform_2f(GLint location, const std::array<GLfloat, 2>& value);
  void set_uniform_2f(GLint location, const std::vector<GLfloat>&   value);
  void set_uniform_3f(GLint location, const std::array<GLfloat, 3>& value);
  void set_uniform_3f(GLint location, const std::vector<GLfloat>&   value);
  void set_uniform_4f(GLint location, const std::array<GLfloat, 4>& value);
  void set_uniform_4f(GLint location, const std::vector<GLfloat>&   value);

  void set_uniform_1d(GLint location, GLdouble value);
  void set_uniform_1d(GLint location, const std::vector<GLdouble>&   value);
  void set_uniform_2d(GLint location, const std::array<GLdouble, 2>& value);
  void set_uniform_2d(GLint location, const std::vector<GLdouble>&   value);
  void set_uniform_3d(GLint location, const std::array<GLdouble, 3>& value);
  void set_uniform_3d(GLint location, const std::vector<GLdouble>&   value);
  void set_uniform_4d(GLint location, const std::array<GLdouble, 4>& value);
  void set_uniform_4d(GLint location, const std::vector<GLdouble>&   value);

  void set_uniform_matrix_22f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);
  void set_uniform_matrix_33f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);
  void set_uniform_matrix_44f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);
  void set_uniform_matrix_23f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);
  void set_uniform_matrix_32f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);
  void set_uniform_matrix_24f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);
  void set_uniform_matrix_42f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);
  void set_uniform_matrix_34f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);
  void set_uniform_matrix_43f(GLint location, const std::vector<GLfloat>& value, bool transpose = false);

  void set_uniform_matrix_22d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);
  void set_uniform_matrix_33d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);
  void set_uniform_matrix_44d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);
  void set_uniform_matrix_23d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);
  void set_uniform_matrix_32d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);
  void set_uniform_matrix_24d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);
  void set_uniform_matrix_42d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);
  void set_uniform_matrix_34d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);
  void set_uniform_matrix_43d(GLint location, const std::vector<GLdouble>& value, bool transpose = false);

#ifdef GL_ARB_bindless_texture
  void set_uniform_handle(GLint location, const texture_handle&              value);
  void set_uniform_handle(GLint location, const std::vector<texture_handle>& value);
  void set_uniform_handle(GLint location, const image_handle&                value);
  void set_uniform_handle(GLint location, const std::vector<image_handle>&   value);
#endif

  void set_uniform_block_binding(GLuint index, GLuint binding);

  // 7.8 Shader buffer variables.
  void set_shader_storage_block_binding(GLuint index, GLuint binding);

  // 7.9 Subroutine uniform variables.
  GLint               subroutine_uniform_location                          (GLenum shader_type, const std::string& name) const;
  GLuint              subroutine_index                                     (GLenum shader_type, const std::string& name) const;
  std::string         active_subroutine_name                               (GLenum shader_type, GLuint subroutine_index) const;
  std::string         active_subroutine_uniform_name                       (GLenum shader_type, GLuint subroutine_index) const;
  std::vector<GLuint> active_subroutine_uniform_compatible_subroutines     (GLenum shader_type, GLuint subroutine_index) const;
  GLsizei             active_subroutine_uniform_compatible_subroutine_count(GLenum shader_type, GLuint subroutine_index) const;
  void                set_active_subroutine_uniforms                       (GLenum shader_type, const std::vector<GLuint>& subroutine_indices);

  // 7.13 Program queries.
  bool    delete_status                           () const;
  bool    link_status                             () const;
  bool    validation_status                       () const;
  GLsizei info_log_length                         () const;
  GLsizei attached_shader_count                   () const;
  GLsizei active_attribute_count                  () const;
  GLsizei active_attribute_max_length             () const;
  GLsizei active_uniform_count                    () const;
  GLsizei active_uniform_max_length               () const;
  GLsizei active_uniform_block_count              () const;
  GLsizei active_uniform_block_max_name_length    () const;
  GLenum  transform_feedback_buffer_mode          () const;
  GLsizei transform_feedback_varying_count        () const;
  GLsizei transform_feedback_varying_max_length   () const;
  GLsizei geometry_vertices_out                   () const;
  GLenum  geometry_input_type                     () const;
  GLenum  geometry_output_type                    () const;
  GLsizei geometry_shader_invocations             () const;
  GLsizei tessellation_control_output_vertex_count() const;
  GLenum  tessellation_generation_mode            () const;
  GLenum  tessellation_generation_spacing         () const;
  GLenum  tessellation_generation_vertex_order    () const;
  bool    tessellation_generation_point_mode      () const;
  bool    is_separable                            () const;
  bool    is_binary_retrievable                   () const;
  GLsizei active_atomic_counter_buffer_count      () const;
  GLsizei binary_length                           () const;

  std::array<GLint, 3> compute_work_group_size() const;

  std::vector<shader> attached_shaders() const;

  std::string info_log() const;

  GLint                    uniform_1i (GLint location) const;
  std::array<GLint, 2>     uniform_2i (GLint location) const;
  std::array<GLint, 3>     uniform_3i (GLint location) const;
  std::array<GLint, 4>     uniform_4i (GLint location) const;
                           
  GLuint                   uniform_1ui(GLint location) const;
  std::array<GLuint, 2>    uniform_2ui(GLint location) const;
  std::array<GLuint, 3>    uniform_3ui(GLint location) const;
  std::array<GLuint, 4>    uniform_4ui(GLint location) const;
                           
  GLfloat                  uniform_1f (GLint location) const;
  std::array<GLfloat, 2>   uniform_2f (GLint location) const;
  std::array<GLfloat, 3>   uniform_3f (GLint location) const;
  std::array<GLfloat, 4>   uniform_4f (GLint location) const;
                           
  GLdouble                 uniform_1d (GLint location) const;
  std::array<GLdouble, 2>  uniform_2d (GLint location) const;
  std::array<GLdouble, 3>  uniform_3d (GLint location) const;
  std::array<GLdouble, 4>  uniform_4d (GLint location) const;
                           
  std::array<GLfloat, 4>   uniform_matrix_22f(GLint location) const;
  std::array<GLfloat, 9>   uniform_matrix_33f(GLint location) const;
  std::array<GLfloat, 16>  uniform_matrix_44f(GLint location) const;
  std::array<GLfloat, 6>   uniform_matrix_23f(GLint location) const;
  std::array<GLfloat, 6>   uniform_matrix_32f(GLint location) const;
  std::array<GLfloat, 8>   uniform_matrix_24f(GLint location) const;
  std::array<GLfloat, 8>   uniform_matrix_42f(GLint location) const;
  std::array<GLfloat, 12>  uniform_matrix_34f(GLint location) const;
  std::array<GLfloat, 12>  uniform_matrix_43f(GLint location) const;

  std::array<GLdouble, 4>  uniform_matrix_22d(GLint location) const;
  std::array<GLdouble, 9>  uniform_matrix_33d(GLint location) const;
  std::array<GLdouble, 16> uniform_matrix_44d(GLint location) const;
  std::array<GLdouble, 6>  uniform_matrix_23d(GLint location) const;
  std::array<GLdouble, 6>  uniform_matrix_32d(GLint location) const;
  std::array<GLdouble, 8>  uniform_matrix_24d(GLint location) const;
  std::array<GLdouble, 8>  uniform_matrix_42d(GLint location) const;
  std::array<GLdouble, 12> uniform_matrix_34d(GLint location) const;
  std::array<GLdouble, 12> uniform_matrix_43d(GLint location) const;

  static GLuint subroutine_uniform(GLenum shader_type, GLint location);

  GLsizei active_subroutine_count                 (GLenum shader_type) const;
  GLsizei active_subroutine_max_length            (GLenum shader_type) const;
  GLsizei active_subroutine_uniform_count         (GLenum shader_type) const;
  GLsizei active_subroutine_uniform_max_length    (GLenum shader_type) const;
  GLsizei active_subroutine_uniform_location_count(GLenum shader_type) const;

  // 11.1.1 Vertex attributes.
  void   set_attribute_location(const std::string& attribute_name, GLuint location);
  GLuint attribute_location    (const std::string& attribute_name) const;

  std::tuple<std::string, GLenum, GLsizei> active_attribute(GLuint location) const;

  // 11.1.2 Transform feedback variables.
  void set_transform_feedback_varyings(const std::vector<std::string>& varyings, GLenum buffer_mode = GL_INTERLEAVED_ATTRIBS);

  std::tuple<std::string, GLenum, GLsizei> transform_feedback_varying(GLuint index) const;

  // 11.1.3 Shader execution.
  bool validate() const;

  // 15.2 Fragment shaders.
  void   set_frag_data_location        (GLuint color_number,               const std::string& name);
  void   set_frag_data_location_indexed(GLuint color_number, GLuint index, const std::string& name);
  GLuint frag_data_location            (const std::string& name) const;
  GLuint frag_data_index               (const std::string& name) const;

  // X Extended Functionality - Type-inferring uniform setters.
  template<typename type>
  void set_uniform       (GLint location, const type& value);
  template<typename type>
  void set_uniform_vector(GLint location, const std::vector<type>& values);

  static const GLenum native_type = GL_PROGRAM;

  GLuint id() const;

protected:
  GLint              get_parameter                             (                             GLenum parameter) const;
  GLint              get_interface_parameter                   (GLenum interface,            GLenum parameter) const;
  GLint              get_active_uniform_parameter              (GLuint index,                GLenum parameter) const;
  std::vector<GLint> get_active_uniform_parameters             (std::vector<GLuint> indices, GLenum parameter) const;
  GLint              get_active_uniform_block_parameter        (GLuint index,                GLenum parameter) const;
  GLint              get_active_atomic_counter_buffer_parameter(GLuint index,                GLenum parameter) const;
  GLint              get_program_stage_parameter               (GLenum shader_type,          GLenum parameter) const;

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};

// 7.3 Program objects.
template <GLenum shader_type>
program program::create_shader_program(const std::string&              shader_string )
{
  auto shader_string_c = shader_string.c_str();
  return std::move(program(glCreateShaderProgramv(shader_type, 1, &shader_string_c)));
}
template <GLenum shader_type>
program program::create_shader_program(const std::vector<std::string>& shader_strings)
{
  std::vector<char*> shader_strings_c(shader_strings.size());
  std::transform(shader_strings.begin(), shader_strings.end(), shader_strings_c.begin(), 
  [&](const std::string& shader_string)
  {
    return shader_string.c_str();
  });
  return program(glCreateShaderProgramv(shader_type, static_cast<GLsizei>(shader_strings.size()), shader_strings_c.data()));
}

// 7.5 Program binaries.
template <typename type>
std::vector<type> program::program_binary    (GLenum format) const
{
  std::vector<type> result(binary_length());
  glGetProgramBinary(id_, static_cast<GLsizei>(result.size()), nullptr, &format, static_cast<void*>(result.data()));
  return result;
}
template <typename type>
void              program::set_program_binary(GLenum format, const std::vector<type>& binary)
{
  glProgramBinary(id_, format, static_cast<const void*>(binary.data()), static_cast<GLsizei>(binary.size()));
}

// X Extended Functionality - Type-inferring uniform setters.
template <typename type>
void program::set_uniform              (GLint location, const type&                  value )
{
  static_assert(sizeof(type) == 0, "Type not allowed.");
}
template <typename type>                                                             
void program::set_uniform_vector       (GLint location, const std::vector<type>&     values)
{
  static_assert(sizeof(type) == 0, "Type not allowed.");
}
template <>                                                                          
inline void program::set_uniform       (GLint location, const bool&                  value )
{
  set_uniform_1i(location, value);
}
template <>                                                                          
inline void program::set_uniform       (GLint location, const GLint&                 value )
{
  set_uniform_1i(location, value);
}
template <>                                                                          
inline void program::set_uniform_vector(GLint location, const std::vector<GLint>&    values)
{
  set_uniform_1i(location, values);
}
template <>                                                                          
inline void program::set_uniform       (GLint location, const GLuint&                value )
{
  set_uniform_1ui(location, value);
}
template <>                                                                          
inline void program::set_uniform_vector(GLint location, const std::vector<GLuint>&   values)
{
  set_uniform_1ui(location, values);
}
template <>                                                                          
inline void program::set_uniform       (GLint location, const GLfloat&               value )
{
  set_uniform_1f(location, value);
}
template <>                                                                          
inline void program::set_uniform_vector(GLint location, const std::vector<GLfloat>&  values)
{
  set_uniform_1f(location, values);
}
template <>                                                                          
inline void program::set_uniform       (GLint location, const GLdouble&              value )
{
  set_uniform_1d(location, value);
}
template <>
inline void program::set_uniform_vector(GLint location, const std::vector<GLdouble>& values)
{
  set_uniform_1d(location, values);
}
}

#endif
