
#ifndef VTKRENDERINGVTKJS_EXPORT_H
#define VTKRENDERINGVTKJS_EXPORT_H

#ifdef VTKRENDERINGVTKJS_STATIC_DEFINE
#  define VTKRENDERINGVTKJS_EXPORT
#  define VTKRENDERINGVTKJS_NO_EXPORT
#else
#  ifndef VTKRENDERINGVTKJS_EXPORT
#    ifdef RenderingVtkJS_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGVTKJS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKRENDERINGVTKJS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKRENDERINGVTKJS_NO_EXPORT
#    define VTKRENDERINGVTKJS_NO_EXPORT 
#  endif
#endif

#ifndef VTKRENDERINGVTKJS_DEPRECATED
#  define VTKRENDERINGVTKJS_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKRENDERINGVTKJS_DEPRECATED_EXPORT
#  define VTKRENDERINGVTKJS_DEPRECATED_EXPORT VTKRENDERINGVTKJS_EXPORT VTKRENDERINGVTKJS_DEPRECATED
#endif

#ifndef VTKRENDERINGVTKJS_DEPRECATED_NO_EXPORT
#  define VTKRENDERINGVTKJS_DEPRECATED_NO_EXPORT VTKRENDERINGVTKJS_NO_EXPORT VTKRENDERINGVTKJS_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKRENDERINGVTKJS_NO_DEPRECATED
#    define VTKRENDERINGVTKJS_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkRenderingVtkJSModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKRENDERINGVTKJS_EXPORT_H */
