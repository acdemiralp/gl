//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_IMPLEMENTATION_DRAW_COMMANDS_IPP_
#define GL_IMPLEMENTATION_DRAW_COMMANDS_IPP_

namespace gl
{
// 7.12.2 Shader memory access.
void memory_barrier          (GLbitfield barrier_bits)
{
  glMemoryBarrier(barrier_bits);
}
void memory_barrier_by_region(GLbitfield barrier_bits)
{
  glMemoryBarrierByRegion(barrier_bits);
}

// 9.3.1 Feedback loops.
void texture_barrier()
{
  glTextureBarrier();
}

// 10.3.6 Primitive restart.
void set_primitive_restart_enabled(bool enabled, bool fixed_index)
{
  auto mode = !fixed_index ? GL_PRIMITIVE_RESTART : GL_PRIMITIVE_RESTART_FIXED_INDEX;
  enabled ? glEnable(mode) : glDisable(mode);
}
bool primitive_restart_enabled    (              bool fixed_index)
{
  return glIsEnabled(!fixed_index ? GL_PRIMITIVE_RESTART : GL_PRIMITIVE_RESTART_FIXED_INDEX) != 0;
}

// 10.4 Drawing commands.
void draw_arrays                                      (GLenum mode, GLint offset, GLsizei count)
{
  glDrawArrays(mode, offset, count);
}
void draw_arrays_instanced                            (GLenum mode, GLint offset, GLsizei count, GLsizei instance_count)
{
  glDrawArraysInstanced(mode, offset, count, instance_count);
}
void draw_arrays_instanced_base_intance               (GLenum mode, GLint offset, GLsizei count, GLsizei instance_count, GLuint base_instance)
{
  glDrawArraysInstancedBaseInstance(mode, offset, count, instance_count, base_instance);
}
void draw_arrays_indirect                             (GLenum mode, GLint offset)
{
  glDrawArraysIndirect(mode, reinterpret_cast<const void*>(std::size_t(offset)));
}

void multi_draw_arrays                                (GLenum mode, const std::vector<std::pair<GLint, GLsizei>>& offset_count_pairs)
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
void multi_draw_arrays_indirect                       (GLenum mode, const GLint offset, GLsizei draw_count, GLsizei stride)
{
  glMultiDrawArraysIndirect(mode, reinterpret_cast<const void*>(std::size_t(offset)), draw_count, stride);
}
void multi_draw_arrays_indirect_count                 (GLenum mode, const GLint offset, GLintptr draw_count, GLsizei max_draw_count, GLsizei stride)
{
  glMultiDrawArraysIndirectCount(mode, reinterpret_cast<const void*>(std::size_t(offset)), draw_count, max_draw_count, stride);
}

void draw_elements                                    (GLenum mode, GLsizei count, GLenum type, const void* indices)
{
  glDrawElements(mode, count, type, indices);
}
void draw_elements_instanced                          (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instance_count)
{
  glDrawElementsInstanced(mode, count, type, indices, instance_count);
}
void draw_elements_instanced_base_instance            (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instance_count, GLuint base_instance)
{
  glDrawElementsInstancedBaseInstance(mode, count, type, indices, instance_count, base_instance);
}
void draw_range_elements                              (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices)
{
  glDrawRangeElements(mode, start, end, count, type, indices);
}
void draw_elements_indirect                           (GLenum mode, GLenum type, const GLint offset)
{
  glDrawElementsIndirect(mode, type, reinterpret_cast<const void*>(std::size_t(offset)));
}

void multi_draw_elements                              (GLenum mode, GLenum type, const std::vector<std::pair<GLint, GLsizei>>& offset_count_pairs)
{
  std::vector<const void*> offsets;
  std::vector<GLsizei>     counts ;
  offsets.reserve(offset_count_pairs.size());
  counts .reserve(offset_count_pairs.size());
  for (const auto& pair : offset_count_pairs)
  {
    offsets.push_back(reinterpret_cast<const void*>(std::size_t(pair.first)));
    counts .push_back(pair.second);
  }
  glMultiDrawElements(mode, counts.data(), type, offsets.data(), static_cast<GLsizei>(offset_count_pairs.size()));
}
void multi_draw_elements_indirect                     (GLenum mode, GLenum type, const GLint offset, GLsizei draw_count, GLsizei stride)
{
  glMultiDrawElementsIndirect(mode, type, reinterpret_cast<const void*>(std::size_t(offset)), draw_count, stride);
}
void multi_draw_elements_indirect_count               (GLenum mode, GLenum type, const GLint offset, GLintptr draw_count, GLsizei max_draw_count, GLsizei stride)
{
  glMultiDrawElementsIndirectCount(mode, type, reinterpret_cast<const void*>(std::size_t(offset)), draw_count, max_draw_count, stride);
}

void draw_elements_base_vertex                        (GLenum mode, GLsizei count, GLenum type, void* indices, GLint base_vertex)
{
  glDrawElementsBaseVertex(mode, count, type, indices, base_vertex);
}
void draw_elements_instanced_base_vertex              (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instance_count, GLint base_vertex)
{
  glDrawElementsInstancedBaseVertex(mode, count, type, indices, instance_count, base_vertex);
}
void draw_elements_instanced_base_vertex_base_instance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instance_count, GLint base_vertex, GLuint base_instance)
{
  glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instance_count, base_vertex, base_instance);
}
void draw_range_elements_base_vertex                  (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, void* indices, GLint base_vertex)
{
  glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, base_vertex);
}

void multi_draw_elements_base_vertex                  (GLenum mode, GLenum type, const std::vector<std::tuple<GLint, GLsizei, GLint>>& offset_count_base_vertex_triplets)
{
  std::vector<void*>   offsets;
  std::vector<GLsizei> counts;
  std::vector<GLint>   base_vertices;
  offsets      .reserve(offset_count_base_vertex_triplets.size());
  counts       .reserve(offset_count_base_vertex_triplets.size());
  base_vertices.reserve(offset_count_base_vertex_triplets.size());
  for (const auto& triplet : offset_count_base_vertex_triplets)
  {
    offsets      .push_back(reinterpret_cast<void*>(std::size_t(std::get<0>(triplet))));
    counts       .push_back(std::get<1>(triplet));
    base_vertices.push_back(std::get<2>(triplet));
  }
  glMultiDrawElementsBaseVertex(mode, counts.data(), type, offsets.data(), static_cast<GLsizei>(offset_count_base_vertex_triplets.size()), base_vertices.data());
}
}

#endif