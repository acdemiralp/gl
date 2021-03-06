//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_DRAW_COMMANDS_HPP
#define GL_DRAW_COMMANDS_HPP

#include <cstddef>
#include <tuple>
#include <utility>
#include <vector>

#include <gl/opengl.hpp>

namespace gl
{
// 7.12.2 Shader memory access.
inline void memory_barrier          (const GLbitfield barrier_bits = GL_ALL_BARRIER_BITS)
{
  glMemoryBarrier(barrier_bits);
}
inline void memory_barrier_by_region(const GLbitfield barrier_bits = GL_ALL_BARRIER_BITS)
{
  glMemoryBarrierByRegion(barrier_bits);
}

// 9.3.1 Feedback loops.
inline void texture_barrier()
{
  glTextureBarrier();
}

// 10.3.6 Primitive restart.
inline void set_primitive_restart_enabled(const bool enabled, const bool fixed_index = false)
{
  const auto mode = !fixed_index ? GL_PRIMITIVE_RESTART : GL_PRIMITIVE_RESTART_FIXED_INDEX;
  enabled ? glEnable(mode) : glDisable(mode);
}
inline bool primitive_restart_enabled    (                    const bool fixed_index = false)
{
  return glIsEnabled(!fixed_index ? GL_PRIMITIVE_RESTART : GL_PRIMITIVE_RESTART_FIXED_INDEX) != 0;
}

// 10.4 Drawing commands.
struct draw_arrays_indirect_command
{
  GLuint count;
  GLuint instance_count;
  GLuint first;
  GLuint base_instance;
};
struct draw_elements_indirect_command
{
  GLuint count;
  GLuint instance_count;
  GLuint first;
  GLuint base_vertex;
  GLuint base_instance;
};

inline void draw_arrays                                      (const GLenum mode, const GLint offset, const GLsizei count)
{
  glDrawArrays(mode, offset, count);
}
inline void draw_arrays_instanced                            (const GLenum mode, const GLint offset, const GLsizei count, const GLsizei instance_count = 1)
{
  glDrawArraysInstanced(mode, offset, count, instance_count);
}
inline void draw_arrays_instanced_base_instance              (const GLenum mode, const GLint offset, const GLsizei count, const GLsizei instance_count = 1, const GLuint base_instance = 0)
{
  glDrawArraysInstancedBaseInstance(mode, offset, count, instance_count, base_instance);
}
inline void draw_arrays_indirect                             (const GLenum mode, const GLint offset = 0)
{
  glDrawArraysIndirect(mode, reinterpret_cast<const void*>(static_cast<std::size_t>(offset)));
}

inline void multi_draw_arrays                                (const GLenum mode, const std::vector<std::pair<GLint, GLsizei>>& offset_count_pairs)
{
  std::vector<GLint>   offsets;
  std::vector<GLsizei> counts ;
  offsets.reserve(offset_count_pairs.size());
  counts .reserve(offset_count_pairs.size());
  for (const auto& pair : offset_count_pairs)
  {
    offsets.push_back(pair.first );
    counts .push_back(pair.second);
  }
  glMultiDrawArrays(mode, offsets.data(), counts.data(), static_cast<GLsizei>(offset_count_pairs.size()));
}
inline void multi_draw_arrays_indirect                       (const GLenum mode, const GLint offset, const GLsizei draw_count, const GLsizei stride = sizeof(draw_arrays_indirect_command))
{
  glMultiDrawArraysIndirect(mode, reinterpret_cast<const void*>(static_cast<std::size_t>(offset)), draw_count, stride);
}
inline void multi_draw_arrays_indirect_count                 (const GLenum mode, const GLint offset, const GLintptr draw_count, const GLsizei max_draw_count, const GLsizei stride = sizeof(draw_arrays_indirect_command))
{
  glMultiDrawArraysIndirectCount(mode, reinterpret_cast<const void*>(static_cast<std::size_t>(offset)), draw_count, max_draw_count, stride);
}

inline void draw_elements                                    (const GLenum mode, const GLsizei count, const GLenum type, const void* indices = nullptr)
{
  glDrawElements(mode, count, type, indices);
}
inline void draw_elements_instanced                          (const GLenum mode, const GLsizei count, const GLenum type, const void* indices = nullptr, const GLsizei instance_count = 1)
{
  glDrawElementsInstanced(mode, count, type, indices, instance_count);
}
inline void draw_elements_instanced_base_instance            (const GLenum mode, const GLsizei count, const GLenum type, const void* indices = nullptr, const GLsizei instance_count = 1, const GLuint base_instance = 0)
{
  glDrawElementsInstancedBaseInstance(mode, count, type, indices, instance_count, base_instance);
}
inline void draw_range_elements                              (const GLenum mode, const GLuint start, const GLuint end, const GLsizei count, const GLenum type, const void* indices = nullptr)
{
  glDrawRangeElements(mode, start, end, count, type, indices);
}
inline void draw_elements_indirect                           (const GLenum mode, const GLenum type, const GLint offset = 0)
{
  glDrawElementsIndirect(mode, type, reinterpret_cast<const void*>(static_cast<std::size_t>(offset)));
}

inline void multi_draw_elements                              (const GLenum mode, const GLenum type, const std::vector<std::pair<GLint, GLsizei>>& offset_count_pairs)
{
  std::vector<const void*> offsets;
  std::vector<GLsizei>     counts ;
  offsets.reserve(offset_count_pairs.size());
  counts .reserve(offset_count_pairs.size());
  for (const auto& pair : offset_count_pairs)
  {
    offsets.push_back(reinterpret_cast<const void*>(static_cast<std::size_t>(pair.first)));
    counts .push_back(pair.second);
  }
  glMultiDrawElements(mode, counts.data(), type, offsets.data(), static_cast<GLsizei>(offset_count_pairs.size()));
}
inline void multi_draw_elements_indirect                     (const GLenum mode, const GLenum type, const GLint offset, const GLsizei draw_count, const GLsizei stride = sizeof(draw_elements_indirect_command))
{
  glMultiDrawElementsIndirect(mode, type, reinterpret_cast<const void*>(static_cast<std::size_t>(offset)), draw_count, stride);
}
inline void multi_draw_elements_indirect_count               (const GLenum mode, const GLenum type, const GLint offset, const GLintptr draw_count, const GLsizei max_draw_count, const GLsizei stride = sizeof(draw_arrays_indirect_command))
{
  glMultiDrawElementsIndirectCount(mode, type, reinterpret_cast<const void*>(static_cast<std::size_t>(offset)), draw_count, max_draw_count, stride);
}

inline void draw_elements_base_vertex                        (const GLenum mode, const GLsizei count, const GLenum type, void* indices = nullptr, const GLint base_vertex = 0)
{
  glDrawElementsBaseVertex(mode, count, type, indices, base_vertex);
}
inline void draw_elements_instanced_base_vertex              (const GLenum mode, const GLsizei count, const GLenum type, const void* indices = nullptr, const GLsizei instance_count = 1, const GLint base_vertex = 0)
{
  glDrawElementsInstancedBaseVertex(mode, count, type, indices, instance_count, base_vertex);
}
inline void draw_elements_instanced_base_vertex_base_instance(const GLenum mode, const GLsizei count, const GLenum type, const void* indices = nullptr, const GLsizei instance_count = 1, const GLint base_vertex = 0, const GLuint base_instance = 0)
{
  glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instance_count, base_vertex, base_instance);
}
inline void draw_range_elements_base_vertex                  (const GLenum mode, const GLuint start, const GLuint end, const GLsizei count, const GLenum type, void* indices = nullptr, const GLint base_vertex = 0)
{
  glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, base_vertex);
}

inline void multi_draw_elements_base_vertex                  (const GLenum mode, const GLenum type, const std::vector<std::tuple<GLint, GLsizei, GLint>>& offset_count_base_vertex_triplets)
{
  std::vector<void*>   offsets;
  std::vector<GLsizei> counts;
  std::vector<GLint>   base_vertices;
  offsets      .reserve(offset_count_base_vertex_triplets.size());
  counts       .reserve(offset_count_base_vertex_triplets.size());
  base_vertices.reserve(offset_count_base_vertex_triplets.size());
  for (const auto& triplet : offset_count_base_vertex_triplets)
  {
    offsets      .push_back(reinterpret_cast<void*>(static_cast<std::size_t>(std::get<0>(triplet))));
    counts       .push_back(std::get<1>(triplet));
    base_vertices.push_back(std::get<2>(triplet));
  }
  glMultiDrawElementsBaseVertex(mode, counts.data(), type, offsets.data(), static_cast<GLsizei>(offset_count_base_vertex_triplets.size()), base_vertices.data());
}
}

#endif
