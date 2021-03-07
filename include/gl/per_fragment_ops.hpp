//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_PER_FRAGMENT_OPS_HPP
#define GL_PER_FRAGMENT_OPS_HPP

#include <array>

#include <gl/opengl.hpp>

namespace gl
{
// 17.3.2 Scissor test.
inline void set_scissor_test_enabled(const bool enabled)
{
  enabled ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
}
inline bool scissor_test_enabled    ()
{
  return glIsEnabled(GL_SCISSOR_TEST) != 0;
}
inline void set_scissor             (const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size)
{
  glScissor(offset[0], offset[1], size[0], size[1]);
}

inline void set_indexed_scissor_test_enabled(const GLuint index, const bool enabled)
{
  enabled ? glEnablei(GL_SCISSOR_TEST, index) : glDisablei(GL_SCISSOR_TEST, index);
}
inline bool indexed_scissor_test_enabled    (const GLuint index)
{
  return glIsEnabledi(GL_SCISSOR_TEST, index) != 0;
}
inline void set_indexed_scissor             (const GLuint index, const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size)
{
  glScissorIndexed(index, offset[0], offset[1], size[0], size[1]);
}

// 17.3.3 Multisample fragment operations.
inline void set_sample_alpha_to_coverage_enabled(const bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE) : glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}
inline bool sample_alpha_to_coverage_enabled    ()
{
  return glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE) != 0;
}
inline void set_sample_alpha_to_one_enabled     (const bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_ALPHA_TO_ONE) : glDisable(GL_SAMPLE_ALPHA_TO_ONE);
}
inline bool sample_alpha_to_one_enabled         ()
{
  return glIsEnabled(GL_SAMPLE_ALPHA_TO_ONE) != 0;
}
inline void set_sample_coverage_enabled         (const bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_COVERAGE) : glDisable(GL_SAMPLE_COVERAGE);
}
inline bool sample_coverage_enabled             ()
{
  return glIsEnabled(GL_SAMPLE_COVERAGE) != 0;
}
inline void set_sample_mask_enabled             (const bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_MASK) : glDisable(GL_SAMPLE_MASK);
}
inline bool sample_mask_enabled                 ()
{
  return glIsEnabled(GL_SAMPLE_MASK) != 0;
}
inline void set_sample_coverage                 (const GLfloat value, const bool invert = false)
{
  glSampleCoverage(value, invert);
}
inline void set_sample_mask                     (const GLuint index, const GLbitfield mask)
{
  glSampleMaski(index, mask);
}

// 17.3.5 Stencil test.
inline void set_stencil_test_enabled(const bool enabled)
{
  enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}
inline bool stencil_test_enabled    ()
{
  return glIsEnabled(GL_STENCIL_TEST) != 0;
}
inline void set_stencil_function    (const GLenum function, const GLint reference_value, const GLuint mask         , const GLenum face = GL_FRONT_AND_BACK)
{
  glStencilFuncSeparate(face, function, reference_value, mask);
}
inline void set_stencil_operation   (const GLenum stencil_fail, const GLenum depth_fail, const GLenum depth_success, const GLenum face = GL_FRONT_AND_BACK)
{
  glStencilOpSeparate(face, stencil_fail, depth_fail, depth_success);
}

// 17.3.6 Depth buffer test.
inline void set_depth_test_enabled(const bool enabled)
{
  enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}
inline bool depth_test_enabled    ()
{
  return glIsEnabled(GL_DEPTH_TEST) != 0;
}
inline void set_depth_function    (const GLenum function = GL_LESS)
{
  glDepthFunc(function);
}

// 17.3.7 SRGB conversion.
inline void set_framebuffer_srgb_enabled(const bool enabled)
{
  enabled ? glEnable(GL_FRAMEBUFFER_SRGB) : glDisable(GL_FRAMEBUFFER_SRGB);
}
inline bool framebuffer_srgb_enabled    ()
{
  return glIsEnabled(GL_FRAMEBUFFER_SRGB) != 0;
}

// 17.3.8 Blending.
inline void set_blending_enabled(const bool enabled)
{
  enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}
inline bool blending_enabled    ()
{
  return glIsEnabled(GL_BLEND) != 0;
}
inline void set_blend_equation  (const GLenum mode       = GL_FUNC_ADD)
{
  glBlendEquation(mode);
}
inline void set_blend_equation  (const GLenum rgb_mode   = GL_FUNC_ADD, const GLenum alpha_mode   = GL_FUNC_ADD)
{
  glBlendEquationSeparate(rgb_mode, alpha_mode);
}
inline void set_blend_function  (const GLenum source     = GL_ONE     , const GLenum destination  = GL_ZERO    )
{
  glBlendFunc(source, destination);
}
inline void set_blend_function  (const GLenum source_rgb = GL_ONE     , const GLenum source_alpha = GL_ONE     , const GLenum destination_rgb = GL_ZERO, const GLenum destination_alpha = GL_ZERO)
{
  glBlendFuncSeparate(source_rgb, destination_rgb, source_alpha, destination_alpha);
}
inline void set_blend_color     (const std::array<GLfloat, 4>& color)
{
  glBlendColor(color[0], color[1], color[2], color[3]);
}

inline void set_indexed_blending_enabled(const GLuint index, const bool enabled)
{
  enabled ? glEnablei(GL_BLEND, index) : glDisablei(GL_BLEND, index);
}
inline bool indexed_blending_enabled    (const GLuint index)
{
  return glIsEnabledi(GL_BLEND, index) != 0;
}
inline void set_indexed_blend_equation  (const GLuint index, const GLenum mode       = GL_FUNC_ADD)
{
  glBlendEquationi(index, mode);
}
inline void set_indexed_blend_equation  (const GLuint index, const GLenum rgb_mode   = GL_FUNC_ADD, const GLenum alpha_mode   = GL_FUNC_ADD)
{
  glBlendEquationSeparatei(index, rgb_mode, alpha_mode);
}
inline void set_indexed_blend_function  (const GLuint index, const GLenum source     = GL_ONE     , const GLenum destination  = GL_ZERO)
{
  glBlendFunci(index, source, destination);
}
inline void set_indexed_blend_function  (const GLuint index, const GLenum source_rgb = GL_ONE     , const GLenum source_alpha = GL_ONE, const GLenum destination_rgb = GL_ZERO, const GLenum destination_alpha = GL_ZERO)
{
  glBlendFuncSeparatei(index, source_rgb, destination_rgb, source_alpha, destination_alpha);
}

// 17.3.10 Dithering.
inline void set_dithering_enabled(const bool enabled)
{
  enabled ? glEnable(GL_DITHER) : glDisable(GL_DITHER);
}
inline bool dithering_enabled    ()
{
  return glIsEnabled(GL_DITHER) != 0;
}

// 17.3.11 Logical operation.
inline void set_logic_operation_enabled(const bool enabled)
{
  enabled ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);
}
inline bool logic_operation_enabled    ()
{
  return glIsEnabled(GL_COLOR_LOGIC_OP) != 0;
}
inline void set_logic_operation        (const GLenum operation = GL_COPY)
{
  glLogicOp(operation);
}
}

#endif