//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_BUFFER_HPP_
#define GL_BUFFER_HPP_

#include <cstddef>
#include <type_traits>
#include <vector>

#include <gl/opengl.hpp>

#ifdef GL_CUDA_INTEROP_SUPPORT
  #include <cuda_gl_interop.h>
  #include <cuda_runtime_api.h>
#endif

#include <export.hpp>

namespace gl
{
class GL_EXPORT buffer
{
public:
  // 6.0 Buffer objects.
  buffer();
  buffer(GLuint id);
  buffer(const buffer&  that);
  buffer(      buffer&& temp) = default;
  virtual ~buffer();
  buffer& operator=(const buffer&  that);
  buffer& operator=(      buffer&& temp) = default;

  // 6.1 Create and bind buffer objects.
  template<GLenum target>
  void        bind      () const;
  template<GLenum target>
  static void unbind    ();
  template<GLenum target, typename = typename std::enable_if<target == GL_ATOMIC_COUNTER_BUFFER || target == GL_SHADER_STORAGE_BUFFER || target == GL_UNIFORM_BUFFER || target == GL_TRANSFORM_FEEDBACK_BUFFER>::type>
  void        bind_range(GLuint index, GLintptr offset, GLsizeiptr size) const;
  template<GLenum target, typename = typename std::enable_if<target == GL_ATOMIC_COUNTER_BUFFER || target == GL_SHADER_STORAGE_BUFFER || target == GL_UNIFORM_BUFFER || target == GL_TRANSFORM_FEEDBACK_BUFFER>::type>
  void        bind_base (GLuint index) const;

  // 6.2 Create / modify buffer object data (bindless).
  void set_data_immutable(GLsizeiptr size, const void* data = nullptr, GLbitfield storage_flags = GL_DYNAMIC_STORAGE_BIT);
  void set_data          (GLsizeiptr size, const void* data = nullptr, GLenum     usage         = GL_DYNAMIC_DRAW       );
  void set_sub_data      (                        GLintptr offset, GLsizeiptr size,                                  const void* data);
  void clear_sub_data    (GLenum internal_format, GLintptr offset, GLsizeiptr size, GLenum format, GLenum data_type, const void* data = nullptr);
  void clear_data        (GLenum internal_format,                                   GLenum format, GLenum data_type, const void* data = nullptr);

  // 6.3 Map / unmap buffer data (bindless).
  void* map_range         (GLintptr offset, GLsizeiptr size, GLbitfield access_flags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
  void* map               (                                  GLenum     access       = GL_READ_WRITE                     );
  void  flush_mapped_range(GLintptr offset, GLsizeiptr size);
  void  unmap             ();

  // 6.5 Invalidate buffer data (bindless).
  void invalidate_sub_data(GLintptr offset, GLsizeiptr size);
  void invalidate         ();

  // 6.6 Copy between buffers (bindless).
  void copy_sub_data(const buffer& source, GLintptr source_offset, GLintptr offset, GLsizeiptr size);

  // 6.7 Buffer object queries (bindless).
  bool                 is_valid     () const;
  std::vector<GLubyte> sub_data     (GLintptr offset, GLsizeiptr size) const;
  GLsizeiptr           size         () const;
  GLenum               usage        () const;
  GLenum               access       () const;
  GLbitfield           access_flags () const;
  bool                 is_mapped    () const;
  bool                 is_immutable () const;
  GLbitfield           storage_flags() const;
  GLintptr             map_offset   () const;
  GLsizeiptr           map_size     () const;
  void*                map_pointer  () const;

  static const GLenum native_type = GL_BUFFER;

  GLuint id() const;

#ifdef GL_CUDA_INTEROP_SUPPORT
  void cuda_register  (cudaGraphicsMapFlags flags = cudaGraphicsMapFlagsNone);
  void cuda_unregister();

  template<typename type>
  type* cuda_map  ();
  void  cuda_unmap();
#endif

protected:
  GLint   get_parameter   (GLenum parameter) const;
  GLint64 get_parameter_64(GLenum parameter) const;

  GLuint id_      = 0;
  bool   managed_ = true;

#ifdef GL_CUDA_INTEROP_SUPPORT
  cudaGraphicsResource* resource_ = nullptr;
#endif
};

// 6.1 Create and bind buffer objects.
template <GLenum target>
void buffer::bind      () const
{
  glBindBuffer(target, id_);
}
template <GLenum target>
void buffer::unbind    ()
{
  glBindBuffer(target, 0);
}
template <GLenum target, typename>
void buffer::bind_range(GLuint index, GLintptr offset, GLsizeiptr size) const
{
  glBindBufferRange(target, index, id_, offset, size);
}
template <GLenum target, typename>
void buffer::bind_base (GLuint index) const
{
  glBindBufferBase(target, index, id_);
}

#ifdef GL_CUDA_INTEROP_SUPPORT
template <typename type>
type* buffer::cuda_map()
{
  type*  buffer_ptr ;
  size_t buffer_size;
  cudaGraphicsMapResources(1, &resource_, nullptr);
  cudaGraphicsResourceGetMappedPointer(static_cast<void**>(&buffer_ptr), &buffer_size, resource_);
  return buffer_ptr;
}
#endif
}

#endif