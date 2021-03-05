//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_IMPLEMENTATION_COMMAND_EXECUTION_IPP_
#define GL_IMPLEMENTATION_COMMAND_EXECUTION_IPP_

namespace gl
{
// 2.3.1 OpenGL errors.
GLenum error()
{
  return glGetError();
}

// 2.3.2 Graphics reset recovery.
GLenum graphics_reset_status()
{
  return glGetGraphicsResetStatus();
}
GLenum reset_notification_strategy()
{
  GLint value;
  glGetIntegerv(GL_RESET_NOTIFICATION_STRATEGY, &value);
  return value;
}

// 2.3.3 Flush and finish.
void flush ()
{
  glFlush();
}
void finish()
{
  glFinish();
}
}

#endif