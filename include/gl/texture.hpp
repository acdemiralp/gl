//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_TEXTURE_HPP
#define GL_TEXTURE_HPP

#include <array>
#include <cstddef>
#include <utility>
#include <vector>

#include <gl/buffer.hpp>
#include <gl/opengl.hpp>
#include <gl/renderbuffer.hpp>

#ifdef GL_CUDA_INTEROP_SUPPORT
  #include <cuda_gl_interop.h>
  #include <cuda_runtime_api.h>
#endif

namespace gl
{
// Next steps: Increase std::enable_if checks based on texture and function dimensions.
template<GLenum target>
class texture
{
public:
  // 8.0 Textures and samplers.
  static void set_active(const GLenum texture_unit)
  {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
  }

  // 8.1 Texture objects.
  texture           ()
  {
    glCreateTextures(target, 1, &id_);
  }
  explicit texture  (const GLuint id) : id_(id), managed_(false)
  {

  }
  texture           (const texture&  that) = delete;
  texture           (      texture&& temp) noexcept : id_(temp.id_), managed_(temp.managed_)
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
  virtual ~texture  ()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteTextures(1, &id_);
  }
  texture& operator=(const texture&  that) = delete;
  texture& operator=(      texture&& temp) noexcept
  {
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteTextures(1, &id_);

      id_       = temp.id_;
      managed_  = temp.managed_;
#ifdef GL_CUDA_INTEROP_SUPPORT
      resource_ = std::move(temp.resource_);
#endif

      temp.id_       = invalid_id;
      temp.managed_  = false     ;
#ifdef GL_CUDA_INTEROP_SUPPORT
      temp.resource_ = nullptr   ;
#endif
    }
  return *this;
  }

  void        bind      () const
  {
    glBindTexture(target, id_);
  }
  static void unbind    ()
  {
    glBindTexture(target, 0  );
  }
  void        bind_unit (const GLuint unit) const
  {
    glBindTextureUnit(unit, id_);
  }
  [[nodiscard]]
  bool        is_valid  () const
  {
    return glIsTexture(id_);
  }

  // 8.6 Alternate texture image specification.
  void set_sub_image (const GLint level, const GLint x,                               const GLsizei width,                                            const GLenum format, const GLenum type, const void* data) const
  {
    glTextureSubImage1D(id_, level, x,        width,                format, type, data);
  }
  void set_sub_image (const GLint level, const GLint x, const GLint y,                const GLsizei width, const GLsizei height,                      const GLenum format, const GLenum type, const void* data) const
  {
    glTextureSubImage2D(id_, level, x, y,     width, height,        format, type, data);
  }
  void set_sub_image (const GLint level, const GLint x, const GLint y, const GLint z, const GLsizei width, const GLsizei height, const GLsizei depth, const GLenum format, const GLenum type, const void* data) const
  {
    glTextureSubImage3D(id_, level, x , y, z, width, height, depth, format, type, data);
  }
  void copy_sub_image(const GLint level, const GLint x,                               const GLint read_buffer_x, const GLint read_buffer_y, const GLsizei width) const
  {
    // Note: Copies from read buffer.
    glCopyTextureSubImage1D(id_, level, x,       read_buffer_x, read_buffer_y, width);
  }
  void copy_sub_image(const GLint level, const GLint x, const GLint y,                const GLint read_buffer_x, const GLint read_buffer_y, const GLsizei width, const GLsizei height) const
  {
    // Note: Copies from read buffer.
    glCopyTextureSubImage2D(id_, level, x, y,    read_buffer_x, read_buffer_y, width, height);
  }
  void copy_sub_image(const GLint level, const GLint x, const GLint y, const GLint z, const GLint read_buffer_x, const GLint read_buffer_y, const GLsizei width, const GLsizei height) const
  {
    // Note: Copies from read buffer.
    glCopyTextureSubImage3D(id_, level, x, y, z, read_buffer_x, read_buffer_y, width, height);
  }
  
  // 8.7 Compressed texture images.
  void set_compressed_sub_image(const GLint level, const GLint x,                               const GLsizei width,                                            const GLenum format, const GLsizei size, const void* data) const
  {
    glCompressedTextureSubImage1D(id_, level, x,       width,                format, size, data);
  }
  void set_compressed_sub_image(const GLint level, const GLint x, const GLint y,                const GLsizei width, const GLsizei height,                      const GLenum format, const GLsizei size, const void* data) const
  {
    glCompressedTextureSubImage2D(id_, level, x, y,    width, height,        format, size, data);
  }
  void set_compressed_sub_image(const GLint level, const GLint x, const GLint y, const GLint z, const GLsizei width, const GLsizei height, const GLsizei depth, const GLenum format, const GLsizei size, const void* data) const
  {
    glCompressedTextureSubImage3D(id_, level, x, y, z, width, height, depth, format, size, data);
  }

  // 8.9 Buffer textures.
  void attach_buffer_range (const GLenum internal_format, const buffer& buffer, const GLintptr offset, const GLsizeiptr size) const
  {
    glTextureBufferRange(id_, internal_format, buffer.id(), offset, size);
  }
  void attach_buffer       (const GLenum internal_format, const buffer& buffer) const
  {
    glTextureBuffer     (id_, internal_format, buffer.id());
  }
  
  // 8.10 Texture parameters.
  void set_depth_stencil_mode(const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_DEPTH_STENCIL_TEXTURE_MODE, mode);
  }   
  void set_wrap_s            (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_WRAP_S, mode);
  }
  void set_wrap_t            (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_WRAP_T, mode);
  }
  void set_wrap_r            (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_WRAP_R, mode);
  }  
  void set_border_color      (const std::array<GLfloat, 4>& color                            ) const
  {
    glTextureParameterfv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_border_color      (const std::array<GLint  , 4>& color, const bool convert = false) const
  {
    convert 
    ? glTextureParameteriv (id_, GL_TEXTURE_BORDER_COLOR, color.data())
    : glTextureParameterIiv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_border_color      (const std::array<GLuint , 4>& color                            ) const
  {
    glTextureParameterIuiv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_min_filter        (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, mode);
  }
  void set_mag_filter        (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, mode);
  }
  void set_lod_bias          (const GLfloat                 bias                             ) const
  {
    glTextureParameterf(id_, GL_TEXTURE_LOD_BIAS, bias);
  }
  void set_min_lod           (const GLfloat                 value                            ) const
  {
    glTextureParameterf(id_, GL_TEXTURE_MIN_LOD, value);
  }
  void set_max_lod           (const GLfloat                 value                            ) const
  {
    glTextureParameterf(id_, GL_TEXTURE_MAX_LOD, value);
  }
  void set_max_anisotropy    (const GLfloat                 max_anisotropy                   ) const
  {
    glTextureParameterf(id_, GL_TEXTURE_MAX_ANISOTROPY, max_anisotropy);
  }
  void set_base_level        (const GLuint                  value                            ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_BASE_LEVEL, value);
  }
  void set_max_level         (const GLuint                  value                            ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_MAX_LEVEL, value);
  }
  void set_swizzle_r         (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_SWIZZLE_R, mode);
  }
  void set_swizzle_g         (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_SWIZZLE_G, mode);
  }
  void set_swizzle_b         (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_SWIZZLE_B, mode);
  }
  void set_swizzle_a         (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_SWIZZLE_A, mode);
  }
  void set_swizzle_rgba      (const std::array<GLenum, 4>&  modes                            ) const
  {
    glTextureParameteriv(id_, GL_TEXTURE_SWIZZLE_RGBA, reinterpret_cast<const GLint*>(modes.data()));
  }
  void set_compare_mode      (const GLenum                  mode                             ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_COMPARE_MODE, mode);
  }
  void set_compare_func      (const GLenum                  function                         ) const
  {
    glTextureParameteri(id_, GL_TEXTURE_COMPARE_FUNC, function);
  }   
  
  // 8.11 Texture queries.
  [[nodiscard]]
  GLenum                 compatibility_type() const
  {
    return get_int_parameter(GL_IMAGE_FORMAT_COMPATIBILITY_TYPE);
  }
  [[nodiscard]]
  bool                   is_immutable      () const
  {
    return get_int_parameter(GL_TEXTURE_IMMUTABLE_FORMAT);
  }
  [[nodiscard]]
  GLenum                 depth_stencil_mode() const
  {
    return get_int_parameter(GL_DEPTH_STENCIL_TEXTURE_MODE);
  }
  [[nodiscard]]
  GLenum                 wrap_s            () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_S);
  }
  [[nodiscard]]
  GLenum                 wrap_t            () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_T);
  }
  [[nodiscard]]
  GLenum                 wrap_r            () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_R);
  }
  [[nodiscard]]
  std::array<GLfloat, 4> border_color      () const
  {
    return get_float_parameter<4>(GL_TEXTURE_BORDER_COLOR);
  }
  [[nodiscard]]
  std::array<GLint, 4>   border_color_int  () const
  {
    return get_int_parameter<4>(GL_TEXTURE_BORDER_COLOR);
  }
  [[nodiscard]]
  GLenum                 min_filter        () const
  {
    return get_int_parameter(GL_TEXTURE_MIN_FILTER);
  }
  [[nodiscard]]
  GLenum                 mag_filter        () const
  {
    return get_int_parameter(GL_TEXTURE_MAG_FILTER);
  }
  [[nodiscard]]
  GLfloat                lod_bias          () const
  {
    return get_float_parameter(GL_TEXTURE_LOD_BIAS);
  }
  [[nodiscard]]
  GLfloat                min_lod           () const
  {
    return get_float_parameter(GL_TEXTURE_MIN_LOD);
  }
  [[nodiscard]]
  GLfloat                max_lod           () const
  {
    return get_float_parameter(GL_TEXTURE_MAX_LOD);
  }
  [[nodiscard]]
  GLfloat                max_anisotropy    () const
  {
    return get_float_parameter(GL_TEXTURE_MAX_ANISOTROPY);
  }
  [[nodiscard]]
  GLuint                 base_level        () const
  {
    return get_int_parameter(GL_TEXTURE_BASE_LEVEL);
  }
  [[nodiscard]]
  GLuint                 max_level         () const
  {
    return get_int_parameter(GL_TEXTURE_MAX_LEVEL);
  }
  [[nodiscard]]
  GLenum                 swizzle_r         () const
  {
    return get_int_parameter(GL_TEXTURE_SWIZZLE_R);
  }
  [[nodiscard]]
  GLenum                 swizzle_g         () const
  {
    return get_int_parameter(GL_TEXTURE_SWIZZLE_G);
  }
  [[nodiscard]]
  GLenum                 swizzle_b         () const
  {
    return get_int_parameter(GL_TEXTURE_SWIZZLE_B);
  }
  [[nodiscard]]
  GLenum                 swizzle_a         () const
  {
    return get_int_parameter(GL_TEXTURE_SWIZZLE_A);
  }
  [[nodiscard]]
  std::array<GLint, 4>   swizzle_rgba      () const
  {
    return get_int_parameter<4>(GL_TEXTURE_SWIZZLE_RGBA);
  }
  [[nodiscard]]
  GLenum                 compare_mode      () const
  {
    return get_int_parameter(GL_TEXTURE_COMPARE_MODE);
  }
  [[nodiscard]]
  GLenum                 compare_func      () const
  {
    return get_int_parameter(GL_TEXTURE_COMPARE_FUNC);
  }   

  [[nodiscard]]
  GLsizei  width                 (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_WIDTH);
  }
  [[nodiscard]]
  GLsizei  height                (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_HEIGHT);
  }
  [[nodiscard]]
  GLsizei  depth                 (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_DEPTH);
  }
  [[nodiscard]]
  bool     fixed_sample_locations(const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_FIXED_SAMPLE_LOCATIONS);
  }
  [[nodiscard]]
  GLenum   internal_format       (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_INTERNAL_FORMAT);
  }
  [[nodiscard]]
  GLsizei  shared_size           (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_SHARED_SIZE);
  }
  [[nodiscard]]
  bool     is_compressed         (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_COMPRESSED);
  }
  [[nodiscard]]
  GLsizei  compressed_size       (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE);
  }
  [[nodiscard]]
  GLsizei  samples               (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_SAMPLES);
  }
  [[nodiscard]]
  GLintptr buffer_offset         (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_BUFFER_OFFSET);
  }
  [[nodiscard]]
  GLsizei  buffer_size           (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_BUFFER_SIZE);
  }
  [[nodiscard]]
  GLsizei  red_size              (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_RED_SIZE);
  }
  [[nodiscard]]
  GLsizei  green_size            (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_GREEN_SIZE);
  }
  [[nodiscard]]
  GLsizei  blue_size             (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_BLUE_SIZE);
  }
  [[nodiscard]]
  GLsizei  alpha_size            (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_ALPHA_SIZE);
  }
  [[nodiscard]]
  GLsizei  depth_size            (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_DEPTH_SIZE);
  }
  [[nodiscard]]
  GLenum   red_type              (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_RED_TYPE);
  }
  [[nodiscard]]
  GLenum   green_type            (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_GREEN_TYPE);
  }
  [[nodiscard]]
  GLenum   blue_type             (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_BLUE_TYPE);
  }
  [[nodiscard]]
  GLenum   alpha_type            (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_ALPHA_TYPE);
  }
  [[nodiscard]]
  GLenum   depth_type            (const GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_DEPTH_TYPE);
  }

  [[nodiscard]]
  std::vector<GLubyte> image               (const GLint level,                                                                                                              const GLenum format, const GLenum type) const
  {
    auto w = width (level); if (w == 0) w = 1;
    auto h = height(level); if (h == 0) h = 1;
    auto d = depth (level); if (d == 0) d = 1;
    std::vector<GLubyte> data(w * h * d * format_component_count(format) * type_size(type));
    glGetTextureImage(id_, level, format, type, static_cast<GLsizei>(data.size()), static_cast<void*>(data.data()));
    return data;
  }
  [[nodiscard]]
  std::vector<GLubyte> sub_image           (const GLint level, const GLint x, const GLint y, const GLint z, const GLsizei width, const GLsizei height, const GLsizei depth, const GLenum format, const GLenum type) const
  {
    auto w = width ; if (w == 0) w = 1;
    auto h = height; if (h == 0) h = 1;
    auto d = depth ; if (d == 0) d = 1;
    std::vector<GLubyte> data(w * h * d * format_component_count(format) * type_size(type));
    glGetTextureSubImage(id_, level, x, y, z, w, h, d, format, type, static_cast<GLsizei>(data.size()), static_cast<void*>(data.data()));
    return data;
  }
  [[nodiscard]]
  std::vector<GLubyte> compressed_image    (const GLint level) const
  {
    std::vector<GLubyte> data(compressed_size(level));
    glGetCompressedTextureImage(id_, level, static_cast<GLsizei>(data.size()), static_cast<void*>(data.data()));
    return data;
  }
  [[nodiscard]]
  std::vector<GLubyte> compressed_sub_image(const GLint level, const GLint x, const GLint y, const GLint z, const GLsizei width, const GLsizei height, const GLsizei depth, const GLsizei buffer_size) const
  {
    // Note: Buffer size is exposed since it cannot be calculated due to lack of format and type information.
    std::vector<GLubyte> data(buffer_size);
    glGetCompressedTextureSubImage(id_, level, x, y, z, width, height, depth, static_cast<GLsizei>(data.size()), static_cast<void*>(data.data()));
    return data;
  }

  // 8.14.4 Manual mipmap generation.
  void generate_mipmap() const
  {
    glGenerateTextureMipmap(id_);
  }

  // 8.19 Immutable format texture images.
  void set_storage            (const GLsizei levels , const GLenum internal_format, const GLsizei width) const
  {
    glTextureStorage1D(id_, levels, internal_format, width);
  }
  void set_storage            (const GLsizei levels , const GLenum internal_format, const GLsizei width, const GLsizei height) const
  {
    glTextureStorage2D(id_, levels, internal_format, width, height);
  }
  void set_storage            (const GLsizei levels , const GLenum internal_format, const GLsizei width, const GLsizei height, const GLsizei depth) const
  {
    glTextureStorage3D(id_, levels, internal_format, width, height, depth);
  }
  void set_storage_multisample(const GLsizei samples, const GLenum internal_format, const GLsizei width, const GLsizei height,                      const bool fixed_sample_locations = true) const
  {
    glTextureStorage2DMultisample(id_, samples, internal_format, width, height,        fixed_sample_locations);
  }
  void set_storage_multisample(const GLsizei samples, const GLenum internal_format, const GLsizei width, const GLsizei height, const GLsizei depth, const bool fixed_sample_locations = true) const
  {
    glTextureStorage3DMultisample(id_, samples, internal_format, width, height, depth, fixed_sample_locations);
  }

  // 8.20 Invalidate texture image data.
  void invalidate_sub_image(const GLint level, const GLint x, const GLint y, const GLint z, const GLsizei width, const GLsizei height = 1, const GLsizei depth = 1) const
  {
    glInvalidateTexSubImage(id_, level, x, y, z, width, height, depth);
  }
  void invalidate          (const GLint level = 0) const
  {
    glInvalidateTexImage(id_, level);
  }

  // 8.21 Clear texture image data.
  void clear_sub_image     (const GLint level, const GLint x, const GLint y, const GLint z, const GLsizei width, const GLsizei height, const GLsizei depth, const GLenum format, const GLenum type, const void* data = nullptr) const
  {
    glClearTexSubImage(id_, level, x, y, z, width, height, depth, format, type, data);
  }
  void clear               (const GLint level,                                                                                                              const GLenum format, const GLenum type, const void* data = nullptr) const
  {
    glClearTexImage(id_, level, format, type, data);
  }

  // 8.26 Texture image loads / stores.
  void bind_image_texture  (const GLuint unit, const GLint level, const bool layered, const GLint layer, const GLenum access, const GLenum format) const
  {
    glBindImageTexture(unit, id_, level, layered, layer, access, format);
  }

  // 18.3 Copying pixels.
  template<GLenum source_target>
  void copy_image_sub_data(const texture<source_target>& source, 
    const GLint source_level, const GLint source_x, const GLint source_y, const GLint source_z,
    const GLint level       , const GLint x       , const GLint y       , const GLint z       , const GLint width, const GLint height, const GLint depth) const
  {
    glCopyImageSubData(source.id(), source_target, source_level, source_x, source_y, source_z, id_, target, level, x, y, z, width, height, depth);
  }
  void copy_image_sub_data(const renderbuffer& source, 
    const GLint source_level, const GLint source_x, const GLint source_y, const GLint source_z,
    const GLint level       , const GLint x       , const GLint y       , const GLint z       , const GLint width, const GLint height, const GLint depth) const
  {
    glCopyImageSubData(source.id(), GL_RENDERBUFFER, source_level, source_x, source_y, source_z, id_, target, level, x, y, z, width, height, depth);
  }
  
  // 22.3 Internal format queries.
  static GLint   internal_format_info   (const GLenum internal_format, const GLenum parameter)
  {
    GLint result;
    glGetInternalformativ(target, internal_format, parameter, 1, &result);
    return result;
  }
  static GLint64 internal_format_info_64(const GLenum internal_format, const GLenum parameter)
  {
    GLint64 result;
    glGetInternalformati64v(target, internal_format, parameter, 1, &result);
    return result;
  }

  static const GLenum native_type = GL_TEXTURE;

  [[nodiscard]]
  GLuint id() const
  {
    return id_;
  }
  
