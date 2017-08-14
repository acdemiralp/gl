//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_TEXTURE_HANDLE_HPP_
#define GL_TEXTURE_HANDLE_HPP_

#include <gl/export.hpp>
#include <gl/sampler.hpp>
#include <gl/texture.hpp>

#ifdef GL_ARB_bindless_texture

namespace gl
{
class GL_EXPORT texture_handle
{
public:
  template<GLenum target>
  texture_handle(const texture<target>& texture) : id_(glGetTextureHandleARB(texture.id()))
  {

  }
  template<GLenum target>
  texture_handle(const texture<target>& texture, const sampler& sampler) : id_(glGetTextureSamplerHandleARB(texture.id(), sampler.id()))
  {

  }
  texture_handle(const texture_handle&  that) = default;
  texture_handle(      texture_handle&& temp) = default;
 ~texture_handle()                            = default;

  texture_handle& operator=(const texture_handle&  that) = default;
  texture_handle& operator=(      texture_handle&& temp) = default;

  void set_resident(bool resident);
  bool is_resident () const;

  GLuint64 id() const;

protected:
  GLuint64 id_;
};
}

#endif
#endif