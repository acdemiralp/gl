//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_COMMAND_EXECUTION_HPP
#define GL_COMMAND_EXECUTION_HPP

#include <gl/opengl.hpp>

// 2.3 Command execution.
namespace gl
{
// 2.3.1 OpenGL errors.
inline GLenum error()
{
  return glGetError();
}

// 2.3.2 Graphics reset recovery.
inline GLenum graphics_reset_status      ()
{
  return glGetGraphicsResetStatus();
}
inline GLenum reset_notification_strategy()
{
  GLint value;
  glGetIntegerv(GL_RESET_NOTIFICATION_STRATEGY, &value);
  return value;
}

// 2.3.3 Flush and finish.
inline void flush ()
{
  glFlush();
}
inline void finish()
{
  glFinish();
}
}

#endif