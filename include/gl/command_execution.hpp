//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_COMMAND_EXECUTION_HPP_
#define GL_COMMAND_EXECUTION_HPP_

#include <gl/opengl.hpp>

// 2.3 Command execution.
namespace gl
{
// 2.3.1 OpenGL errors.
GLenum error();

// 2.3.2 Graphics reset recovery.
GLenum graphics_reset_status();
GLenum reset_notification_strategy();

// 2.3.3 Flush and finish.
void flush ();
void finish();
}

#include <gl/implementation/command_execution.ipp>

#endif