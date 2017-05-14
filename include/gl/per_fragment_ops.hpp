//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_PER_FRAGMENT_OPS_HPP_
#define GL_PER_FRAGMENT_OPS_HPP_

#include <array>
#include <vector>

#include <gl/opengl.hpp>

#include <export.hpp>

namespace gl
{
// 17.3.2 Scissor test.
GL_EXPORT void set_scissor_test_enabled(bool enabled);
GL_EXPORT bool scissor_test_enabled    ();
GL_EXPORT void set_scissor             (const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size);

GL_EXPORT void set_indexed_scissor_test_enabled(GLuint index, bool enabled);
GL_EXPORT bool indexed_scissor_test_enabled    (GLuint index);
GL_EXPORT void set_indexed_scissor             (GLuint index, const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size);

// 17.3.3 Multisample fragment operations.
GL_EXPORT void set_sample_alpha_to_coverage_enabled(bool enabled);
GL_EXPORT bool sample_alpha_to_coverage_enabled    ();
GL_EXPORT void set_sample_alpha_to_one_enabled     (bool enabled);
GL_EXPORT bool sample_alpha_to_one_enabled         ();
GL_EXPORT void set_sample_coverage_enabled         (bool enabled);
GL_EXPORT bool sample_coverage_enabled             ();
GL_EXPORT void set_sample_mask_enabled             (bool enabled);
GL_EXPORT bool sample_mask_enabled                 ();
GL_EXPORT void set_sample_coverage                 (GLfloat value, bool invert = false);
GL_EXPORT void set_sample_mask                     (GLuint index, GLbitfield mask);

// 17.3.5 Stencil test.
GL_EXPORT void set_stencil_test_enabled(bool enabled);
GL_EXPORT bool stencil_test_enabled    ();
GL_EXPORT void set_stencil_function    (GLenum function, GLint reference_value, GLuint mask         , GLenum face = GL_FRONT_AND_BACK);
GL_EXPORT void set_stencil_operation   (GLenum stencil_fail, GLenum depth_fail, GLenum depth_success, GLenum face = GL_FRONT_AND_BACK);

// 17.3.6 Depth buffer test.
GL_EXPORT void set_depth_test_enabled(bool enabled);
GL_EXPORT bool depth_test_enabled    ();
GL_EXPORT void set_depth_function    (GLenum function = GL_LESS);

// 17.3.8 Blending.
GL_EXPORT void set_blending_enabled(bool enabled);
GL_EXPORT bool blending_enabled    ();
GL_EXPORT void set_blend_equation  (GLenum mode       = GL_FUNC_ADD);
GL_EXPORT void set_blend_equation  (GLenum rgb_mode   = GL_FUNC_ADD, GLenum alpha_mode   = GL_FUNC_ADD);
GL_EXPORT void set_blend_function  (GLenum source     = GL_ONE     , GLenum destination  = GL_ZERO    );
GL_EXPORT void set_blend_function  (GLenum source_rgb = GL_ONE     , GLenum source_alpha = GL_ONE     , GLenum destination_rgb = GL_ZERO, GLenum destination_alpha = GL_ZERO);
GL_EXPORT void set_blend_color     (const std::array<GLfloat, 4>& color);

GL_EXPORT void set_indexed_blending_enabled(GLuint index, bool enabled);
GL_EXPORT bool indexed_blending_enabled    (GLuint index);
GL_EXPORT void set_indexed_blend_equation  (GLuint index, GLenum mode       = GL_FUNC_ADD);
GL_EXPORT void set_indexed_blend_equation  (GLuint index, GLenum rgb_mode   = GL_FUNC_ADD, GLenum alpha_mode   = GL_FUNC_ADD);
GL_EXPORT void set_indexed_blend_function  (GLuint index, GLenum source     = GL_ONE     , GLenum destination  = GL_ZERO);
GL_EXPORT void set_indexed_blend_function  (GLuint index, GLenum source_rgb = GL_ONE     , GLenum source_alpha = GL_ONE, GLenum destination_rgb = GL_ZERO, GLenum destination_alpha = GL_ZERO);

// 17.3.10 Dithering.
GL_EXPORT void set_dithering_enabled(bool enabled);
GL_EXPORT bool dithering_enabled    ();

// 17.3.11 Logical operation.
GL_EXPORT void set_logic_operation_enabled(bool enabled);
GL_EXPORT bool logic_operation_enabled    ();
GL_EXPORT void set_logic_operation        (GLenum operation = GL_COPY);
}

#endif