#ifdef GL_CUDA_INTEROP_SUPPORT
  void cuda_register  (const cudaGraphicsMapFlags flags = cudaGraphicsMapFlagsNone)
  {
    if (resource_ != nullptr)
      cuda_unregister();
    cudaGraphicsGLRegisterImage(&resource_, id_, target, flags);
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
  template<std::size_t count>
  std::array<GLint, count>   get_int_parameter        (const GLenum parameter) const
  {
    std::array<GLint, count> result;
    glGetTextureParameteriv(id_, parameter, result.data());
    return result;
  }
  [[nodiscard]]
  GLint                      get_int_parameter        (const GLenum parameter) const
  {
    GLint result;
    glGetTextureParameteriv(id_, parameter, &result);
    return result;
  }
  template<std::size_t count>
  std::array<GLfloat, count> get_float_parameter      (const GLenum parameter) const
  {
    std::array<GLfloat, count> result;
    glGetTextureParameterfv(id_, parameter, result.data());
    return result;
  }
  [[nodiscard]]
  GLfloat                    get_float_parameter      (const GLenum parameter) const
  {
    GLfloat result;
    glGetTextureParameterfv(id_, parameter, &result);
    return result;
  }
  template<std::size_t count>
  std::array<GLint, count>   get_int_level_parameter  (const GLuint level, const GLenum parameter) const
  {
    std::array<GLint, count> result;
    glGetTextureLevelParameteriv(id_, level, parameter, result.data());
    return result;
  }
  [[nodiscard]]
  GLint                      get_int_level_parameter  (const GLuint level, const GLenum parameter) const
  {
    GLint result;
    glGetTextureLevelParameteriv(id_, level, parameter, &result);
    return result;
  }
  template<std::size_t count>
  std::array<GLfloat, count> get_float_level_parameter(const GLuint level, const GLenum parameter) const
  {
    std::array<GLfloat, count> result;
    glGetTextureLevelParameterfv(id_, level, parameter, result.data());
    return result;
  }
  [[nodiscard]]
  GLfloat                    get_float_level_parameter(const GLuint level, const GLenum parameter) const
  {
    GLfloat result;
    glGetTextureLevelParameterfv(id_, level, parameter, &result);
    return result;
  }
  
  GLuint id_      = invalid_id;
  bool   managed_ = true;

#ifdef GL_CUDA_INTEROP_SUPPORT
  cudaGraphicsResource* resource_ = nullptr;
#endif
};

