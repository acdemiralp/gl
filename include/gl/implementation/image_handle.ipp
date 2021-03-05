//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_IMPLEMENTATION_IMAGE_HANDLE_IPP_
#define GL_IMPLEMENTATION_IMAGE_HANDLE_IPP_

namespace gl
{
void image_handle::set_resident(bool resident, GLenum access)
{
  resident ? glMakeImageHandleResidentARB(id_, access) : glMakeImageHandleNonResidentARB(id_);
}
bool image_handle::is_resident () const
{
  return glIsImageHandleResidentARB(id_) != 0;
}

GLuint64 image_handle::id() const
{
  return id_;
}
}

#endif