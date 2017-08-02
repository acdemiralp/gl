//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_RASTERIZATION_HPP_
#define GL_RASTERIZATION_HPP_

#include <array>

#include <gl/export.hpp>
#include <gl/opengl.hpp>

namespace gl
{
// 14.0 Rasterization.
GL_EXPORT void set_rasterizer_discard_enabled(bool enabled);
GL_EXPORT bool rasterizer_discard_enabled    ();

// 14.3.1 Multisampling.
GL_EXPORT void set_multisampling_enabled (bool enabled);
GL_EXPORT bool multisampling_enabled     ();
GL_EXPORT void set_sample_shading_enabled(bool enabled);
GL_EXPORT bool sample_shading_enabled    ();

GL_EXPORT void set_minimum_sample_shading(GLfloat value = 1.0);

GL_EXPORT std::array<GLfloat, 2> multisample_sample_position(GLuint index);

// 14.4 Points.
GL_EXPORT void set_point_size_enabled(bool enabled);
GL_EXPORT bool point_size_enabled    ();

GL_EXPORT void set_point_size               (GLfloat size   = 1.0);
GL_EXPORT void set_point_fade_threshold_size(GLfloat size   = 1.0);
GL_EXPORT void set_point_sprite_coord_origin(GLenum  origin = GL_UPPER_LEFT);

// 14.5 Line segments.
GL_EXPORT void set_line_smoothing_enabled(bool enabled);
GL_EXPORT bool line_smoothing_enabled    ();

GL_EXPORT void set_line_width(GLfloat width);

// 14.6 Polygons.
GL_EXPORT void set_polygon_smoothing_enabled(bool enabled);
GL_EXPORT bool polygon_smoothing_enabled    ();

GL_EXPORT void set_polygon_face_culling_enabled(bool enabled);
GL_EXPORT bool polygon_face_culling_enabled    ();

GL_EXPORT void set_front_face(GLenum mode = GL_CCW );
GL_EXPORT void set_cull_face (GLenum mode = GL_BACK);

// 14.6.4 Polygon rasterization and depth offset.
GL_EXPORT void set_polygon_mode        (GLenum mode = GL_FILL);
GL_EXPORT void set_polygon_offset_clamp(GLfloat factor, GLfloat units, GLfloat clamp = 0.0F);

GL_EXPORT void set_polygon_point_offset_enabled(bool enabled);
GL_EXPORT bool polygon_point_offset_enabled    ();
GL_EXPORT void set_polygon_line_offset_enabled (bool enabled);
GL_EXPORT bool polygon_line_offset_enabled     ();
GL_EXPORT void set_polygon_fill_offset_enabled (bool enabled);
GL_EXPORT bool polygon_fill_offset_enabled     ();
}

#endif
