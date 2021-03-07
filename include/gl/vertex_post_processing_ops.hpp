//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_VERTEX_POST_PROCESSING_OPS_HPP
#define GL_VERTEX_POST_PROCESSING_OPS_HPP

#include <gl/opengl.hpp>

namespace gl
{
// 13.4 Flatshading.
inline void set_provoking_vertex(const GLenum provoke_mode)
{
  glProvokingVertex(provoke_mode);
}

// 13.5 Primitive clipping.
inline void set_depth_clamp_enabled  (const bool enabled)
{
  enabled ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
}
inline bool depth_clamp_enabled      ()
{
  return glIsEnabled(GL_DEPTH_CLAMP) != 0;
}
inline void set_clip_distance_enabled(const bool enabled , const GLuint index = 0)
{
  enabled ? glEnable(GL_CLIP_DISTANCE0 + index) : glDisable(GL_CLIP_DISTANCE0 + index);
}
inline bool clip_distance_enabled    (                     const GLuint index = 0)
{
  return glIsEnabled(GL_CLIP_DISTANCE0 + index) != 0;
}
inline void set_clip_control         (const GLenum origin, const GLenum depth)
{
  glClipControl(origin, depth);
}
}

#endif