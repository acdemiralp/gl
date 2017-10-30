//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_DEBUG_HPP_
#define GL_DEBUG_HPP_

#include <functional>
#include <string>
#include <vector>

#include <gl/export.hpp>
#include <gl/opengl.hpp>
#include <gl/sync.hpp>

namespace gl
{
struct GL_EXPORT debug_log
{
  GLenum      source   ;
  GLenum      type     ;
  GLuint      id       ;
  GLenum      severity ;
  std::string message  ; // Warning C4251 inside this line.
  const void* user_data;
};

// 20.0 Debug output.
GL_EXPORT void set_debug_output_enabled(bool enabled);
GL_EXPORT bool debug_output_enabled    ();

// 20.2 Debug message callback.
GL_EXPORT void set_debug_log_callback(const std::function<void(const debug_log&)>& callback, void* user_data = nullptr);

// 20.4 Controlling debug messages.
GL_EXPORT void set_debug_log_filters(GLenum source, GLenum type, const std::vector<GLuint>& ids, GLenum severity, bool enabled);

// 20.5 Externally generated messages.
GL_EXPORT void insert_debug_log(const debug_log& log);

// 20.6 Debug groups.
GL_EXPORT void push_debug_group(GLenum source, GLuint id, const std::string& message);
GL_EXPORT void pop_debug_group ();

// 20.7 Debug labels.
template<typename type>
void set_object_label     (const type& object     , const std::string& label)
{
  glObjectLabel(type::native_type, object.id(), label.size(), label.data());
}
GL_EXPORT void set_sync_object_label(const sync& sync_object, const std::string& label);

// 20.8 Synchronous Debug output.
GL_EXPORT void set_syncronous_debug_output_enabled(bool enabled);
GL_EXPORT bool synchronous_debug_output_enabled   ();

// 20.9 Debug output queries.
GL_EXPORT std::vector<debug_log> debug_logs(GLuint count);

template<typename type>
std::string object_label     (const type& object)
{
  GLint       size ; glGetIntegerv(GL_MAX_LABEL_LENGTH, &size);
  std::string label; label.resize(size);
  glGetObjectLabel(type::native_type, object.id(), size, nullptr, &label[0]);
  return label;
}
GL_EXPORT std::string sync_object_label(const sync& sync_object);

// 22.2 String queries.
void* get_debug_callback_function ();
void* get_debug_callback_user_data();
}

#endif