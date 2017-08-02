//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_SHADER_HPP_
#define GL_SHADER_HPP_

#include <fstream>
#include <string>

#include <gl/export.hpp>
#include <gl/opengl.hpp>
#include <gl/unmanaged.hpp>

namespace gl
{
class GL_EXPORT shader
{
public:
  // 7.1 Shader objects.
  shader(GLenum type);
  shader(GLuint id, unmanaged_t unmanaged);
  shader(const shader&  that) = delete ;
  shader(      shader&& temp) = default;
  virtual ~shader();
  shader& operator=(const shader&  that) = delete ;
  shader& operator=(      shader&& temp) = default;

  void set_source(const std::string& source) const;
  bool compile   () const;
  bool is_valid  () const;

  static void release_compiler();

  // 7.13 Shader queries.
  GLenum      type            () const;
  bool        compile_status  () const;
  bool        delete_status   () const;
  bool        is_spir_v_binary() const;
  GLsizei     source_length   () const;
  GLsizei     info_log_length () const;
  std::string source          () const;
  std::string info_log        () const;

  static const GLenum native_type = GL_SHADER;

  GLuint id() const;

  // Extended functionality.
  void load_source(const std::string& filename) const;

protected:
  GLint get_parameter(GLenum parameter) const;

  GLuint id_      = 0;
  bool   managed_ = true;
};
}
#endif
