//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_BUFFER_HPP
#define GL_BUFFER_HPP

#include <vector>

#include <gl/opengl.hpp>

#ifdef GL_CUDA_INTEROP_SUPPORT
  #include <cuda_gl_interop.h>
  #include <cuda_runtime_api.h>
#endif

namespace gl
{
class buffer
{
public:
  // 6.0 Buffer objects.
  buffer           ()
  {
    glCreateBuffers(1, &id_);
  }
  explicit buffer  (const GLuint id) : id_(id), managed_(false)
  {

  }
  buffer           (const buffer&  that): buffer()
  {
    that.is_immutable()
      ? set_data_immutable(that.size(), nullptr, that.storage_flags())
      : set_data          (that.size(), nullptr, that.usage        ());
    copy_sub_data(that, 0, 0, size());
  }
  buffer           (      buffer&& temp) noexcept : id_(temp.id_), managed_(temp.managed_)
  {
#ifdef GL_CUDA_INTEROP_SUPPORT
    resource_ = std::move(temp.resource_);
#endif
  
    temp.id_       = invalid_id;
    temp.managed_  = false;
#ifdef GL_CUDA_INTEROP_SUPPORT
    temp.resource_ = nullptr;
#endif
  }
  virtual ~buffer  ()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteBuffers(1, &id_);
  }
  buffer& operator=(const buffer&  that)
  {
    that.is_immutable()
      ? set_data_immutable(that.size(), nullptr, that.storage_flags())
      : set_data          (that.size(), nullptr, that.usage        ());
    copy_sub_data(that, 0, 0, size());
    return *this;
  }
  buffer& operator=(      buffer&& temp) noexcept
  { 
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteBuffers(1, &id_);
  
      id_       = temp.id_;
      managed_  = temp.managed_;
#ifdef GL_CUDA_INTEROP_SUPPORT
      resource_ = std::move(temp.resource_);
#endif
  
      temp.id_       = invalid_id;
      temp.managed_  = false;
#ifdef GL_CUDA_INTEROP_SUPPORT
      temp.resource_ = nullptr;
#endif
    }
    return *this;
  }

  // 6.1 Create and bind buffer objects.
  void        bind        (const GLenum target) const
  {
    glBindBuffer(target, id_);
  }
  static void unbind      (const GLenum target)
  {
    glBindBuffer(target, 0);
  }
  void        bind_range  (const GLenum target, const GLuint index, const GLintptr offset, const GLsizeiptr size) const
  {
    glBindBufferRange(target, index, id_, offset, size);
  }
  static void unbind_range(const GLenum target, const GLuint index, const GLintptr offset, const GLsizeiptr size)
  {
    glBindBufferRange(target, index, 0, offset, size);
  }
  void        bind_base   (const GLenum target, const GLuint index) const
  {
    glBindBufferBase(target, index, id_);
  }
  static void unbind_base (const GLenum target, const GLuint index)
  {
    glBindBufferBase(target, index, 0);
  }

  // 6.2 Create / modify buffer object data (bindless).
  void set_data_immutable(const GLsizeiptr size, const void* data = nullptr, const GLbitfield storage_flags = GL_DYNAMIC_STORAGE_BIT) const
  {
    glNamedBufferStorage(id_, size, data, storage_flags);
  }
  void set_data          (const GLsizeiptr size, const void* data = nullptr, const GLenum     usage         = GL_DYNAMIC_DRAW       ) const
  {
    glNamedBufferData(id_, size, data, usage);
  }
  void set_sub_data      (                              const GLintptr offset, const GLsizeiptr size,                                              const void* data) const
  {
    glNamedBufferSubData(id_, offset, size, data);
  }
  void clear_sub_data    (const GLenum internal_format, const GLintptr offset, const GLsizeiptr size, const GLenum format, const GLenum data_type, const void* data = nullptr) const
  {
    glClearNamedBufferSubData(id_, internal_format, offset, size, format, data_type, data);
  }
  void clear_data        (const GLenum internal_format,                                               const GLenum format, const GLenum data_type, const void* data = nullptr) const
  {
    glClearNamedBufferData(id_, internal_format, format, data_type, data);
  }

  // 6.3 Map / unmap buffer data (bindless).
  [[nodiscard]]
  void* map_range         (const GLintptr offset, const GLsizeiptr size, const GLbitfield access_flags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT) const
  {
    return glMapNamedBufferRange(id_, offset, size, access_flags);
  }
  [[nodiscard]]
  void* map               (const GLenum access = GL_READ_WRITE) const
  {
    return glMapNamedBuffer(id_, access);
  }
  void  flush_mapped_range(const GLintptr offset, const GLsizeiptr size) const
  {
    glFlushMappedNamedBufferRange(id_, offset, size);
  }
  void  unmap             () const
  {
    glUnmapNamedBuffer(id_);
  }

  // 6.5 Invalidate buffer data (bindless).
  void invalidate_sub_data(const GLintptr offset, const GLsizeiptr size) const
  {
    glInvalidateBufferSubData(id_, offset, size);
  }
  void invalidate         () const
  {
    glInvalidateBufferData(id_);
  }

  // 6.6 Copy between buffers (bindless).
  void copy_sub_data(const buffer& source, const GLintptr source_offset, const GLintptr offset, const GLsizeiptr size) const
  {
    glCopyNamedBufferSubData(source.id_, id_, source_offset, offset, size);
  }

  // 6.7 Buffer object queries (bindless).
  [[nodiscard]]
  bool                 is_valid     () const
  {
    return glIsBuffer(id_) != 0;
  }
  [[nodiscard]]
  std::vector<GLubyte> sub_data     (const GLintptr offset, const GLsizeiptr size) const
  {
    std::vector<GLubyte> data(size);
    glGetNamedBufferSubData(id_, offset, size, static_cast<void*>(data.data()));
    return data;
  }
  [[nodiscard]]
  GLsizeiptr           size         () const
  {
    return get_parameter(GL_BUFFER_SIZE);
  }
  [[nodiscard]]
  GLenum               usage        () const
  {
    return get_parameter(GL_BUFFER_USAGE);
  }
  [[nodiscard]]
  GLenum               access       () const
  {
    return get_parameter(GL_BUFFER_ACCESS);
  }
  [[nodiscard]]
  GLbitfield           access_flags () const
  {
    return get_parameter(GL_BUFFER_ACCESS_FLAGS);
  }
  [[nodiscard]]
  bool                 is_mapped    () const
  {
    return get_parameter(GL_BUFFER_MAPPED) != 0;
  }
  [[nodiscard]]
  bool                 is_immutable () const
  {
    return get_parameter(GL_BUFFER_IMMUTABLE_STORAGE) != 0;
  }
  [[nodiscard]]
  GLbitfield           storage_flags() const
  {
    return get_parameter(GL_BUFFER_STORAGE_FLAGS);
  }
  [[nodiscard]]
  GLintptr             map_offset   () const
  {
    return get_parameter_64(GL_BUFFER_MAP_OFFSET);
  }
  [[nodiscard]]
  GLsizeiptr           map_size     () const
  {
    return get_parameter_64(GL_BUFFER_MAP_LENGTH);
  }
  [[nodiscard]]
  void*                map_pointer  () const
  {
    void* pointer;
    glGetNamedBufferPointerv(id_, GL_BUFFER_MAP_POINTER, &pointer);
    return pointer;
  }

  static const GLenum native_type = GL_BUFFER;

  [[nodiscard]]
  GLuint id() const
  {
    return id_;
  }

