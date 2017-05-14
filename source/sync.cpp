//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/sync.hpp>

namespace gl
{
sync::sync () : id_(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0))
{
  
}
sync::~sync()
{
  glDeleteSync(id_);
}

GLenum sync::client_wait(GLbitfield flags, GLuint64 timeout_ns) const
{
  return glClientWaitSync(id_, flags, timeout_ns);
}
void   sync::wait       () const
{
  return glWaitSync(id_, 0, GL_TIMEOUT_IGNORED);
}

GLenum sync::status  () const
{
  return get_property(GL_SYNC_STATUS);
}
bool   sync::is_valid() const
{
  return glIsSync(id_) != 0;
}

GLsync sync::id() const
{
  return id_;
}

GLint sync::get_property(GLenum property) const
{
  GLsizei size ;
  GLint   value;
  glGetSynciv(id_, property, 1, &size, &value);
  return value;
}
}