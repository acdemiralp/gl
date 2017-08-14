//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_SYNC_HPP_
#define GL_SYNC_HPP_

#include <gl/export.hpp>
#include <gl/opengl.hpp>

namespace gl
{
class GL_EXPORT sync
{
public:
  // 4.1 Sync objects and fences.
  sync();
  sync(const sync&  that) = delete;
  sync(      sync&& temp) noexcept;
  virtual ~sync();
  sync& operator=(const sync&  that) = delete;
  sync& operator=(      sync&& temp) noexcept;

  // 4.1.1 Waiting for sync objects.
  GLenum client_wait(GLbitfield flags = GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64 timeout_ns = 10E+10) const;
  void   wait       () const;

  // 4.1.3 Sync object queries.
  GLenum status  () const;
  bool   is_valid() const;

  GLsync id() const;

protected:
  GLint get_property(GLenum property) const;

  GLsync id_;
};
}

#endif