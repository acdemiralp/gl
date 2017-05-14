//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/transform_feedback.hpp>

namespace gl
{
transform_feedback::transform_feedback ()
{
  glCreateTransformFeedbacks(1, &id_);
}
transform_feedback::transform_feedback (GLuint id) : id_(id), managed_(false)
{

}
transform_feedback::~transform_feedback()
{
  if(managed_)
    glDeleteTransformFeedbacks(1, &id_);
}

void transform_feedback::bind    () const
{
  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, id_);
}
void transform_feedback::unbind  ()
{
  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}
bool transform_feedback::is_valid() const
{
  return glIsTransformFeedback(id_) != 0;
}

void transform_feedback::set_buffer_range(GLuint index, const buffer& buffer, GLintptr offset, GLsizeiptr size)
{
  glTransformFeedbackBufferRange(id_, index, buffer.id(), offset, size);
}
void transform_feedback::set_buffer_base (GLuint index, const buffer& buffer)
{
  glTransformFeedbackBufferBase(id_, index, buffer.id());
}

void transform_feedback::begin (GLenum primitive_mode)
{
  glBeginTransformFeedback(primitive_mode);
}
void transform_feedback::pause ()
{
  glPauseTransformFeedback();
}
void transform_feedback::resume()
{
  glResumeTransformFeedback();
}
void transform_feedback::end   ()
{
  glEndTransformFeedback();
}

// 13.2.3 Transform feedback drawing.
void transform_feedback::draw                 (GLenum mode)                                        const
{
  glDrawTransformFeedback(mode, id_);
}
void transform_feedback::draw_instanced       (GLenum mode,                GLsizei instance_count) const
{
  glDrawTransformFeedbackInstanced(mode, id_, instance_count);
}
void transform_feedback::draw_stream          (GLenum mode, GLuint stream)                         const
{
  glDrawTransformFeedbackStream(mode, id_, stream);
}
void transform_feedback::draw_stream_instanced(GLenum mode, GLuint stream, GLsizei instance_count) const
{
  glDrawTransformFeedbackStreamInstanced(mode, id_, stream, instance_count);
}

GLuint transform_feedback::id() const
{
  return id_;
}
}