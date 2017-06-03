//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/texture_handle.hpp>

namespace gl
{
void texture_handle::set_resident(bool resident)
{
  resident ? glMakeTextureHandleResidentARB(id_) : glMakeTextureHandleNonResidentARB(id_);
}
bool texture_handle::is_resident () const
{
  return glIsTextureHandleResidentARB(id_);
}

GLuint texture_handle::id() const
{
  return id_;
}
}
