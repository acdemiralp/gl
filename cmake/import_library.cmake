# Imports a library which is not built with cmake.
# The imported library is appended to the PROJECT_LIBRARIES variable.
# Usage:
#   Header Only:
#     import_library(ALIAS INCLUDE_DIRS)
#   Identical Debug and Release:
#     import_library(ALIAS INCLUDE_DIRS LIBRARIES)
#   Separate  Debug and Release:
#     import_library(ALIAS INCLUDE_DIRS DEBUG_LIBRARIES RELEASE_LIBRARIES)
function(import_library ALIAS INCLUDE_DIRS)
  set(ALIAS ${PROJECT_NAME}::${ALIAS})
  add_library(${ALIAS} INTERFACE IMPORTED)
  set_property(TARGET ${ALIAS} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${${INCLUDE_DIRS}})
  
  set (_EXTRA_ARGS ${ARGN})
  list(LENGTH _EXTRA_ARGS _EXTRA_ARGS_LENGTH)
  if    (_EXTRA_ARGS_LENGTH EQUAL 1)
    list        (GET _EXTRA_ARGS 0 _LIBRARIES)
    set_property(TARGET ${ALIAS} PROPERTY INTERFACE_LINK_LIBRARIES ${${_LIBRARIES}})
  elseif(_EXTRA_ARGS_LENGTH EQUAL 2)
    list        (GET _EXTRA_ARGS 0 _DEBUG_LIBRARIES  )
    list        (GET _EXTRA_ARGS 1 _RELEASE_LIBRARIES)
    set_property(TARGET ${ALIAS} PROPERTY INTERFACE_LINK_LIBRARIES  
      $<$<CONFIG:Debug>:${${_DEBUG_LIBRARIES}}> 
      $<$<NOT:$<CONFIG:Debug>>:${${_RELEASE_LIBRARIES}}>)
  endif ()
  
  set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} ${ALIAS} PARENT_SCOPE)
endfunction(import_library)
