//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_UNMANAGED_HPP_
#define GL_UNMANAGED_HPP_

#include <export.hpp>

namespace gl
{
// Some object wrappers in this library have multiple constructors taking 
// a single unsigned integer parameter (whether it is a GLenum or GLuint).
// In order to distinguish the managed constructors (ones which do not take 
// in a raw object identifier, and instead create / destroy it on their own) 
// from unmanaged ones (which take in a raw object identifier and do not 
// delete them on scope exit), a parameter of type unmanaged_t has to 
// be specified in the unmanaged constructors.
//
// Example case:
// As GLenum and GLuint have the same underlying type (unsigned int), 
// the constructors below cannot be distinguished unless unmanaged_t
// is defined as an extra parameter to the second constructor. 
// Without unmanaged_t, the two constructors would be defined 
// with identical parameters and fail to compile.
//
// class shader
// {
//  public:
//   shader(GLenum shader_type);
//   shader(GLuint raw_id     , unmanaged_t unmanaged); 
//   ...
// }
// 
// Create a managed shader.
// mak::gl::shader managed_shader(GL_VERTEX_SHADER);
// 
// Create an unmanaged shader.
// GLuint raw_id = glCreateShader(GL_VERTEX_SHADER);
// mak::gl::shader unmanaged_shader(raw_id, mak::gl::unmanaged);

struct GL_EXPORT unmanaged_t
{
  
};

constexpr unmanaged_t unmanaged = unmanaged_t();
}

#endif
