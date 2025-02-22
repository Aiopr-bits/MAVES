
#ifndef VTKFILTERSTENSOR_EXPORT_H
#define VTKFILTERSTENSOR_EXPORT_H

#ifdef VTKFILTERSTENSOR_STATIC_DEFINE
#  define VTKFILTERSTENSOR_EXPORT
#  define VTKFILTERSTENSOR_NO_EXPORT
#else
#  ifndef VTKFILTERSTENSOR_EXPORT
#    ifdef FiltersTensor_EXPORTS
        /* We are building this library */
#      define VTKFILTERSTENSOR_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKFILTERSTENSOR_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKFILTERSTENSOR_NO_EXPORT
#    define VTKFILTERSTENSOR_NO_EXPORT 
#  endif
#endif

#ifndef VTKFILTERSTENSOR_DEPRECATED
#  define VTKFILTERSTENSOR_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKFILTERSTENSOR_DEPRECATED_EXPORT
#  define VTKFILTERSTENSOR_DEPRECATED_EXPORT VTKFILTERSTENSOR_EXPORT VTKFILTERSTENSOR_DEPRECATED
#endif

#ifndef VTKFILTERSTENSOR_DEPRECATED_NO_EXPORT
#  define VTKFILTERSTENSOR_DEPRECATED_NO_EXPORT VTKFILTERSTENSOR_NO_EXPORT VTKFILTERSTENSOR_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKFILTERSTENSOR_NO_DEPRECATED
#    define VTKFILTERSTENSOR_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkFiltersTensorModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKFILTERSTENSOR_EXPORT_H */