#ifdef GL_CUDA_INTEROP_SUPPORT
  void cuda_register  (cudaGraphicsMapFlags flags = cudaGraphicsMapFlagsNone)
  {
    if (resource_ != nullptr)
      cuda_unregister();
    cudaGraphicsGLRegisterBuffer(&resource_, id_, flags);
  }
  void cuda_unregister()
  {
    if (resource_ == nullptr)
      return;
    cudaGraphicsUnregisterResource(resource_);
    resource_ = nullptr;
  }

  template<typename type>
  type* cuda_map  ()
  {
    type*       buffer_ptr;
    std::size_t buffer_size;
    cudaGraphicsMapResources(1, &resource_, nullptr);
    cudaGraphicsResourceGetMappedPointer(static_cast<void**>(&buffer_ptr), &buffer_size, resource_);
    return buffer_ptr;
  }
  void  cuda_unmap()
  {
    cudaGraphicsUnmapResources(1, &resource_, nullptr);
  }
#endif

protected:
  [[nodiscard]]
  GLint   get_parameter   (const GLenum parameter) const
  {
    GLint result;
    glGetNamedBufferParameteriv(id_, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint64 get_parameter_64(const GLenum parameter) const
  {
    GLint64 result;
    glGetNamedBufferParameteri64v(id_, parameter, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;

#ifdef GL_CUDA_INTEROP_SUPPORT
  cudaGraphicsResource* resource_ = nullptr;
#endif
};
}

#endif