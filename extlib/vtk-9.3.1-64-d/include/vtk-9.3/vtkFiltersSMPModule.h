
#ifndef VTKFILTERSSMP_EXPORT_H
#define VTKFILTERSSMP_EXPORT_H

#ifdef VTKFILTERSSMP_STATIC_DEFINE
#  define VTKFILTERSSMP_EXPORT
#  define VTKFILTERSSMP_NO_EXPORT
#else
#  ifndef VTKFILTERSSMP_EXPORT
#    ifdef FiltersSMP_EXPORTS
        /* We are building this library */
#      define VTKFILTERSSMP_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKFILTERSSMP_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKFILTERSSMP_NO_EXPORT
#    define VTKFILTERSSMP_NO_EXPORT 
#  endif
#endif

#ifndef VTKFILTERSSMP_DEPRECATED
#  define VTKFILTERSSMP_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKFILTERSSMP_DEPRECATED_EXPORT
#  define VTKFILTERSSMP_DEPRECATED_EXPORT VTKFILTERSSMP_EXPORT VTKFILTERSSMP_DEPRECATED
#endif

#ifndef VTKFILTERSSMP_DEPRECATED_NO_EXPORT
#  define VTKFILTERSSMP_DEPRECATED_NO_EXPORT VTKFILTERSSMP_NO_EXPORT VTKFILTERSSMP_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKFILTERSSMP_NO_DEPRECATED
#    define VTKFILTERSSMP_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkFiltersSMPModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKFILTERSSMP_EXPORT_H */
