//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_TEXTURE_HPP_
#define GL_TEXTURE_HPP_

#include <array>
#include <cstddef>

#include <gl/buffer.hpp>
#include <gl/export.hpp>
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
  static void set_active(GLenum texture_unit)
  {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
  }

  // 8.1 Texture objects.
  texture()
  {
    glCreateTextures(target, 1, &id_);
  }
  texture(GLuint id) : id_(id), managed_(false)
  {

  }
  texture(const texture&  that) = delete ;
  texture(      texture&& temp) = default;
  virtual ~texture()
  {
    if (managed_)
      glDeleteTextures(1, &id_);
  }
  texture& operator=(const texture&  that) = delete ;
  texture& operator=(      texture&& temp) = default;

  void        bind      () const
  {
    glBindTexture(target, id_);
  }
  static void unbind    ()
  {
    glBindTexture(target, 0  );
  }
  void        bind_unit (GLuint unit) const
  {
    glBindTextureUnit(unit, id_);
  }
  bool        is_valid  () const
  {
    return glIsTexture(id_);
  }

  // 8.6 Alternate texture image specification.
  void set_sub_image (GLint level, GLint x,                   GLsizei width,                                GLenum format, GLenum type, const void* data)
  {
    glTextureSubImage1D(id_, level, x,        width,                format, type, data);
  }
  void set_sub_image (GLint level, GLint x, GLint y,          GLsizei width, GLsizei height,                GLenum format, GLenum type, const void* data)
  {
    glTextureSubImage2D(id_, level, x, y,     width, height,        format, type, data);
  }
  void set_sub_image (GLint level, GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* data)
  {
    glTextureSubImage3D(id_, level, x , y, z, width, height, depth, format, type, data);
  }
  void copy_sub_image(GLint level, GLint x,                   GLint read_buffer_x, GLint read_buffer_y, GLsizei width)
  {
    // Note: Copies from read buffer.
    glCopyTextureSubImage1D(id_, level, x,       read_buffer_x, read_buffer_y, width);
  }
  void copy_sub_image(GLint level, GLint x, GLint y,          GLint read_buffer_x, GLint read_buffer_y, GLsizei width, GLsizei height)
  {
    // Note: Copies from read buffer.
    glCopyTextureSubImage2D(id_, level, x, y,    read_buffer_x, read_buffer_y, width, height);
  }
  void copy_sub_image(GLint level, GLint x, GLint y, GLint z, GLint read_buffer_x, GLint read_buffer_y, GLsizei width, GLsizei height)
  {
    // Note: Copies from read buffer.
    glCopyTextureSubImage3D(id_, level, x, y, z, read_buffer_x, read_buffer_y, width, height);
  }
  
  // 8.7 Compressed texture images.
  void set_compressed_sub_image(GLint level, GLint x,                   GLsizei width,                                GLenum format, GLsizei size, const void* data)
  {
    glCompressedTextureSubImage1D(id_, level, x,       width,                format, size, data);
  }
  void set_compressed_sub_image(GLint level, GLint x, GLint y,          GLsizei width, GLsizei height,                GLenum format, GLsizei size, const void* data)
  {
    glCompressedTextureSubImage2D(id_, level, x, y,    width, height,        format, size, data);
  }
  void set_compressed_sub_image(GLint level, GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei size, const void* data)
  {
    glCompressedTextureSubImage3D(id_, level, x, y, z, width, height, depth, format, size, data);
  }

  // 8.9 Buffer textures.
  void attach_buffer_range (GLenum internal_format, const buffer& buffer, GLintptr offset, GLsizeiptr size)
  {
    glTextureBufferRange(id_, internal_format, buffer.id(), offset, size);
  }
  void attach_buffer       (GLenum internal_format, const buffer& buffer)
  {
    glTextureBuffer     (id_, internal_format, buffer.id());
  }
  
  // 8.10 Texture parameters.
  void set_depth_stencil_mode(GLenum  mode )
  {
    glTextureParameteri(id_, GL_DEPTH_STENCIL_TEXTURE_MODE, mode);
  }   
  void set_wrap_s            (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_WRAP_S, mode);
  }
  void set_wrap_t            (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_WRAP_T, mode);
  }
  void set_wrap_r            (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_WRAP_R, mode);
  }  
  void set_border_color      (const std::array<GLfloat, 4>& color)
  {
    glTextureParameterfv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_border_color      (const std::array<GLint  , 4>& color, bool convert = false)
  {
    convert 
    ? glTextureParameteriv (id_, GL_TEXTURE_BORDER_COLOR, color.data())
    : glTextureParameterIiv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_border_color      (const std::array<GLuint , 4>& color)
  {
    glTextureParameterIuiv(id_, GL_TEXTURE_BORDER_COLOR, color.data());
  }
  void set_min_filter        (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, mode);
  }
  void set_mag_filter        (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, mode);
  }
  void set_lod_bias          (GLfloat bias )
  {
    glTextureParameterf(id_, GL_TEXTURE_LOD_BIAS, bias);
  }
  void set_min_lod           (GLfloat value)
  {
    glTextureParameterf(id_, GL_TEXTURE_MIN_LOD, value);
  }
  void set_max_lod           (GLfloat value)
  {
    glTextureParameterf(id_, GL_TEXTURE_MAX_LOD, value);
  }
  void set_max_anisotropy    (GLfloat max_anisotropy)
  {
    glTextureParameterf(id_, GL_TEXTURE_MAX_ANISOTROPY, max_anisotropy);
  }
  void set_base_level        (GLuint  value)
  {
    glTextureParameteri(id_, GL_TEXTURE_BASE_LEVEL, value);
  }
  void set_max_level         (GLuint  value)
  {
    glTextureParameteri(id_, GL_TEXTURE_MAX_LEVEL, value);
  }
  void set_swizzle_r         (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_SWIZZLE_R, mode);
  }
  void set_swizzle_g         (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_SWIZZLE_G, mode);
  }
  void set_swizzle_b         (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_SWIZZLE_B, mode);
  }
  void set_swizzle_a         (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_SWIZZLE_A, mode);
  }
  void set_swizzle_rgba      (const std::array<GLenum, 4>& modes)
  {
    glTextureParameteriv(id_, GL_TEXTURE_SWIZZLE_RGBA, reinterpret_cast<const GLint*>(modes.data()));
  }
  void set_compare_mode      (GLenum  mode )
  {
    glTextureParameteri(id_, GL_TEXTURE_COMPARE_MODE, mode);
  }
  void set_compare_func      (GLenum  function)
  {
    glTextureParameteri(id_, GL_TEXTURE_COMPARE_FUNC, function);
  }   
  
  // 8.11 Texture queries.
  GLenum                 compatibility_type() const
  {
    return get_int_parameter(GL_IMAGE_FORMAT_COMPATIBILITY_TYPE);
  }
  bool                   is_immutable      () const
  {
    return get_int_parameter(GL_TEXTURE_IMMUTABLE_FORMAT);
  }
  GLenum                 depth_stencil_mode() const
  {
    return get_int_parameter(GL_DEPTH_STENCIL_TEXTURE_MODE);
  }   
  GLenum                 wrap_s            () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_S);
  }
  GLenum                 wrap_t            () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_T);
  }
  GLenum                 wrap_r            () const
  {
    return get_int_parameter(GL_TEXTURE_WRAP_R);
  }
  std::array<GLfloat, 4> border_color      () const
  {
    return get_float_parameter<4>(GL_TEXTURE_BORDER_COLOR);
  }
  std::array<GLint, 4>   border_color_int  () const
  {
    return get_int_parameter<4>(GL_TEXTURE_BORDER_COLOR);
  }
  GLenum                 min_filter        () const
  {
    return get_int_parameter(GL_TEXTURE_MIN_FILTER);
  }
  GLenum                 mag_filter        () const
  {
    return get_int_parameter(GL_TEXTURE_MAG_FILTER);
  }
  GLfloat                lod_bias          () const
  {
    return get_float_parameter(GL_TEXTURE_LOD_BIAS);
  }
  GLfloat                min_lod           () const
  {
    return get_float_parameter(GL_TEXTURE_MIN_LOD);
  }
  GLfloat                max_lod           () const
  {
    return get_float_parameter(GL_TEXTURE_MAX_LOD);
  }
  GLfloat                max_anisotropy    () const
  {
    return get_float_parameter(GL_TEXTURE_MAX_ANISOTROPY);
  }
  GLuint                 base_level        () const
  {
    return get_int_parameter(GL_TEXTURE_BASE_LEVEL);
  }
  GLuint                 max_level         () const
  {
    return get_int_parameter(GL_TEXTURE_MAX_LEVEL);
  }
  GLenum                 swizzle_r         () const
  {
    return get_int_parameter(GL_TEXTURE_SWIZZLE_R);
  }
  GLenum                 swizzle_g         () const
  {
    return get_int_parameter(GL_TEXTURE_SWIZZLE_G);
  }
  GLenum                 swizzle_b         () const
  {
    return get_int_parameter(GL_TEXTURE_SWIZZLE_B);
  }
  GLenum                 swizzle_a         () const
  {
    return get_int_parameter(GL_TEXTURE_SWIZZLE_A);
  }
  std::array<GLint, 4>   swizzle_rgba      () const
  {
    return get_int_parameter<4>(GL_TEXTURE_SWIZZLE_RGBA);
  }
  GLenum                 compare_mode      () const
  {
    return get_int_parameter(GL_TEXTURE_COMPARE_MODE);
  }
  GLenum                 compare_func      () const
  {
    return get_int_parameter(GL_TEXTURE_COMPARE_FUNC);
  }   
 
  GLsizei  width                 (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_WIDTH);
  }
  GLsizei  height                (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_HEIGHT);
  }
  GLsizei  depth                 (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_DEPTH);
  }
  bool     fixed_sample_locations(GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_FIXED_SAMPLE_LOCATIONS);
  }
  GLenum   internal_format       (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_INTERNAL_FORMAT);
  }
  GLsizei  shared_size           (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_SHARED_SIZE);
  }
  bool     is_compressed         (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_COMPRESSED);
  }
  GLsizei  compressed_size       (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE);
  }
  GLsizei  samples               (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_SAMPLES);
  }
  GLintptr buffer_offset         (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_BUFFER_OFFSET);
  }
  GLsizei  buffer_size           (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_BUFFER_SIZE);
  }
  GLsizei  red_size              (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_RED_SIZE);
  }
  GLsizei  green_size            (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_GREEN_SIZE);
  }
  GLsizei  blue_size             (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_BLUE_SIZE);
  }
  GLsizei  alpha_size            (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_ALPHA_SIZE);
  }
  GLsizei  depth_size            (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_DEPTH_SIZE);
  }
  GLenum   red_type              (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_RED_TYPE);
  }
  GLenum   green_type            (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_GREEN_TYPE);
  }
  GLenum   blue_type             (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_BLUE_TYPE);
  }
  GLenum   alpha_type            (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_ALPHA_TYPE);
  }
  GLenum   depth_type            (GLuint level = 0) const
  {
    return get_int_level_parameter(level, GL_TEXTURE_DEPTH_TYPE);
  }

  std::vector<GLubyte> image               (GLint level,                                                                          GLenum format, GLenum type) const
  {
    auto w = width (level); if (w == 0) w = 1;
    auto h = height(level); if (h == 0) h = 1;
    auto d = depth (level); if (d == 0) d = 1;
    std::vector<GLubyte> data(w * h * d * format_component_count(format) * type_size(type));
    glGetTextureImage(id_, level, format, type, data.size(), static_cast<void*>(data.data()));
    return data;
  }
  std::vector<GLubyte> sub_image           (GLint level, GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type) const
  {
    auto w = width ; if (w == 0) w = 1;
    auto h = height; if (h == 0) h = 1;
    auto d = depth ; if (d == 0) d = 1;
    std::vector<GLubyte> data(w * h * d * format_component_count(format) * type_size(type));
    glGetTextureSubImage(id_, level, x, y, z, w, h, d, format, type, data.size(), static_cast<void*>(data.data()));
    return data;
  }
  std::vector<GLubyte> compressed_image    (GLint level) const
  {
    std::vector<GLubyte> data(compressed_size(level));
    glGetCompressedTextureImage(id_, level, data.size(), static_cast<void*>(data.data()));
    return data;
  }
  std::vector<GLubyte> compressed_sub_image(GLint level, GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, GLsizei buffer_size) const
  {
    // Note: Buffer size is exposed since it cannot be calculated due to lack of format and type information.
    std::vector<GLubyte> data(buffer_size);
    glGetCompressedTextureSubImage(id_, level, x, y, z, width, height, depth, data.size(), static_cast<void*>(data.data()));
    return data;
  }

  // 8.14.4 Manual mipmap generation.
  void generate_mipmap()
  {
    glGenerateTextureMipmap(id_);
  }

  // 8.19 Immutable format texture images.
  void set_storage            (GLsizei levels , GLenum internal_format, GLsizei width)
  {
    glTextureStorage1D(id_, levels, internal_format, width);
  }
  void set_storage            (GLsizei levels , GLenum internal_format, GLsizei width, GLsizei height)
  {
    glTextureStorage2D(id_, levels, internal_format, width, height);
  }
  void set_storage            (GLsizei levels , GLenum internal_format, GLsizei width, GLsizei height, GLsizei depth)
  {
    glTextureStorage3D(id_, levels, internal_format, width, height, depth);
  }
  void set_storage_multisample(GLsizei samples, GLenum internal_format, GLsizei width, GLsizei height,                bool fixed_sample_locations = true)
  {
    glTextureStorage2DMultisample(id_, samples, internal_format, width, height,        fixed_sample_locations);
  }
  void set_storage_multisample(GLsizei samples, GLenum internal_format, GLsizei width, GLsizei height, GLsizei depth, bool fixed_sample_locations = true)
  {
    glTextureStorage3DMultisample(id_, samples, internal_format, width, height, depth, fixed_sample_locations);
  }

  // 8.20 Invalidate texture image data.
  void invalidate_sub_image(GLint level, GLint x, GLint y, GLint z, GLsizei width, GLsizei height = 1, GLsizei depth = 1)
  {
    glInvalidateTexSubImage(id_, level, x, y, z, width, height, depth);
  }
  void invalidate          (GLint level = 0)
  {
    glInvalidateTexImage(id_, level);
  }

  // 8.21 Clear texture image data.
  void clear_sub_image     (GLint level, GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* data = nullptr)
  {
    glClearTexSubImage(id_, level, x, y, z, width, height, depth, format, type, data);
  }
  void clear               (GLint level,                                                                          GLenum format, GLenum type, const void* data = nullptr)
  {
    glClearTexImage(id_, level, format, type, data);
  }

  // 8.26 Texture image loads / stores.
  void bind_image_texture  (GLuint unit, GLint level, bool layered, GLint layer, GLenum access, GLenum format)
  {
    glBindImageTexture(unit, id_, level, layered, layer, access, format);
  }

  // 18.3 Copying pixels.
  template<GLenum source_target>
  void copy_image_sub_data(const texture<source_target>& source, 
    GLint source_level, GLint source_x, GLint source_y, GLint source_z,
    GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth)
  {
    glCopyImageSubData(source.id(), source_target, source_level, source_x, source_y, source_z, id_, target, level, x, y, z, width, height, depth);
  }
  void copy_image_sub_data(const renderbuffer& source, 
    GLint source_level, GLint source_x, GLint source_y, GLint source_z,
    GLint level       , GLint x       , GLint y       , GLint z       , GLint width, GLint height, GLint depth)
  {
    glCopyImageSubData(source.id(), GL_RENDERBUFFER, source_level, source_x, source_y, source_z, id_, target, level, x, y, z, width, height, depth);
  }
  
  // 22.3 Internal format queries.
  static GLint   internal_format_info   (GLenum internal_format, GLenum parameter)
  {
    GLint result;
    glGetInternalformativ(target, internal_format, parameter, 1, &result);
    return result;
  }
  static GLint64 internal_format_info_64(GLenum internal_format, GLenum parameter)
  {
    GLint64 result;
    glGetInternalformati64v(target, internal_format, parameter, 1, &result);
    return result;
  }

  static const GLenum native_type = GL_TEXTURE;

  GLuint id() const
  {
    return id_;
  }
  
