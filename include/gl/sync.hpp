//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_SYNC_HPP
#define GL_SYNC_HPP

#include <utility>

#include <gl/opengl.hpp>

namespace gl
{
class sync
{
public:
  // 4.1 Sync objects and fences.
  sync() : id_(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0))
  {

  }
  sync(const sync&  that) = delete;
  sync(      sync&& temp) noexcept : id_(std::move(temp.id_))
  {
    temp.id_ = nullptr;
  }
  virtual ~sync()
  {
    if (id_ != nullptr)
      glDeleteSync(id_);
  }
  sync& operator=(const sync&  that) = delete;
  sync& operator=(      sync&& temp) noexcept
  {
    if (this != &temp)
    {
      if (id_ != nullptr)
        glDeleteSync(id_);
  
      id_      = std::move(temp.id_);
      temp.id_ = nullptr;
    }
    return *this;
  }

  // 4.1.1 Waiting for sync objects.
  GLenum client_wait(GLbitfield flags = GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64 timeout_ns = 10E+10) const
  {
    return glClientWaitSync(id_, flags, timeout_ns);
  }
  void   wait       () const
  {
    return glWaitSync(id_, 0, GL_TIMEOUT_IGNORED);
  }

  // 4.1.3 Sync object queries.
  GLenum status  () const
  {
    return get_property(GL_SYNC_STATUS);
  }
  bool   is_valid() const
  {
    return glIsSync(id_) != 0;
  }

  GLsync id() const
  {
    return id_;
  }

protected:
  GLint get_property(GLenum property) const
  {
    GLsizei size ;
    GLint   value;
    glGetSynciv(id_, property, 1, &size, &value);
    return value;
  }

  GLsync id_;
};
}

#endif