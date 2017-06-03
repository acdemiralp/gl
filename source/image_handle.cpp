//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/image_handle.hpp>

namespace gl
{
void image_handle::set_resident(bool resident, GLenum access)
{
  resident ? glMakeImageHandleResidentARB(id_, access) : glMakeImageHandleNonResidentARB(id_);
}
bool image_handle::is_resident () const
{
  return glIsImageHandleResidentARB(id_);
}

GLuint image_handle::id() const
{
  return id_;
}
}
