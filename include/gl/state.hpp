//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_STATE_HPP_
#define GL_STATE_HPP_

#include <gl/opengl.hpp>

#include <export.hpp>

namespace gl
{
// 8.4.1 Pixel storage modes.
GL_EXPORT void set_pack_swap_bytes               (bool   value = false);
GL_EXPORT void set_pack_lsb_first                (bool   value = false);
GL_EXPORT void set_pack_row_length               (GLint  value = 0);
GL_EXPORT void set_pack_image_height             (GLint  value = 0);
GL_EXPORT void set_pack_skip_images              (GLint  value = 0);
GL_EXPORT void set_pack_skip_pixels              (GLint  value = 0);
GL_EXPORT void set_pack_skip_rows                (GLint  value = 0);
GL_EXPORT void set_pack_alignment                (GLint  value = 4);
GL_EXPORT void set_pack_compressed_block_width   (GLint  value = 0);
GL_EXPORT void set_pack_compressed_block_height  (GLint  value = 0);
GL_EXPORT void set_pack_compressed_block_depth   (GLint  value = 0);
GL_EXPORT void set_pack_compressed_block_size    (GLint  value = 0);
GL_EXPORT void set_unpack_swap_bytes             (bool   value = false);
GL_EXPORT void set_unpack_lsb_first              (bool   value = false);
GL_EXPORT void set_unpack_row_length             (GLint  value = 0);
GL_EXPORT void set_unpack_image_height           (GLint  value = 0);
GL_EXPORT void set_unpack_skip_images            (GLint  value = 0);
GL_EXPORT void set_unpack_skip_pixels            (GLint  value = 0);
GL_EXPORT void set_unpack_skip_rows              (GLint  value = 0);
GL_EXPORT void set_unpack_alignment              (GLint  value = 4);
GL_EXPORT void set_unpack_compressed_block_width (GLint  value = 0);
GL_EXPORT void set_unpack_compressed_block_height(GLint  value = 0);
GL_EXPORT void set_unpack_compressed_block_depth (GLint  value = 0);
GL_EXPORT void set_unpack_compressed_block_size  (GLint  value = 0);

// 21.5 Hints.
GL_EXPORT void set_fragment_shader_derivative_hint(GLenum hint = GL_NICEST);
GL_EXPORT void set_texture_compression_hint       (GLenum hint = GL_NICEST);
GL_EXPORT void set_line_smooth_hint               (GLenum hint = GL_NICEST);
GL_EXPORT void set_polygon_smooth_hint            (GLenum hint = GL_NICEST);

// 22.1 Simple queries.

// 22.2 String queries.

// 22.3 Internal format queries.

// 22.4 Transform feedback queries.

}

#endif