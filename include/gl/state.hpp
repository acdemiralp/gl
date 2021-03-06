//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_STATE_HPP
#define GL_STATE_HPP

#include <string>
#include <vector>

#include <gl/opengl.hpp>

namespace gl
{
// 8.4.1 Pixel storage modes.
inline void set_pack_swap_bytes                (const bool   value = false)
{
  glPixelStorei(GL_PACK_SWAP_BYTES, value);
}
inline void set_pack_lsb_first                 (const bool   value = false)
{
  glPixelStorei(GL_PACK_LSB_FIRST, value);
}
inline void set_pack_row_length                (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_ROW_LENGTH, value);
}
inline void set_pack_image_height              (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_IMAGE_HEIGHT, value);
}
inline void set_pack_skip_images               (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_SKIP_IMAGES, value);
}
inline void set_pack_skip_pixels               (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_SKIP_PIXELS, value);
}
inline void set_pack_skip_rows                 (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_SKIP_ROWS, value);
}
inline void set_pack_alignment                 (const GLint  value = 4)
{
  glPixelStorei(GL_PACK_ALIGNMENT, value);
}
inline void set_pack_compressed_block_width    (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_COMPRESSED_BLOCK_WIDTH, value);
}
inline void set_pack_compressed_block_height   (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_COMPRESSED_BLOCK_HEIGHT, value);
}
inline void set_pack_compressed_block_depth    (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_COMPRESSED_BLOCK_DEPTH, value);
}
inline void set_pack_compressed_block_size     (const GLint  value = 0)
{
  glPixelStorei(GL_PACK_COMPRESSED_BLOCK_SIZE, value);
}
inline void set_unpack_swap_bytes              (const bool   value = false)
{
  glPixelStorei(GL_UNPACK_SWAP_BYTES, value);
}
inline void set_unpack_lsb_first               (const bool   value = false)
{
  glPixelStorei(GL_UNPACK_LSB_FIRST, value);
}
inline void set_unpack_row_length              (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_ROW_LENGTH, value);
}
inline void set_unpack_image_height            (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, value);
}
inline void set_unpack_skip_images             (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_SKIP_IMAGES, value);
}
inline void set_unpack_skip_pixels             (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, value);
}
inline void set_unpack_skip_rows               (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_SKIP_ROWS, value);
}
inline void set_unpack_alignment               (const GLint  value = 4)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, value);
}
inline void set_unpack_compressed_block_width  (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_WIDTH, value);
}
inline void set_unpack_compressed_block_height (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_HEIGHT, value);
}
inline void set_unpack_compressed_block_depth  (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_DEPTH, value);
}
inline void set_unpack_compressed_block_size   (const GLint  value = 0)
{
  glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_SIZE, value);
}

// 21.5 Hints.
inline void set_fragment_shader_derivative_hint(const GLenum hint = GL_NICEST)
{
  glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, hint);
}
inline void set_texture_compression_hint       (const GLenum hint = GL_NICEST)
{
  glHint(GL_TEXTURE_COMPRESSION_HINT, hint);
}
inline void set_line_smooth_hint               (const GLenum hint = GL_NICEST)
{
  glHint(GL_LINE_SMOOTH_HINT, hint);
}
inline void set_polygon_smooth_hint            (const GLenum hint = GL_NICEST)
{
  glHint(GL_POLYGON_SMOOTH_HINT, hint);
}

// 22.2 String queries.
inline std::string renderer()
{
  return std::string(reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
}
inline std::string vendor  ()
{
  return std::string(reinterpret_cast<char const*>(glGetString(GL_VENDOR)));
}
inline std::string version ()
{
  return std::string(reinterpret_cast<char const*>(glGetString(GL_VERSION)));
}

inline std::vector<std::string> extensions               ()
{
  GLint count;
  glGetIntegerv(GL_NUM_EXTENSIONS, &count);

  std::vector<std::string> result;
  for (auto i = 0; i < count; i++)
    result.emplace_back(reinterpret_cast<char const*>(glGetStringi(GL_EXTENSIONS, i)));
  return result;
}
inline std::vector<std::string> shading_language_versions()
{
  GLint count;
  glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &count);

  std::vector<std::string> result;
  for (auto i = 0; i < count; i++)
    result.emplace_back(reinterpret_cast<char const*>(glGetStringi(GL_SHADING_LANGUAGE_VERSION, i)));
  return result;
}
inline std::vector<std::string> spir_v_extensions        ()
{
  GLint count;
  glGetIntegerv(GL_NUM_SPIR_V_EXTENSIONS, &count);

  std::vector<std::string> result;
  for (auto i = 0; i < count; i++)
    result.emplace_back(reinterpret_cast<char const*>(glGetStringi(GL_SPIR_V_EXTENSIONS, i)));
  return result;
}
}

#endif