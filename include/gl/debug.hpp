//          Copyright Ali Can Demiralp 2016 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GL_DEBUG_HPP
#define GL_DEBUG_HPP

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include <gl/opengl.hpp>
#include <gl/sync.hpp>

namespace gl
{
struct debug_log
{
  GLenum      source   = 0;
  GLenum      type     = 0;
  GLuint      id       = 0;
  GLenum      severity = 0;
  std::string message  = std::string(); // Warning C4251 inside this line.
};

// 20.0 Debug output.
inline void set_debug_output_enabled(const bool enabled)
{
  enabled ? glEnable(GL_DEBUG_OUTPUT) : glDisable(GL_DEBUG_OUTPUT);
}
inline bool debug_output_enabled    ()
{
  return glIsEnabled(GL_DEBUG_OUTPUT) != 0;
}

// 20.2 Debug message callback.
inline void set_debug_log_callback(const std::function<void(debug_log)>& callback)
{
  static auto function = callback;
  glDebugMessageCallback(static_cast<GLDEBUGPROC>([ ] (const GLenum source, const GLenum type, const GLuint id, const GLenum severity, GLsizei length, const GLchar* message, const void* data)
  {
    const auto* const function_ptr = static_cast<const std::function<void(debug_log)>*>(data);
    if (function_ptr != nullptr) 
      (*function_ptr)(debug_log({source, type, id, severity, std::string(message)}));
  }), reinterpret_cast<const void*>(&function));
}

// 20.4 Controlling debug messages.
inline void set_debug_log_filters(const GLenum source, const GLenum type, const std::vector<GLuint>& ids, const GLenum severity, const bool enabled)
{
  glDebugMessageControl(source, type, severity, static_cast<GLsizei>(ids.size()), ids.data(), enabled);
}

// 20.5 Externally generated messages.
inline void insert_debug_log(const debug_log& log)
{
  glDebugMessageInsert(log.source, log.type, log.id, log.severity, static_cast<GLsizei>(log.message.size()), log.message.data());
}

// 20.6 Debug groups.
inline void push_debug_group(const GLenum source, const GLuint id, const std::string& message)
{
  glPushDebugGroup(source, id, static_cast<GLsizei>(message.size()), message.data());
}
inline void pop_debug_group ()
{
  glPopDebugGroup();
}

// 20.7 Debug labels.
template<typename type>
void        set_object_label     (const type& object     , const std::string& label)
{
  glObjectLabel(type::native_type, object.id(), static_cast<GLsizei>(label.size()), label.data());
}
inline void set_sync_object_label(const sync& sync_object, const std::string& label)
{
  glObjectPtrLabel(sync_object.id(), static_cast<GLsizei>(label.size()), label.data());
}

// 20.8 Synchronous Debug output.
inline void set_synchronous_debug_output_enabled(const bool enabled)
{
  enabled ? glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS) : glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
inline bool synchronous_debug_output_enabled    ()
{
  return glIsEnabled(GL_DEBUG_OUTPUT_SYNCHRONOUS) != 0;
}

// 20.9 Debug output queries.
inline std::vector<debug_log> debug_logs(const GLuint count)
{
  auto max_message_size = 0;
  glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &max_message_size);

  std::vector<GLenum>  sources      (count);
  std::vector<GLenum>  types        (count);
  std::vector<GLuint>  ids          (count);
  std::vector<GLenum>  severities   (count);
  std::vector<GLsizei> message_sizes(count);
  std::vector<GLchar>  message_data (count * max_message_size);
  const auto real_count = glGetDebugMessageLog(count, static_cast<GLsizei>(message_data.size()), &sources[0], &types[0], &ids[0], &severities[0], &message_sizes[0], &message_data[0]);
  sources      .resize(real_count);
  types        .resize(real_count);
  ids          .resize(real_count);
  severities   .resize(real_count);
  message_sizes.resize(real_count);

  std::vector<std::string> messages(real_count);
  auto current_message = message_data.begin();
  for (std::size_t i = 0; i < message_sizes.size(); ++i)
  {
    messages[i] = std::string(current_message, current_message + message_sizes[i] - 1);
    current_message = current_message + message_sizes[i];
  }

  std::vector<debug_log> debug_logs(real_count);
  for (unsigned i = 0; i < real_count; i++)
    debug_logs[i] = debug_log{sources[i], types[i], ids[i], severities[i], messages[i]};
  return debug_logs;
}

template<typename type>
std::string        object_label     (const type& object)
{
  GLint       size ; glGetIntegerv(GL_MAX_LABEL_LENGTH, &size);
  std::string label; label.resize(size);
  glGetObjectLabel(type::native_type, object.id(), size, nullptr, &label[0]);
  return label;
}
inline std::string sync_object_label(const sync& sync_object)
{
  GLint size;
  glGetIntegerv(GL_MAX_LABEL_LENGTH, &size);
  std::string label;
  label.resize(size);
  glGetObjectPtrLabel(sync_object.id(), size, nullptr, &label[0]);
  return label;
}

// 22.2 String queries.
inline void* get_debug_callback_function ()
{
  GLvoid* result;
  glGetPointerv(GL_DEBUG_CALLBACK_FUNCTION, &result);
  return result;
}
inline void* get_debug_callback_user_data()
{
  GLvoid* result;
  glGetPointerv(GL_DEBUG_CALLBACK_USER_PARAM, &result);
  return result;
}
}

#endif