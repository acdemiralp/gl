
#ifndef GL_EXPORT_H
#define GL_EXPORT_H

#ifdef GL_STATIC
#  define GL_EXPORT
#  define GL_NO_EXPORT
#else
#  ifndef GL_EXPORT
#    ifdef gl_EXPORTS
        /* We are building this library */
#      define GL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define GL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef GL_NO_EXPORT
#    define GL_NO_EXPORT 
#  endif
#endif

#ifndef GL_DEPRECATED
#  define GL_DEPRECATED __declspec(deprecated)
#endif

#ifndef GL_DEPRECATED_EXPORT
#  define GL_DEPRECATED_EXPORT GL_EXPORT GL_DEPRECATED
#endif

#ifndef GL_DEPRECATED_NO_EXPORT
#  define GL_DEPRECATED_NO_EXPORT GL_NO_EXPORT GL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef GL_NO_DEPRECATED
#    define GL_NO_DEPRECATED
#  endif
#endif

#endif
