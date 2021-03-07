//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_TRANSFORM_FEEDBACK_HPP
#define GL_TRANSFORM_FEEDBACK_HPP

#include <gl/buffer.hpp>
#include <gl/opengl.hpp>

namespace gl
{
class transform_feedback
{
public:
  // 13.2 Transform feedback.
  transform_feedback           ()
  {
    glCreateTransformFeedbacks(1, &id_);
  }
  explicit transform_feedback  (const GLuint id) : id_(id), managed_(false)
  {

  }
  transform_feedback           (const transform_feedback&  that) = delete;
  transform_feedback           (      transform_feedback&& temp) noexcept : id_(temp.id_), managed_(temp.managed_)
  {
    temp.id_      = invalid_id;
    temp.managed_ = false;
  }
  virtual ~transform_feedback  ()
  {
    if (managed_ && id_ != invalid_id)
      glDeleteTransformFeedbacks(1, &id_);
  }
  transform_feedback& operator=(const transform_feedback&  that) = delete;
  transform_feedback& operator=(      transform_feedback&& temp) noexcept
  {
    if (this != &temp)
    {
      if (managed_ && id_ != invalid_id)
        glDeleteTransformFeedbacks(1, &id_);
  
      id_      = temp.id_;
      managed_ = temp.managed_;
  
      temp.id_      = invalid_id;
      temp.managed_ = false;    
    }
    return *this;
  }
  
  void        bind    () const
  {
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, id_);
  }
  static void unbind  ()
  {
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
  }
  [[nodiscard]]
  bool        is_valid() const
  {
    return glIsTransformFeedback(id_) != 0;
  }

  void set_buffer_range(const GLuint index, const buffer& buffer, const GLintptr offset, const GLsizeiptr size) const
  {
    glTransformFeedbackBufferRange(id_, index, buffer.id(), offset, size);
  }
  void set_buffer_base (const GLuint index, const buffer& buffer) const
  {
    glTransformFeedbackBufferBase(id_, index, buffer.id());
  }
  
  static void begin (const GLenum primitive_mode = GL_TRIANGLES)
  {
    glBeginTransformFeedback(primitive_mode);
  }
  static void pause ()
  {
    glPauseTransformFeedback();
  }
  static void resume()
  {
    glResumeTransformFeedback();
  }
  static void end   ()
  {
    glEndTransformFeedback();
  }

  // 13.2.3 Transform feedback drawing.
  void draw                 (const GLenum mode)                                                    const
  {
    glDrawTransformFeedback(mode, id_);
  }
  void draw_instanced       (const GLenum mode,                      const GLsizei instance_count) const
  {
    glDrawTransformFeedbackInstanced(mode, id_, instance_count);
  }
  void draw_stream          (const GLenum mode, const GLuint stream)                               const
  {
    glDrawTransformFeedbackStream(mode, id_, stream);
  }
  void draw_stream_instanced(const GLenum mode, const GLuint stream, const GLsizei instance_count) const
  {
    glDrawTransformFeedbackStreamInstanced(mode, id_, stream, instance_count);
  }

  // 22.4 Transform feedback queries.
  [[nodiscard]]
  bool    is_paused     () const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_PAUSED) != 0;
  }
  [[nodiscard]]
  bool    is_active     () const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_ACTIVE) != 0;
  }
  [[nodiscard]]
  GLint   buffer_binding(const GLuint index) const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, index);
  }
  [[nodiscard]]
  GLint   buffer_start  (const GLuint index) const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_BUFFER_START, index);
  }
  [[nodiscard]]
  GLint64 buffer_size   (const GLuint index) const
  {
    return get_parameter_64(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE, index);
  }

  static const GLenum native_type = GL_TRANSFORM_FEEDBACK;

  [[nodiscard]]
  GLuint id() const
  {
    return id_;
  }

protected:
  [[nodiscard]]
  GLint   get_parameter   (const GLenum parameter) const
  {
    GLint result;
    glGetTransformFeedbackiv(id_, parameter, &result);
    return result;
  }
  [[nodiscard]]
  GLint   get_parameter   (const GLenum parameter, const GLuint index) const
  {
    GLint result;
    glGetTransformFeedbacki_v(id_, parameter, index, &result);
    return result;
  }
  [[nodiscard]]
  GLint64 get_parameter_64(const GLenum parameter, const GLuint index) const
  {
    GLint64 result;
    glGetTransformFeedbacki64_v(id_, parameter, index, &result);
    return result;
  }

  GLuint id_      = invalid_id;
  bool   managed_ = true;
};
}

#endif
