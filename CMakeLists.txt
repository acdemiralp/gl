##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (gl VERSION 4.6 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 11)
set                   (CMAKE_CXX_VISIBILITY_PRESET hidden)
set                   (CMAKE_VISIBILITY_INLINES_HIDDEN 1)

include               (set_max_warning_level)
set_max_warning_level ()

##################################################    Options     ##################################################
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." ON)
option(CUDA_INTEROP_SUPPORT "Include CUDA interoperation support for buffers and textures." OFF)

##################################################    Sources     ##################################################
file(GLOB_RECURSE PROJECT_HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES source/*.c source/*.cpp)
file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
set (PROJECT_EXPORT_HPP include/${PROJECT_NAME}/export.hpp)
set (PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES} 
  ${PROJECT_CMAKE_UTILS} 
  ${PROJECT_MISC} 
  ${PROJECT_EXPORT_HPP})

include            (assign_source_group)
assign_source_group(${PROJECT_FILES})

##################################################  Dependencies  ##################################################
include(import_library)

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
    "-gencode=arch=compute_70,code=sm_70" # Volta
    "--expt-extended-lambda")
  import_library(CUDA_INCLUDE_DIRS CUDA_LIBRARIES)
endif()

find_library  (GLEW REQUIRED)
import_library(GLEW_INCLUDE_DIRS GLEW_LIBRARIES)

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} ${PROJECT_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE source)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
  string               (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()

if(CUDA_INTEROP_SUPPORT)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_CUDA_INTEROP_SUPPORT)
endif()

##################################################    Postbuild   ##################################################
include               (GenerateExportHeader)
string                (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
generate_export_header(${PROJECT_NAME} 
  EXPORT_FILE_NAME     ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/export.hpp
  EXPORT_MACRO_NAME    ${PROJECT_NAME_UPPER}_EXPORT
  STATIC_DEFINE        ${PROJECT_NAME_UPPER}_STATIC
)

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT  ${PROJECT_NAME}-config DESTINATION cmake)
export (TARGETS ${PROJECT_NAME}        FILE        ${PROJECT_NAME}-config.cmake)
