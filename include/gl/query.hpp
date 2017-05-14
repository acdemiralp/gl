//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_QUERY_HPP_
#define GL_QUERY_HPP_

#include <type_traits>

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
  query(GLuint id) : id_(id), managed_(false)
  {
    
  }
  query(const query&  that) = delete ;
  query(      query&& temp) = default;
  virtual ~query()
  {
    if(managed_)
      glDeleteQueries(1, &id_);
  }
  query& operator=(const query&  that) = delete ;
  query& operator=(      query&& temp) = default;
  
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

  GLuint id_;
  bool   managed_ = true;
};

using any_samples_passed_query                    = query<GL_ANY_SAMPLES_PASSED>;
using any_samples_passed_query_conservative       = query<GL_ANY_SAMPLES_PASSED_CONSERVATIVE>;
using primitives_generated_query                  = query<GL_PRIMITIVES_GENERATED>;
using samples_passed_query                        = query<GL_SAMPLES_PASSED>;
using time_elapsed_query                          = query<GL_TIME_ELAPSED>;
using timestamp_query                             = query<GL_TIMESTAMP>;
using transform_feedback_primitives_written_query = query<GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN>;
}

#endif