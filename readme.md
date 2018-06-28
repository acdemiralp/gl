#### GL
Complete OpenGL 4.6 Core Profile Wrapper for C++11.

---

#### Dependencies
* OpenGL
* GLEW
* CUDA (optional, for interoperation support) 

---

#### Building
* Follow the cmake build process for locating the dependencies.
* Toggle BUILD_SHARED_LIBS for creating dynamic library.
* Toggle CUDA_INTEROP_SUPPORT for Cuda interoperation support. Note that the build will ask for the location of Cuda upon enabling this option.

---

#### Getting Started

Creating and uploading data to buffers:

```cpp
#include <gl/all.hpp>

void buffer_example()
{
  gl::buffer buffer;
  
  std::vector<float> vertices(32);
  buffer.set_data(sizeof(float) * vertices.size(), vertices.data());
}
```

Creating and uploading data to textures:

```cpp
#include <gl/all.hpp>

void texture_example()
{
  gl::texture_1d texture_1d;
  texture_1d.set_min_filter(GL_LINEAR);
  texture_1d.set_mag_filter(GL_LINEAR);
  texture_1d.set_wrap_s    (GL_CLAMP );
  
  std::array<float, 32> scalars;
  texture_1d.set_storage   (0, GL_RGBA, sizeof(float) * scalars.size());
  texture_1d.set_sub_image (0, 0, scalars.size(), GL_RGBA, GL_FLOAT, scalars.data());
}
```
Creating shaders and attaching them to programs:
```cpp
#include <gl/all.hpp>

void shader_program_example()
{
  gl::shader vert_shader(GL_VERTEX_SHADER);
  vert_shader.load_source("vert_example.glsl");

  gl::shader frag_shader(GL_FRAGMENT_SHADER);
  frag_shader.load_source("frag_example.glsl");

  gl::program program;
  program.attach_shader(vert_shader);
  program.attach_shader(frag_shader);
  program.link();
  program.use();
  program.set_uniform(program.uniform_location("example_uniform"), 42);
  program.unuse();
}
```

---

#### Extensions

For adding GLM support to type-inferring uniform variable setters of the shader program, simply `#include <gl/auxiliary/glm_uniforms.hpp>`.