//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_RASTERIZATION_HPP_
#define GL_RASTERIZATION_HPP_

#include <array>

#include <gl/opengl.hpp>

namespace gl
{
// 14.0 Rasterization.
void set_rasterizer_discard_enabled(bool enabled);
bool rasterizer_discard_enabled    ();

// 14.3.1 Multisampling.
void set_multisampling_enabled (bool enabled);
bool multisampling_enabled     ();
void set_sample_shading_enabled(bool enabled);
bool sample_shading_enabled    ();

void set_minimum_sample_shading(GLfloat value = 1.0);

std::array<GLfloat, 2> multisample_sample_position(GLuint index);

// 14.4 Points.
void set_point_size_enabled(bool enabled);
bool point_size_enabled    ();

void set_point_size               (GLfloat size   = 1.0);
void set_point_fade_threshold_size(GLfloat size   = 1.0);
void set_point_sprite_coord_origin(GLenum  origin = GL_UPPER_LEFT);

// 14.5 Line segments.
void set_line_smoothing_enabled(bool enabled);
bool line_smoothing_enabled    ();

void set_line_width(GLfloat width);

// 14.6 Polygons.
void set_polygon_smoothing_enabled(bool enabled);
bool polygon_smoothing_enabled    ();

void set_polygon_face_culling_enabled(bool enabled);
bool polygon_face_culling_enabled    ();

void set_front_face(GLenum mode = GL_CCW );
void set_cull_face (GLenum mode = GL_BACK);

// 14.6.4 Polygon rasterization and depth offset.
void set_polygon_mode        (GLenum mode = GL_FILL);
void set_polygon_offset_clamp(GLfloat factor, GLfloat units, GLfloat clamp = 0.0F);

void set_polygon_point_offset_enabled(bool enabled);
bool polygon_point_offset_enabled    ();
void set_polygon_line_offset_enabled (bool enabled);
bool polygon_line_offset_enabled     ();
void set_polygon_fill_offset_enabled (bool enabled);
bool polygon_fill_offset_enabled     ();
}

#include <gl/implementation/rasterization.ipp>

#endif
