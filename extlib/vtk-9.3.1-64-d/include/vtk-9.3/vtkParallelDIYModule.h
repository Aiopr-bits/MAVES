
#ifndef VTKPARALLELDIY_EXPORT_H
#define VTKPARALLELDIY_EXPORT_H

#ifdef VTKPARALLELDIY_STATIC_DEFINE
#  define VTKPARALLELDIY_EXPORT
#  define VTKPARALLELDIY_NO_EXPORT
#else
#  ifndef VTKPARALLELDIY_EXPORT
#    ifdef ParallelDIY_EXPORTS
        /* We are building this library */
#      define VTKPARALLELDIY_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKPARALLELDIY_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKPARALLELDIY_NO_EXPORT
#    define VTKPARALLELDIY_NO_EXPORT 
#  endif
#endif

#ifndef VTKPARALLELDIY_DEPRECATED
#  define VTKPARALLELDIY_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKPARALLELDIY_DEPRECATED_EXPORT
#  define VTKPARALLELDIY_DEPRECATED_EXPORT VTKPARALLELDIY_EXPORT VTKPARALLELDIY_DEPRECATED
#endif

#ifndef VTKPARALLELDIY_DEPRECATED_NO_EXPORT
#  define VTKPARALLELDIY_DEPRECATED_NO_EXPORT VTKPARALLELDIY_NO_EXPORT VTKPARALLELDIY_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKPARALLELDIY_NO_DEPRECATED
#    define VTKPARALLELDIY_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkParallelDIYModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKPARALLELDIY_EXPORT_H */
