//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_IMAGE_HANDLE_HPP
#define GL_IMAGE_HANDLE_HPP

#include <gl/texture.hpp>

#ifdef GL_ARB_bindless_texture

namespace gl
{
class image_handle
{
public:
  template<GLenum target>
  image_handle(const texture<target>& texture, const GLint level, const bool layered, const GLint layer, const GLenum format) : id_(glGetImageHandleARB(texture.id(), level, layered, layer, format))
  {
  
  }
  image_handle(const image_handle&  that) = default;
  image_handle(      image_handle&& temp) = default;
 ~image_handle()                          = default;

  image_handle& operator=(const image_handle&  that) = default;
  image_handle& operator=(      image_handle&& temp) = default;

  void set_resident(const bool resident, const GLenum access = GL_READ_WRITE) const
  {
    resident ? glMakeImageHandleResidentARB(id_, access) : glMakeImageHandleNonResidentARB(id_);
  }

  [[nodiscard]] bool is_resident () const
  {
    return glIsImageHandleResidentARB(id_) != 0;
  }

  [[nodiscard]] GLuint64 id() const
  {
    return id_;
  }

protected:
  GLuint64 id_;
};
}

#endif
#endif