//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_DRAW_COMMANDS_HPP_
#define GL_DRAW_COMMANDS_HPP_

#include <tuple>
#include <utility>
#include <vector>

#include <gl/export.hpp>
#include <gl/opengl.hpp>

namespace gl
{
// 7.12.2 Shader memory access.
GL_EXPORT void memory_barrier          (GLbitfield barrier_bits = GL_ALL_BARRIER_BITS);
GL_EXPORT void memory_barrier_by_region(GLbitfield barrier_bits = GL_ALL_BARRIER_BITS);

// 9.3.1 Feedback loops.
GL_EXPORT void texture_barrier();

// 10.3.6 Primitive restart.
GL_EXPORT void set_primitive_restart_enabled(bool enabled, bool fixed_index = false);
GL_EXPORT bool primitive_restart_enabled    (              bool fixed_index = false);

// 10.4 Drawing commands.
struct GL_EXPORT draw_arrays_indirect_command
{
  GLuint count;
  GLuint instance_count;
  GLuint first;
  GLuint base_instance;
};
struct GL_EXPORT draw_elements_indirect_command
{
  GLuint count;
  GLuint instance_count;
  GLuint first;
  GLuint base_vertex;
  GLuint base_instance;
};

GL_EXPORT void draw_arrays                                      (GLenum mode, GLint offset, GLsizei count);
GL_EXPORT void draw_arrays_instanced                            (GLenum mode, GLint offset, GLsizei count, GLsizei instance_count = 1);
GL_EXPORT void draw_arrays_instanced_base_intance               (GLenum mode, GLint offset, GLsizei count, GLsizei instance_count = 1, GLuint base_instance = 0);
GL_EXPORT void draw_arrays_indirect                             (GLenum mode, GLint offset = 0);

GL_EXPORT void multi_draw_arrays                                (GLenum mode, const std::vector<std::pair<GLint, GLsizei>>& offset_count_pairs);
GL_EXPORT void multi_draw_arrays_indirect                       (GLenum mode, const GLint offset, GLsizei draw_count, GLsizei stride = sizeof(draw_arrays_indirect_command));

GL_EXPORT void draw_elements                                    (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr);
GL_EXPORT void draw_elements_instanced                          (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLsizei instance_count = 1);
GL_EXPORT void draw_elements_instanced_base_instance            (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLsizei instance_count = 1, GLuint base_instance = 0);
GL_EXPORT void draw_range_elements                              (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices = nullptr);
GL_EXPORT void draw_elements_indirect                           (GLenum mode, GLenum type, const GLint offset = 0);

GL_EXPORT void multi_draw_elements                              (GLenum mode, GLenum type, const std::vector<std::pair<GLint, GLsizei>>& offset_count_pairs);
GL_EXPORT void multi_draw_elements_indirect                     (GLenum mode, GLenum type, const GLint offset, GLsizei draw_count, GLsizei stride = sizeof(draw_elements_indirect_command));

GL_EXPORT void draw_elements_base_vertex                        (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLint base_vertex = 0);
GL_EXPORT void draw_elements_instanced_base_vertex              (GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLsizei instance_count = 1, GLint base_vertex = 0);
GL_EXPORT void draw_elements_instanced_base_vertex_base_instance(GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr, GLsizei instance_count = 1, GLint base_vertex = 0, GLuint base_instance = 0);
GL_EXPORT void draw_range_elements_base_vertex                  (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices = nullptr, GLint base_vertex = 0);

GL_EXPORT void multi_draw_elements_base_vertex                  (GLenum mode, GLenum type, const std::vector<std::tuple<GLint, GLsizei, GLint>>& offset_count_base_vertex_triplets);
}

#endif