using texture_1d                         = texture<GL_TEXTURE_1D>;
using texture_2d                         = texture<GL_TEXTURE_2D>;
using texture_3d                         = texture<GL_TEXTURE_3D>;
using texture_1d_array                   = texture<GL_TEXTURE_1D_ARRAY>;
using texture_2d_array                   = texture<GL_TEXTURE_2D_ARRAY>;
using rectangle_texture                  = texture<GL_TEXTURE_RECTANGLE>;
using cubemap_texture                    = texture<GL_TEXTURE_CUBE_MAP>;
using cubemap_texture_array              = texture<GL_TEXTURE_CUBE_MAP_ARRAY>;
using buffer_texture                     = texture<GL_TEXTURE_BUFFER>;
using texture_2d_multisample             = texture<GL_TEXTURE_2D_MULTISAMPLE>;
using texture_2d_multisample_array       = texture<GL_TEXTURE_2D_MULTISAMPLE_ARRAY>;
using proxy_texture_1d                   = texture<GL_PROXY_TEXTURE_1D>;
using proxy_texture_2d                   = texture<GL_PROXY_TEXTURE_2D>;
using proxy_texture_3d                   = texture<GL_PROXY_TEXTURE_3D>;
using proxy_texture_1d_array             = texture<GL_PROXY_TEXTURE_1D_ARRAY>;
using proxy_texture_2d_array             = texture<GL_PROXY_TEXTURE_2D_ARRAY>;
using proxy_rectangle_texture            = texture<GL_PROXY_TEXTURE_RECTANGLE>;
using proxy_cubemap_texture              = texture<GL_PROXY_TEXTURE_CUBE_MAP>;
using proxy_cubemap_texture_array        = texture<GL_PROXY_TEXTURE_CUBE_MAP_ARRAY>;
using proxy_texture_2d_multisample       = texture<GL_PROXY_TEXTURE_2D_MULTISAMPLE>;
using proxy_texture_2d_multisample_array = texture<GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY>;

inline void set_seamless_cubemap_enabled(const bool enabled)
{
  enabled ? glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS) : glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}
inline bool seamless_cubemap_enabled    ()
{
  return glIsEnabled(GL_TEXTURE_CUBE_MAP_SEAMLESS) != 0;
}
}

#endif
