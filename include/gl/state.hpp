//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_STATE_HPP_
#define GL_STATE_HPP_

#include <string>
#include <vector>

#include <gl/opengl.hpp>

namespace gl
{
// 8.4.1 Pixel storage modes.
void set_pack_swap_bytes                (bool   value = false);
void set_pack_lsb_first                 (bool   value = false);
void set_pack_row_length                (GLint  value = 0);
void set_pack_image_height              (GLint  value = 0);
void set_pack_skip_images               (GLint  value = 0);
void set_pack_skip_pixels               (GLint  value = 0);
void set_pack_skip_rows                 (GLint  value = 0);
void set_pack_alignment                 (GLint  value = 4);
void set_pack_compressed_block_width    (GLint  value = 0);
void set_pack_compressed_block_height   (GLint  value = 0);
void set_pack_compressed_block_depth    (GLint  value = 0);
void set_pack_compressed_block_size     (GLint  value = 0);
void set_unpack_swap_bytes              (bool   value = false);
void set_unpack_lsb_first               (bool   value = false);
void set_unpack_row_length              (GLint  value = 0);
void set_unpack_image_height            (GLint  value = 0);
void set_unpack_skip_images             (GLint  value = 0);
void set_unpack_skip_pixels             (GLint  value = 0);
void set_unpack_skip_rows               (GLint  value = 0);
void set_unpack_alignment               (GLint  value = 4);
void set_unpack_compressed_block_width  (GLint  value = 0);
void set_unpack_compressed_block_height (GLint  value = 0);
void set_unpack_compressed_block_depth  (GLint  value = 0);
void set_unpack_compressed_block_size   (GLint  value = 0);

// 21.5 Hints.
void set_fragment_shader_derivative_hint(GLenum hint = GL_NICEST);
void set_texture_compression_hint       (GLenum hint = GL_NICEST);
void set_line_smooth_hint               (GLenum hint = GL_NICEST);
void set_polygon_smooth_hint            (GLenum hint = GL_NICEST);

// 22.2 String queries.
std::string renderer();
std::string vendor  ();
std::string version ();

std::vector<std::string> extensions               ();
std::vector<std::string> shading_language_versions();
std::vector<std::string> spir_v_extensions        ();
}

#include <gl/implementation/state.ipp>

#endif