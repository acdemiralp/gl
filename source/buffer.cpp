//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/buffer.hpp>

namespace gl
{
// 6.0 Buffer objects.
buffer::buffer ()
{
  glCreateBuffers(1, &id_);
}
buffer::buffer (GLuint id): id_(id), managed_(false)
{

}
buffer::buffer (const buffer& that): buffer()
{
  that.is_immutable()
    ? set_data_immutable(that.size(), nullptr, that.storage_flags())
    : set_data          (that.size(), nullptr, that.usage        ());
  copy_sub_data(that, 0, 0, size());
}
buffer::~buffer()
{
  if (managed_)
  glDeleteBuffers(1, &id_);
}
buffer& buffer::operator=(const buffer& that)
{
  that.is_immutable()
    ? set_data_immutable(that.size(), nullptr, that.storage_flags())
    : set_data          (that.size(), nullptr, that.usage        ());
  copy_sub_data(that, 0, 0, size());
  return *this;
}

// 6.2 Create / modify buffer object data (bindless).
void  buffer::set_data_immutable (GLsizeiptr size, const void* data, GLbitfield storage_flags)
{
  glNamedBufferStorage(id_, size, data, storage_flags);
}
void  buffer::set_data           (GLsizeiptr size, const void* data, GLenum     usage        )
{
  glNamedBufferData(id_, size, data, usage);
}
void  buffer::set_sub_data       (GLintptr offset, GLsizeiptr size, const void* data)
{
  glNamedBufferSubData(id_, offset, size, data);
}
void  buffer::clear_sub_data     (GLenum internal_format, GLintptr offset, GLsizeiptr size, GLenum format, GLenum data_type, const void* data)
{
  glClearNamedBufferSubData(id_, internal_format, offset, size, format, data_type, data);
}
void  buffer::clear_data         (GLenum internal_format,                                   GLenum format, GLenum data_type, const void* data)
{
  glClearNamedBufferData(id_, internal_format, format, data_type, data);
}

// 6.3 Map / unmap buffer data (bindless).
void* buffer::map_range          (GLintptr offset, GLsizeiptr size, GLbitfield access_flags)
{
  return glMapNamedBufferRange(id_, offset, size, access_flags);
}
void* buffer::map                (                                  GLenum     access      )
{
  return glMapNamedBuffer(id_, access);
}
void  buffer::flush_mapped_range (GLintptr offset, GLsizeiptr size)
{
  glFlushMappedNamedBufferRange(id_, offset, size);
}
void  buffer::unmap              ()
{
  glUnmapNamedBuffer(id_);
}

// 6.5 Invalidate buffer data (bindless).
void  buffer::invalidate_sub_data(GLintptr offset, GLsizeiptr size)
{
  glInvalidateBufferSubData(id_, offset, size);
}
void  buffer::invalidate         ()
{
  glInvalidateBufferData(id_);
}

// 6.6 Copy between buffers (bindless).
void  buffer::copy_sub_data      (const buffer& source, GLintptr source_offset, GLintptr offset, GLsizeiptr size)
{
  glCopyNamedBufferSubData(source.id_, id_, source_offset, offset, size);
}

// 6.7 Buffer object queries (bindless).
bool                 buffer::is_valid     () const
{
  return glIsBuffer(id_) != 0;
}
std::vector<GLubyte> buffer::sub_data     (GLintptr offset, GLsizeiptr size) const
{
  std::vector<GLubyte> data(size);
  glGetNamedBufferSubData(id_, offset, size, static_cast<void*>(data.data()));
  return data;
}
GLsizeiptr           buffer::size         () const
{
  return get_parameter(GL_BUFFER_SIZE);
}
GLenum               buffer::usage        () const
{
  return get_parameter(GL_BUFFER_USAGE);
}
GLenum               buffer::access       () const
{
  return get_parameter(GL_BUFFER_ACCESS);
}
GLbitfield           buffer::access_flags () const
{
  return get_parameter(GL_BUFFER_ACCESS_FLAGS);
}
bool                 buffer::is_mapped    () const
{
  return get_parameter(GL_BUFFER_MAPPED) != 0;
}
bool                 buffer::is_immutable () const
{
  return get_parameter(GL_BUFFER_IMMUTABLE_STORAGE) != 0;
}
GLbitfield           buffer::storage_flags() const
{
  return get_parameter(GL_BUFFER_STORAGE_FLAGS);
}
GLintptr             buffer::map_offset   () const
{
  return get_parameter_64(GL_BUFFER_MAP_OFFSET);
}
GLsizeiptr           buffer::map_size     () const
{
  return get_parameter_64(GL_BUFFER_MAP_LENGTH);
}
void*                buffer::map_pointer  () const
{
  void* pointer;
  glGetNamedBufferPointerv(id_, GL_BUFFER_MAP_POINTER, &pointer);
  return pointer;
}

GLuint buffer::id() const
{
  return id_;
}

#ifdef GL_CUDA_INTEROP_SUPPORT
void buffer::cuda_register  (cudaGraphicsMapFlags flags)
{
  if (resource_ != nullptr)
    cuda_unregister();
  cudaGraphicsGLRegisterBuffer(&resource_, id_, flags);
}
void buffer::cuda_unregister()
{
  if (resource_ == nullptr)
    return;
  cudaGraphicsUnregisterResource(resource_);
  resource_ = nullptr;
}

void buffer::cuda_unmap     ()
  {
    cudaGraphicsUnmapResources(1, &resource_, nullptr);
  }
#endif

GLint   buffer::get_parameter   (GLenum parameter) const
{
  GLint result;
  glGetNamedBufferParameteriv  (id_, parameter, &result);
  return result;
}
GLint64 buffer::get_parameter_64(GLenum parameter) const
{
  GLint64 result;
  glGetNamedBufferParameteri64v(id_, parameter, &result);
  return result;
}
}