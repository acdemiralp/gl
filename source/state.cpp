//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/state.hpp>

namespace gl
{
// 8.4.1 Pixel storage modes.
void set_pack_swap_bytes                (bool   value)
{
  glPixelStorei(GL_PACK_SWAP_BYTES, value);
}
void set_pack_lsb_first                 (bool   value)
{
  glPixelStorei(GL_PACK_LSB_FIRST, value);
}
void set_pack_row_length                (GLint  value)
{
  glPixelStorei(GL_PACK_ROW_LENGTH, value);
}
void set_pack_image_height              (GLint  value)
{
  glPixelStorei(GL_PACK_IMAGE_HEIGHT, value);
}
void set_pack_skip_images               (GLint  value)
{
  glPixelStorei(GL_PACK_SKIP_IMAGES, value);
}
void set_pack_skip_pixels               (GLint  value)
{
  glPixelStorei(GL_PACK_SKIP_PIXELS, value);
}
void set_pack_skip_rows                 (GLint  value)
{
  glPixelStorei(GL_PACK_SKIP_ROWS, value);
}
void set_pack_alignment                 (GLint  value)
{
  glPixelStorei(GL_PACK_ALIGNMENT, value);
}
void set_pack_compressed_block_width    (GLint  value)
{
  glPixelStorei(GL_PACK_COMPRESSED_BLOCK_WIDTH, value);
}
void set_pack_compressed_block_height   (GLint  value)
{
  glPixelStorei(GL_PACK_COMPRESSED_BLOCK_HEIGHT, value);
}
void set_pack_compressed_block_depth    (GLint  value)
{
  glPixelStorei(GL_PACK_COMPRESSED_BLOCK_DEPTH, value);
}
void set_pack_compressed_block_size     (GLint  value)
{
  glPixelStorei(GL_PACK_COMPRESSED_BLOCK_SIZE, value);
}
void set_unpack_swap_bytes              (bool   value)
{
  glPixelStorei(GL_UNPACK_SWAP_BYTES, value);
}
void set_unpack_lsb_first               (bool   value)
{
  glPixelStorei(GL_UNPACK_LSB_FIRST, value);
}
void set_unpack_row_length              (GLint  value)
{
  glPixelStorei(GL_UNPACK_ROW_LENGTH, value);
}
void set_unpack_image_height            (GLint  value)
{
  glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, value);
}
void set_unpack_skip_images             (GLint  value)
{
  glPixelStorei(GL_UNPACK_SKIP_IMAGES, value);
}
void set_unpack_skip_pixels             (GLint  value)
{
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, value);
}
void set_unpack_skip_rows               (GLint  value)
{
  glPixelStorei(GL_UNPACK_SKIP_ROWS, value);
}
void set_unpack_alignment               (GLint  value)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, value);
}
void set_unpack_compressed_block_width  (GLint  value)
{
  glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_WIDTH, value);
}
void set_unpack_compressed_block_height (GLint  value)
{
  glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_HEIGHT, value);
}
void set_unpack_compressed_block_depth  (GLint  value)
{
  glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_DEPTH, value);
}
void set_unpack_compressed_block_size   (GLint  value)
{
  glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_SIZE, value);
}

// 21.5 Hints.
void set_fragment_shader_derivative_hint(GLenum hint)
{
  glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, hint);
}
void set_texture_compression_hint       (GLenum hint)
{
  glHint(GL_TEXTURE_COMPRESSION_HINT, hint);
}
void set_line_smooth_hint               (GLenum hint)
{
  glHint(GL_LINE_SMOOTH_HINT, hint);
}
void set_polygon_smooth_hint            (GLenum hint)
{
  glHint(GL_POLYGON_SMOOTH_HINT, hint);
}

// 22.1 Simple queries.

// 22.2 String queries.
std::string renderer()
{
  return std::string(reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
}
std::string vendor  ()
{
  return std::string(reinterpret_cast<char const*>(glGetString(GL_VENDOR)));
}
std::string version ()
{
  return std::string(reinterpret_cast<char const*>(glGetString(GL_VERSION)));
}

std::vector<std::string> extensions               ()
{
  GLint count;
  glGetIntegerv(GL_NUM_EXTENSIONS, &count);

  std::vector<std::string> result;
  for(auto i = 0; i < count; i++)
    result.emplace_back(reinterpret_cast<char const*>(glGetStringi(GL_EXTENSIONS, i)));
  return result;
}
std::vector<std::string> shading_language_versions()
{
  GLint count;
  glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &count);

  std::vector<std::string> result;
  for(auto i = 0; i < count; i++)
    result.emplace_back(reinterpret_cast<char const*>(glGetStringi(GL_SHADING_LANGUAGE_VERSION, i)));
  return result;
}

// 22.3 Internal format queries.

}