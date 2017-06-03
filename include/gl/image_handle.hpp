//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_IMAGE_HANDLE_HPP_
#define GL_IMAGE_HANDLE_HPP_

#include <gl/texture.hpp>

#include <export.hpp>

#ifdef GL_ARB_bindless_texture

namespace gl
{
class GL_EXPORT image_handle
{
public:
  template<GLenum target>
  image_handle(const texture<target>& texture, GLint level, bool layered, GLint layer, GLenum format) : id_(glGetImageHandleARB(texture.id(), level, layered, layer, format))
  {
  
  }
  image_handle(const image_handle&  that) = default;
  image_handle(      image_handle&& temp) = default;
 ~image_handle()                          = default;

  image_handle& operator=(const image_handle&  that) = default;
  image_handle& operator=(      image_handle&& temp) = default;

  void set_resident(bool resident, GLenum access = GL_READ_WRITE);
  bool is_resident () const;

protected:
  GLuint64 id_;
};
}

#endif
#endif