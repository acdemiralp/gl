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
void set_scissor_test_enabled(bool enabled)
{
  enabled ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
}
bool scissor_test_enabled    ()
{
  return glIsEnabled(GL_SCISSOR_TEST) != 0;
}
void set_scissor             (const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size)
{
  glScissor(offset[0], offset[1], size[0], size[1]);
}

void set_indexed_scissor_test_enabled(GLuint index, bool enabled)
{
  enabled ? glEnablei(GL_SCISSOR_TEST, index) : glDisablei(GL_SCISSOR_TEST, index);
}
bool indexed_scissor_test_enabled    (GLuint index)
{
  return glIsEnabledi(GL_SCISSOR_TEST, index) != 0;
}
void set_indexed_scissor             (GLuint index, const std::array<GLint, 2>& offset, const std::array<GLsizei, 2>& size)
{
  glScissorIndexed(index, offset[0], offset[1], size[0], size[1]);
}

// 17.3.3 Multisample fragment operations.
void set_sample_alpha_to_coverage_enabled(bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE) : glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}
bool sample_alpha_to_coverage_enabled    ()
{
  return glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE) != 0;
}
void set_sample_alpha_to_one_enabled     (bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_ALPHA_TO_ONE) : glDisable(GL_SAMPLE_ALPHA_TO_ONE);
}
bool sample_alpha_to_one_enabled         ()
{
  return glIsEnabled(GL_SAMPLE_ALPHA_TO_ONE) != 0;
}
void set_sample_coverage_enabled         (bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_COVERAGE) : glDisable(GL_SAMPLE_COVERAGE);
}
bool sample_coverage_enabled             ()
{
  return glIsEnabled(GL_SAMPLE_COVERAGE) != 0;
}
void set_sample_mask_enabled             (bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_MASK) : glDisable(GL_SAMPLE_MASK);
}
bool sample_mask_enabled                 ()
{
  return glIsEnabled(GL_SAMPLE_MASK) != 0;
}
void set_sample_coverage                 (GLfloat value, bool invert = false)
{
  glSampleCoverage(value, invert);
}
void set_sample_mask                     (GLuint index, GLbitfield mask)
{
  glSampleMaski(index, mask);
}

// 17.3.5 Stencil test.
void set_stencil_test_enabled(bool enabled)
{
  enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}
bool stencil_test_enabled    ()
{
  return glIsEnabled(GL_STENCIL_TEST) != 0;
}
void set_stencil_function    (GLenum function, GLint reference_value, GLuint mask         , GLenum face = GL_FRONT_AND_BACK)
{
  glStencilFuncSeparate(face, function, reference_value, mask);
}
void set_stencil_operation   (GLenum stencil_fail, GLenum depth_fail, GLenum depth_success, GLenum face = GL_FRONT_AND_BACK)
{
  glStencilOpSeparate(face, stencil_fail, depth_fail, depth_success);
}

// 17.3.6 Depth buffer test.
void set_depth_test_enabled(bool enabled)
{
  enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}
bool depth_test_enabled    ()
{
  return glIsEnabled(GL_DEPTH_TEST) != 0;
}
void set_depth_function    (GLenum function = GL_LESS)
{
  glDepthFunc(function);
}

// 17.3.7 SRGB conversion.
void set_framebuffer_srgb_enabled(bool enabled)
{
  enabled ? glEnable(GL_FRAMEBUFFER_SRGB) : glDisable(GL_FRAMEBUFFER_SRGB);
}
bool framebuffer_srgb_enabled    ()
{
  return glIsEnabled(GL_FRAMEBUFFER_SRGB) != 0;
}

// 17.3.8 Blending.
void set_blending_enabled(bool enabled)
{
  enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}
bool blending_enabled    ()
{
  return glIsEnabled(GL_BLEND) != 0;
}
void set_blend_equation  (GLenum mode       = GL_FUNC_ADD)
{
  glBlendEquation(mode);
}
void set_blend_equation  (GLenum rgb_mode   = GL_FUNC_ADD, GLenum alpha_mode   = GL_FUNC_ADD)
{
  glBlendEquationSeparate(rgb_mode, alpha_mode);
}
void set_blend_function  (GLenum source     = GL_ONE     , GLenum destination  = GL_ZERO    )
{
  glBlendFunc(source, destination);
}
void set_blend_function  (GLenum source_rgb = GL_ONE     , GLenum source_alpha = GL_ONE     , GLenum destination_rgb = GL_ZERO, GLenum destination_alpha = GL_ZERO)
{
  glBlendFuncSeparate(source_rgb, destination_rgb, source_alpha, destination_alpha);
}
void set_blend_color     (const std::array<GLfloat, 4>& color)
{
  glBlendColor(color[0], color[1], color[2], color[3]);
}

void set_indexed_blending_enabled(GLuint index, bool enabled)
{
  enabled ? glEnablei(GL_BLEND, index) : glDisablei(GL_BLEND, index);
}
bool indexed_blending_enabled    (GLuint index)
{
  return glIsEnabledi(GL_BLEND, index) != 0;
}
void set_indexed_blend_equation  (GLuint index, GLenum mode       = GL_FUNC_ADD)
{
  glBlendEquationi(index, mode);
}
void set_indexed_blend_equation  (GLuint index, GLenum rgb_mode   = GL_FUNC_ADD, GLenum alpha_mode   = GL_FUNC_ADD)
{
  glBlendEquationSeparatei(index, rgb_mode, alpha_mode);
}
void set_indexed_blend_function  (GLuint index, GLenum source     = GL_ONE     , GLenum destination  = GL_ZERO)
{
  glBlendFunci(index, source, destination);
}
void set_indexed_blend_function  (GLuint index, GLenum source_rgb = GL_ONE     , GLenum source_alpha = GL_ONE, GLenum destination_rgb = GL_ZERO, GLenum destination_alpha = GL_ZERO)
{
  glBlendFuncSeparatei(index, source_rgb, destination_rgb, source_alpha, destination_alpha);
}

// 17.3.10 Dithering.
void set_dithering_enabled(bool enabled)
{
  enabled ? glEnable(GL_DITHER) : glDisable(GL_DITHER);
}
bool dithering_enabled    ()
{
  return glIsEnabled(GL_DITHER) != 0;
}

// 17.3.11 Logical operation.
void set_logic_operation_enabled(bool enabled)
{
  enabled ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);
}
bool logic_operation_enabled    ()
{
  return glIsEnabled(GL_COLOR_LOGIC_OP) != 0;
}
void set_logic_operation        (GLenum operation = GL_COPY)
{
  glLogicOp(operation);
}
}

#endif