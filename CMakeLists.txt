##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project               (gl VERSION 4.5 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 11)
set                   (CMAKE_CXX_VISIBILITY_PRESET hidden)
set                   (CMAKE_VISIBILITY_INLINES_HIDDEN 1)
string                (TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
string                (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

##################################################    Options     ##################################################
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." ON)
option(BUILD_TESTS "Build tests." OFF)
option(CUDA_INTEROP_SUPPORT "Include cuda interoperation support for buffers and textures." OFF)

##################################################    Sources     ##################################################
set(PROJECT_SOURCES
  CMakeLists.txt
  cmake/assign_source_group.cmake
  cmake/import_library.cmake
  
  include/gl/auxiliary/glm_uniforms.hpp
  include/gl/all.hpp
  include/gl/buffer.hpp
  include/gl/command_execution.hpp
  include/gl/compute.hpp
  include/gl/debug.hpp
  include/gl/draw_commands.hpp
  include/gl/framebuffer.hpp
  include/gl/image_handle.hpp
  include/gl/opengl.hpp
  include/gl/per_fragment_ops.hpp
  include/gl/pipeline.hpp
  include/gl/program.hpp
  include/gl/query.hpp
  include/gl/rasterization.hpp
  include/gl/renderbuffer.hpp
  include/gl/sampler.hpp
  include/gl/shader.hpp
  include/gl/state.hpp
  include/gl/sync.hpp
  include/gl/tessellation.hpp
  include/gl/texture.hpp
  include/gl/texture_handle.hpp
  include/gl/texture_view.hpp
  include/gl/transform_feedback.hpp
  include/gl/unmanaged.hpp
  include/gl/vertex_array.hpp
  include/gl/vertex_post_processing_ops.hpp
  include/gl/viewport.hpp

  source/buffer.cpp
  source/command_execution.cpp
  source/compute.cpp
  source/debug.cpp
  source/draw_commands.cpp
  source/framebuffer.cpp
  source/image_handle.cpp
  source/opengl.cpp
  source/per_fragment_ops.cpp
  source/pipeline.cpp
  source/program.cpp
  source/query.cpp
  source/rasterization.cpp
  source/renderbuffer.cpp
  source/sampler.cpp
  source/shader.cpp
  source/state.cpp
  source/sync.cpp
  source/tessellation.cpp
  source/texture.cpp
  source/texture_handle.cpp
  source/texture_view.cpp
  source/transform_feedback.cpp
  source/vertex_array.cpp
  source/vertex_post_processing_ops.cpp
  source/viewport.cpp

  license.txt
  readme.md
)
list(APPEND PROJECT_SOURCES "include/${PROJECT_NAME}/export.hpp")
include(assign_source_group)
assign_source_group(${PROJECT_SOURCES})

##################################################  Dependencies  ##################################################
include(import_library)

find_package  (GLEW REQUIRED)
import_library(GLEW_INCLUDE_DIR GLEW_LIBRARY)

find_package  (OpenGL REQUIRED)
import_library(OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)

if(CUDA_INTEROP_SUPPORT)
  find_package(CUDA REQUIRED)
  set(CUDA_NVCC_FLAGS 
    ${CUDA_NVCC_FLAGS} 
    "-gencode=arch=compute_35,code=sm_35" # Kepler
    "-gencode=arch=compute_50,code=sm_50" # Maxwell
    "-gencode=arch=compute_52,code=sm_52" # Maxwell
    "-gencode=arch=compute_60,code=sm_60" # Pascal
    "-gencode=arch=compute_61,code=sm_61" # Pascal
    "--expt-extended-lambda")
  import_library(CUDA_INCLUDE_DIRS CUDA_LIBRARIES)
endif()

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE source)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
  set(DEFINITIONS "${DEFINITIONS} -D${PROJECT_NAME_UPPER}_STATIC")
endif()
if(CUDA_INTEROP_SUPPORT)
  set(DEFINITIONS "${DEFINITIONS} -D${PROJECT_NAME_UPPER}_CUDA_INTEROP_SUPPORT")
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "${DEFINITIONS}")

##################################################    Postbuild   ##################################################
include               (GenerateExportHeader)
generate_export_header(${PROJECT_NAME} 
  EXPORT_FILE_NAME     ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/export.hpp
  EXPORT_MACRO_NAME    ${PROJECT_NAME_UPPER}_EXPORT
  STATIC_DEFINE        ${PROJECT_NAME_UPPER}_STATIC
)

##################################################    Testing     ##################################################
if(BUILD_TESTS)
  enable_testing()

  set(PROJECT_TEST_SOURCES
  
  )

  foreach(_SOURCE ${PROJECT_TEST_SOURCES})
    get_filename_component(_NAME ${_SOURCE} NAME_WE)
    set                   (_SOURCES tests/catch.hpp tests/main.cpp ${_SOURCE})
    add_executable        (${_NAME} ${_SOURCES})
    target_link_libraries (${_NAME} ${PROJECT_NAME})
    add_test              (${_NAME} ${_NAME})
    set_property          (TARGET ${_NAME} PROPERTY FOLDER "Tests")
    source_group          ("source" FILES ${_SOURCES})
  endforeach()
endif()

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT "${PROJECT_NAME}-config"
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY "include/"               DESTINATION "include")
install(EXPORT    "${PROJECT_NAME}-config" DESTINATION "cmake"  )
export (TARGETS   "${PROJECT_NAME}"        FILE        "${PROJECT_NAME}-config.cmake")
