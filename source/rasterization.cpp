//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/rasterization.hpp>

namespace gl
{
// 14.0 Rasterization.
void set_rasterizer_discard_enabled(bool enabled)
{
  enabled ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);
}
bool rasterizer_discard_enabled    ()
{
  return glIsEnabled(GL_RASTERIZER_DISCARD) != 0;
}

// 14.3.1 Multisampling.
void set_multisampling_enabled (bool enabled)
{
  enabled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
}
bool multisampling_enabled     ()
{
  return glIsEnabled(GL_MULTISAMPLE) != 0;
}
void set_sample_shading_enabled(bool enabled)
{
  enabled ? glEnable(GL_SAMPLE_SHADING) : glDisable(GL_SAMPLE_SHADING);
}
bool sample_shading_enabled    ()
{
  return glIsEnabled(GL_SAMPLE_SHADING) != 0;
}

void set_minimum_sample_shading(GLfloat value)
{
  glMinSampleShading(value);
}

std::array<GLfloat, 2> multisample_sample_position(GLuint index)
{
  std::array<GLfloat, 2> position;
  glGetMultisamplefv(GL_SAMPLE_POSITION, index, position.data());
  return position;
}

// 14.4 Points.
void set_point_size_enabled(bool enabled)
{
  enabled ? glEnable(GL_PROGRAM_POINT_SIZE) : glDisable(GL_PROGRAM_POINT_SIZE);
}
bool point_size_enabled    ()
{
  return glIsEnabled(GL_PROGRAM_POINT_SIZE) != 0;
}

void set_point_size               (GLfloat size  )
{
  glPointSize(size);
}
void set_point_fade_threshold_size(GLfloat size  )
{
  glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, size);
}
void set_point_sprite_coord_origin(GLenum  origin)
{
  glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, origin);
}

// 14.5 Line segments.
void set_line_smoothing_enabled(bool enabled)
{
  enabled ? glEnable(GL_LINE_SMOOTH) : glDisable(GL_LINE_SMOOTH);
}
bool line_smoothing_enabled    ()
{
  return glIsEnabled(GL_LINE_SMOOTH) != 0;
}

void set_line_width(GLfloat width)
{
  glLineWidth(width);
}

// 14.6 Polygons.
void set_polygon_smoothing_enabled(bool enabled)
{
  enabled ? glEnable(GL_POLYGON_SMOOTH) : glDisable(GL_POLYGON_SMOOTH);
}
bool polygon_smoothing_enabled    ()
{
  return glIsEnabled(GL_POLYGON_SMOOTH) != 0;
}

void set_polygon_face_culling_enabled(bool enabled)
{
  enabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}
bool polygon_face_culling_enabled    ()
{
  return glIsEnabled(GL_CULL_FACE) != 0;
}

void set_front_face(GLenum mode)
{
  glFrontFace(mode);
}
void set_cull_face (GLenum mode)
{
  glCullFace(mode);
}

// 14.6.4 Polygon rasterization and depth offset.
void set_polygon_mode        (GLenum mode)
{
  glPolygonMode(GL_FRONT_AND_BACK, mode);
}
void set_polygon_offset_clamp(GLfloat factor, GLfloat units, GLfloat clamp)
{
  glPolygonOffsetClamp(factor, units, clamp);
}

void set_polygon_point_offset_enabled(bool enabled)
{
  enabled ? glEnable(GL_POLYGON_OFFSET_POINT) : glDisable(GL_POLYGON_OFFSET_POINT);
}
bool polygon_point_offset_enabled    ()
{
  return glIsEnabled(GL_POLYGON_OFFSET_POINT) != 0;
}
void set_polygon_line_offset_enabled (bool enabled)
{
  enabled ? glEnable(GL_POLYGON_OFFSET_LINE) : glDisable(GL_POLYGON_OFFSET_LINE);
}
bool polygon_line_offset_enabled     ()
{
  return glIsEnabled(GL_POLYGON_OFFSET_LINE) != 0;
}
void set_polygon_fill_offset_enabled (bool enabled)
{
  enabled ? glEnable(GL_POLYGON_OFFSET_FILL) : glDisable(GL_POLYGON_OFFSET_FILL);
}
bool polygon_fill_offset_enabled     ()
{
  return glIsEnabled(GL_POLYGON_OFFSET_FILL) != 0;
}
}