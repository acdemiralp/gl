//          Copyright Ali Can Demiralp 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gl/debug.hpp>

namespace gl
{
void set_debug_output_enabled(bool enabled)
{
  enabled ? glEnable(GL_DEBUG_OUTPUT) : glDisable(GL_DEBUG_OUTPUT);
}
bool debug_output_enabled    ()
{
  return glIsEnabled(GL_DEBUG_OUTPUT) != 0;
}

void set_debug_log_callback(const std::function<void(const debug_log&)>& callback, void* user_data)
{
  std::function<void(GLenum, GLenum, GLuint, GLenum, GLsizei, const char*, const void*)> function =
  [&](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* data)
  {
    callback(debug_log({source, type, id, severity, std::string(message), data}));
  };
  glDebugMessageCallback((GLDEBUGPROC)function.target<void(GLenum, GLenum, GLuint, GLenum, GLsizei, const char*, const void*)>(), user_data);
}

void set_debug_log_filters(GLenum source, GLenum type, const std::vector<GLuint>& ids, GLenum severity, bool enabled)
{
  glDebugMessageControl(source, type, severity, static_cast<GLsizei>(ids.size()), ids.data(), enabled);
}

void insert_debug_log(const debug_log& log)
{
  glDebugMessageInsert(log.source, log.type, log.id, log.severity, static_cast<GLsizei>(log.message.size()), log.message.data());
}

void push_debug_group(GLenum source, GLuint id, const std::string& message)
{
  glPushDebugGroup(source, id, static_cast<GLsizei>(message.size()), message.data());
}
void pop_debug_group ()
{
  glPopDebugGroup();
}

void set_sync_object_label(const sync& sync_object, const std::string& label)
{
  glObjectPtrLabel(sync_object.id(), static_cast<GLsizei>(label.size()), label.data());
}

void set_syncronous_debug_output_enabled(bool enabled)
{
  enabled ? glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS) : glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
bool synchronous_debug_output_enabled   ()
{
  return glIsEnabled(GL_DEBUG_OUTPUT_SYNCHRONOUS) != 0;
}

std::vector<debug_log> debug_logs(GLuint count)
{
  auto max_message_size = 0;
  glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &max_message_size);

  std::vector<GLenum>  sources      (count);
  std::vector<GLenum>  types        (count);
  std::vector<GLuint>  ids          (count);
  std::vector<GLenum>  severities   (count);
  std::vector<GLsizei> message_sizes(count);
  std::vector<GLchar>  message_data (count * max_message_size);
  auto real_count = glGetDebugMessageLog(count, static_cast<GLsizei>(message_data.size()), &sources[0], &types[0], &ids[0], &severities[0], &message_sizes[0], &message_data[0]);
  sources      .resize(real_count);
  types        .resize(real_count);
  ids          .resize(real_count);
  severities   .resize(real_count);
  message_sizes.resize(real_count);

  std::vector<std::string> messages(real_count);
  auto current_message = message_data.begin();
  for (size_t i = 0; i < message_sizes.size(); ++i)
  {
    messages[i] = std::string(current_message, current_message + message_sizes[i] - 1);
    current_message = current_message + message_sizes[i];
  }

  std::vector<debug_log> debug_logs(real_count);
  for (unsigned i = 0; i < real_count; i++)
    debug_logs[i] = debug_log{sources[i], types[i], ids[i], severities[i], messages[i], nullptr};
  return debug_logs;
}

std::string sync_object_label(const sync& sync_object)
{
  GLint size;
  glGetIntegerv(GL_MAX_LABEL_LENGTH, &size);
  std::string label;
  label.resize(size);
  glGetObjectPtrLabel(sync_object.id(), size, nullptr, &label[0]);
  return label;
}

void* get_debug_callback_function ()
{
  GLvoid* result;
  glGetPointerv(GL_DEBUG_CALLBACK_FUNCTION, &result);
  return result;
}
void* get_debug_callback_user_data()
{
  GLvoid* result;
  glGetPointerv(GL_DEBUG_CALLBACK_USER_PARAM, &result);
  return result;
}
}
