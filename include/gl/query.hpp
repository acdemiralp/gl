//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_QUERY_HPP_
#define GL_QUERY_HPP_

#include <type_traits>
#include <utility>

#include <gl/opengl.hpp>

namespace gl
{
template<GLenum target>
class query
{
public:
  // 4.2 Asynchronous queries.
  query()
  {
    glCreateQueries(target, 1, &id_);
  }
  explicit query(GLuint id) : id_(id), managed_(false)
  {
    
  }
  query(const query&  that) = delete;
  query(      query&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
  {
    temp.id_      = invalid_id;
    temp.managed_ = false;
  }
  virtual ~query()
  {
    if(managed_ && id_ != invalid_id)
      glDeleteQueries(1, &id_);
  }
  query& operator=(const query&  that) = delete;
  query& operator=(      query&& temp) noexcept
  {
    if (this != &temp)
    {
      id_      = std::move(temp.id_);
      managed_ = std::move(temp.managed_);

      temp.id_      = invalid_id;
      temp.managed_ = false;    
    }
    return *this;
  }
  
  void        begin        ()             const
  {
    glBeginQuery       (target, id_);
  }
  void        begin_indexed(GLuint index) const
  {
    glBeginQueryIndexed(target, index, id_);
  }
  static void end          ()
  {
    glEndQuery         (target);
  }
  static void end_indexed  (GLuint index)
  {
    glEndQueryIndexed  (target, index);
  }
  bool        is_valid     () const
  {
    return glIsQuery(id_);
  }

  static GLenum  current     ()
  {
    GLint  result;
    glGetQueryiv       (target,        GL_CURRENT_QUERY, &result);
    return result;
  }
  static GLenum  current     (GLuint index)
  {
    GLint  result;
    glGetQueryIndexediv(target, index, GL_CURRENT_QUERY, &result);
    return result;
  }
  static GLsizei counter_bits()
  {
    GLint  result;
    glGetQueryiv       (target,        GL_QUERY_COUNTER_BITS, &result);
    return result;
  }
  static GLsizei counter_bits(GLuint index)
  {
    GLint  result;
    glGetQueryIndexediv(target, index, GL_QUERY_COUNTER_BITS, &result);
    return result;
  }

  bool   available     () const
  {
    return get_parameter_64(GL_QUERY_RESULT_AVAILABLE) != 0;
  }
  GLuint result_no_wait() const
  {
    return get_parameter_64(GL_QUERY_RESULT_NO_WAIT);
  }
  GLuint result        () const
  {
    return get_parameter_64(GL_QUERY_RESULT);
  }

  // 4.3 Timer queries.
  template<typename = typename std::enable_if<target == GL_TIME_ELAPSED || target == GL_TIMESTAMP>::type>
  void           counter  () const
  {
    glQueryCounter(id_, GL_TIMESTAMP);
  }
  template<typename = typename std::enable_if<target == GL_TIME_ELAPSED || target == GL_TIMESTAMP>::type>
  static GLint64 timestamp()
  {
    GLint64 result;
    glGetInteger64v(GL_TIMESTAMP, &result);
    return  result;
  }

  // 10.9 Conditional rendering.
  template<typename = typename std::enable_if<target == GL_SAMPLES_PASSED || target == GL_ANY_SAMPLES_PASSED>::type>
  void        begin_conditional_render(GLenum mode = GL_QUERY_BY_REGION_NO_WAIT) const
  {
    glBeginConditionalRender(id_, mode);
  }
  template<typename = typename std::enable_if<target == GL_SAMPLES_PASSED || target == GL_ANY_SAMPLES_PASSED>::type>
  static void end_conditional_render  ()
  {
    glEndConditionalRender();
  }

  static const GLenum native_type = GL_QUERY;

  GLuint id() const
  {
    return id_;  
  }

protected:
  GLuint64 get_parameter_64(GLenum parameter) const
  {
    GLuint64 result = 0;
    glGetQueryObjectui64v(id_, parameter, &result);
    return   result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};

using any_samples_passed_query                         = query<GL_ANY_SAMPLES_PASSED>;
using any_samples_passed_query_conservative            = query<GL_ANY_SAMPLES_PASSED_CONSERVATIVE>;
using primitives_generated_query                       = query<GL_PRIMITIVES_GENERATED>;
using samples_passed_query                             = query<GL_SAMPLES_PASSED>;
using time_elapsed_query                               = query<GL_TIME_ELAPSED>;
using primitives_submitted_query                       = query<GL_PRIMITIVES_SUBMITTED>;
using vertices_submitted_query                         = query<GL_VERTICES_SUBMITTED>;
using transform_feedback_primitives_written_query      = query<GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN>;
using transform_feedback_overflow_query                = query<GL_TRANSFORM_FEEDBACK_OVERFLOW>;
using transform_feedback_stream_overflow_query         = query<GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW>;
using compute_shader_invocations_query                 = query<GL_COMPUTE_SHADER_INVOCATIONS>;
using vertex_shader_invocations_query                  = query<GL_VERTEX_SHADER_INVOCATIONS>;
using fragment_shader_invocations_query                = query<GL_FRAGMENT_SHADER_INVOCATIONS>;
using geometry_shader_invocations_query                = query<GL_GEOMETRY_SHADER_INVOCATIONS>;
using tessellation_evaluation_shader_invocations_query = query<GL_TESS_EVALUATION_SHADER_INVOCATIONS>;
using tessellation_control_shader_patches_query        = query<GL_TESS_CONTROL_SHADER_PATCHES>;
using geometry_shader_primitives_emitted_query         = query<GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED>;
using clipping_input_primitives_query                  = query<GL_CLIPPING_INPUT_PRIMITIVES>;
using clipping_output_primitives_query                 = query<GL_CLIPPING_OUTPUT_PRIMITIVES>;
using timestamp_query                                  = query<GL_TIMESTAMP>;
}

#endif