//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_RASTERIZATION_HPP
#define GL_RASTERIZATION_HPP

#include <array>

#include <gl/opengl.hpp>

namespace gl
{
// 14.0 Rasterization.
inline void set_rasterizer_discard_enabled(const bool enabled)
{
  enabled ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);
}
inline bool rasterizer_discard_enabled    ()
{
  return glIsEnabled(GL_RASTERIZER_DISCARD) != 0;
}

// 14.3.1 Multisampling.
inline void set_multisampling_enabled (const bool enabled)
{
  enabled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
}
inline bool multisampling_enabled     ()
{
  return glIsEnabled(GL_MULTISAMPLE) != 0;
}
inline void set_sample_shading_enabled(const bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_SHADING) : glDisable(GL_SAMPLE_SHADING);
}
inline bool sample_shading_enabled    ()
{
  return glIsEnabled(GL_SAMPLE_SHADING) != 0;
}

inline void set_minimum_sample_shading(const GLfloat value = 1.0f)
{
  glMinSampleShading(value);
}

inline std::array<GLfloat, 2> multisample_sample_position(const GLuint index)
{
  std::array<GLfloat, 2> position {};
  glGetMultisamplefv(GL_SAMPLE_POSITION, index, position.data());
  return position;
}

// 14.4 Points.
inline void set_point_size_enabled(const bool enabled)
{
  enabled ? glEnable(GL_PROGRAM_POINT_SIZE) : glDisable(GL_PROGRAM_POINT_SIZE);
}
inline bool point_size_enabled    ()
{
  return glIsEnabled(GL_PROGRAM_POINT_SIZE) != 0;
}

inline void set_point_size               (const GLfloat size   = 1.0f)
{
  glPointSize(size);
}
inline void set_point_fade_threshold_size(const GLfloat size   = 1.0f)
{
  glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, size);
}
inline void set_point_sprite_coord_origin(const GLenum  origin = GL_UPPER_LEFT)
{
  glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, origin);
}

// 14.5 Line segments.
inline void set_line_smoothing_enabled(const bool enabled)
{
  enabled ? glEnable(GL_LINE_SMOOTH) : glDisable(GL_LINE_SMOOTH);
}
inline bool line_smoothing_enabled    ()
{
  return glIsEnabled(GL_LINE_SMOOTH) != 0;
}

inline void set_line_width(const GLfloat width)
{
  glLineWidth(width);
}

// 14.6 Polygons.
inline void set_polygon_smoothing_enabled   (const bool enabled)
{
  enabled ? glEnable(GL_POLYGON_SMOOTH) : glDisable(GL_POLYGON_SMOOTH);
}
inline bool polygon_smoothing_enabled       ()
{
  return glIsEnabled(GL_POLYGON_SMOOTH) != 0;
}
inline void set_polygon_face_culling_enabled(const bool enabled)
{
  enabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}
inline bool polygon_face_culling_enabled    ()
{
  return glIsEnabled(GL_CULL_FACE) != 0;
}

inline void set_front_face(const GLenum mode = GL_CCW )
{
  glFrontFace(mode);
}
inline void set_cull_face (const GLenum mode = GL_BACK)
{
  glCullFace(mode);
}

// 14.6.4 Polygon rasterization and depth offset.
inline void set_polygon_mode        (const GLenum mode = GL_FILL)
{
  glPolygonMode(GL_FRONT_AND_BACK, mode);
}
inline void set_polygon_offset_clamp(const GLfloat factor, const GLfloat units, const GLfloat clamp = 0.0f)
{
  glPolygonOffsetClamp(factor, units, clamp);
}

inline void set_polygon_point_offset_enabled(const bool enabled)
{
  enabled ? glEnable(GL_POLYGON_OFFSET_POINT) : glDisable(GL_POLYGON_OFFSET_POINT);
}
inline bool polygon_point_offset_enabled    ()
{
  return glIsEnabled(GL_POLYGON_OFFSET_POINT) != 0;
}
inline void set_polygon_line_offset_enabled (const bool enabled)
{
  enabled ? glEnable(GL_POLYGON_OFFSET_LINE) : glDisable(GL_POLYGON_OFFSET_LINE);
}
inline bool polygon_line_offset_enabled     ()
{
  return glIsEnabled(GL_POLYGON_OFFSET_LINE) != 0;
}
inline void set_polygon_fill_offset_enabled (const bool enabled)
{
  enabled ? glEnable(GL_POLYGON_OFFSET_FILL) : glDisable(GL_POLYGON_OFFSET_FILL);
}
inline bool polygon_fill_offset_enabled     ()
{
  return glIsEnabled(GL_POLYGON_OFFSET_FILL) != 0;
}
}

#endif
