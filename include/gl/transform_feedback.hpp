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
  transform_feedback()
  {
    glCreateTransformFeedbacks(1, &id_);
  }
  explicit transform_feedback(GLuint id) : id_(id), managed_(false)
  {

  }
  transform_feedback(const transform_feedback&  that) = delete;
  transform_feedback(      transform_feedback&& temp) noexcept : id_(std::move(temp.id_)), managed_(std::move(temp.managed_))
  {
    temp.id_      = invalid_id;
    temp.managed_ = false;
  }
  virtual ~transform_feedback()
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
  
      id_      = std::move(temp.id_);
      managed_ = std::move(temp.managed_);
  
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
  bool        is_valid() const
  {
    return glIsTransformFeedback(id_) != 0;
  }

  void set_buffer_range(GLuint index, const buffer& buffer, GLintptr offset, GLsizeiptr size)
  {
    glTransformFeedbackBufferRange(id_, index, buffer.id(), offset, size);
  }
  void set_buffer_base (GLuint index, const buffer& buffer)
  {
    glTransformFeedbackBufferBase(id_, index, buffer.id());
  }
  
  static void begin (GLenum primitive_mode = GL_TRIANGLES)
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
  void draw                 (GLenum mode)                                        const
  {
    glDrawTransformFeedback(mode, id_);
  }
  void draw_instanced       (GLenum mode,                GLsizei instance_count) const
  {
    glDrawTransformFeedbackInstanced(mode, id_, instance_count);
  }
  void draw_stream          (GLenum mode, GLuint stream)                         const
  {
    glDrawTransformFeedbackStream(mode, id_, stream);
  }
  void draw_stream_instanced(GLenum mode, GLuint stream, GLsizei instance_count) const
  {
    glDrawTransformFeedbackStreamInstanced(mode, id_, stream, instance_count);
  }

  // 22.4 Transform feedback queries.
  bool    is_paused     () const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_PAUSED) != 0;
  }
  bool    is_active     () const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_ACTIVE) != 0;
  }
  GLint   buffer_binding(GLuint index) const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, index);
  }
  GLint   buffer_start  (GLuint index) const
  {
    return get_parameter(GL_TRANSFORM_FEEDBACK_BUFFER_START, index);
  }
  GLint64 buffer_size   (GLuint index) const
  {
    return get_parameter_64(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE, index);
  }

  static const GLenum native_type = GL_TRANSFORM_FEEDBACK;

  GLuint id() const
  {
    return id_;
  }

protected:
  GLint   get_parameter   (GLenum parameter) const
  {
    GLint result;
    glGetTransformFeedbackiv(id_, parameter, &result);
    return result;
  }
  GLint   get_parameter   (GLenum parameter, GLuint index) const
  {
    GLint result;
    glGetTransformFeedbacki_v(id_, parameter, index, &result);
    return result;
  }
  GLint64 get_parameter_64(GLenum parameter, GLuint index) const
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
