//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_VERTEX_POST_PROCESSING_OPS_HPP_
#define GL_VERTEX_POST_PROCESSING_OPS_HPP_

#include <gl/export.hpp>
#include <gl/opengl.hpp>

namespace gl
{
// 13.4 Flatshading.
GL_EXPORT void set_provoking_vertex(GLenum provoke_mode);

// 13.5 Primitive clipping.
GL_EXPORT void set_depth_clamp_enabled  (bool enabled);
GL_EXPORT bool depth_clamp_enabled      ();
GL_EXPORT void set_clip_distance_enabled(bool enabled , GLuint index = 0);
GL_EXPORT bool clip_distance_enabled    (               GLuint index = 0);
GL_EXPORT void set_clip_control         (GLenum origin, GLenum depth);
}

#endif