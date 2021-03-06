//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_TEXTURE_HANDLE_HPP
#define GL_TEXTURE_HANDLE_HPP

#ifdef GL_ARB_bindless_texture

#include <gl/sampler.hpp>
#include <gl/texture.hpp>

namespace gl
{
class texture_handle
{
public:
  template<GLenum target>
  explicit texture_handle  (const texture<target>& texture)
  : id_(glGetTextureHandleARB       (texture.id()))
  {

  }
  template<GLenum target>
  texture_handle           (const texture<target>& texture, const sampler& sampler)
  : id_(glGetTextureSamplerHandleARB(texture.id(), sampler.id()))
  {

  }
  texture_handle           (const texture_handle&  that) = default;
  texture_handle           (      texture_handle&& temp) = default;
 ~texture_handle           ()                            = default;
  texture_handle& operator=(const texture_handle&  that) = default;
  texture_handle& operator=(      texture_handle&& temp) = default;

  void     set_resident(const bool resident) const
  {
    resident ? glMakeTextureHandleResidentARB(id_) : glMakeTextureHandleNonResidentARB(id_);
  }

  [[nodiscard]]
  bool     is_resident () const
  {
    return glIsTextureHandleResidentARB(id_) != 0;
  }

  [[nodiscard]]
  GLuint64 id          () const
  {
    return id_;
  }

protected:
  GLuint64 id_;
};
}

#endif

#endif