#ifdef GL_CUDA_INTEROP_SUPPORT
  void cuda_register  (cudaGraphicsMapFlags flags = cudaGraphicsMapFlagsNone)
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
    type*  buffer_ptr;
    size_t buffer_size;
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
  std::array<GLint, count>   get_int_parameter        (GLenum parameter) const
  {
    std::array<GLint, count> result;
    glGetTextureParameteriv(id_, parameter, result.data());
    return result;
  }
  GLint                      get_int_parameter        (GLenum parameter) const
  {
    GLint result;
    glGetTextureParameteriv(id_, parameter, &result);
    return result;
  }
  template<std::size_t count>
  std::array<GLfloat, count> get_float_parameter      (GLenum parameter) const
  {
    std::array<GLfloat, count> result;
    glGetTextureParameterfv(id_, parameter, result.data());
    return result;
  }
  GLfloat                    get_float_parameter      (GLenum parameter) const
  {
    GLfloat result;
    glGetTextureParameterfv(id_, parameter, &result);
    return result;
  }
  template<std::size_t count>
  std::array<GLint, count>   get_int_level_parameter  (GLuint level, GLenum parameter) const
  {
    std::array<GLint, count> result;
    glGetTextureLevelParameteriv(id_, level, parameter, result.data());
    return result;
  }
  GLint                      get_int_level_parameter  (GLuint level, GLenum parameter) const
  {
    GLint result;
    glGetTextureLevelParameteriv(id_, level, parameter, &result);
    return result;
  }
  template<std::size_t count>
  std::array<GLfloat, count> get_float_level_parameter(GLuint level, GLenum parameter) const
  {
    std::array<GLfloat, count> result;
    glGetTextureLevelParameterfv(id_, level, parameter, result.data());
    return result;
  }
  GLfloat                    get_float_level_parameter(GLuint level, GLenum parameter) const
  {
    GLfloat result;
    glGetTextureLevelParameterfv(id_, level, parameter, &result);
    return result;
  }
  
  GLuint id_      = 0;
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

GL_EXPORT inline void set_seamless_cubemap_enabled(bool enabled)
{
  enabled ? glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS) : glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}
GL_EXPORT inline bool seamless_cubemap_enabled    ()
{
  return glIsEnabled(GL_TEXTURE_CUBE_MAP_SEAMLESS) != 0;
}
}

#endif
