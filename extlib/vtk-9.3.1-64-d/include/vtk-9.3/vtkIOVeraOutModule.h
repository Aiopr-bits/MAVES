
#ifndef VTKIOVERAOUT_EXPORT_H
#define VTKIOVERAOUT_EXPORT_H

#ifdef VTKIOVERAOUT_STATIC_DEFINE
#  define VTKIOVERAOUT_EXPORT
#  define VTKIOVERAOUT_NO_EXPORT
#else
#  ifndef VTKIOVERAOUT_EXPORT
#    ifdef IOVeraOut_EXPORTS
        /* We are building this library */
#      define VTKIOVERAOUT_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOVERAOUT_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOVERAOUT_NO_EXPORT
#    define VTKIOVERAOUT_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOVERAOUT_DEPRECATED
#  define VTKIOVERAOUT_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOVERAOUT_DEPRECATED_EXPORT
#  define VTKIOVERAOUT_DEPRECATED_EXPORT VTKIOVERAOUT_EXPORT VTKIOVERAOUT_DEPRECATED
#endif

#ifndef VTKIOVERAOUT_DEPRECATED_NO_EXPORT
#  define VTKIOVERAOUT_DEPRECATED_NO_EXPORT VTKIOVERAOUT_NO_EXPORT VTKIOVERAOUT_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOVERAOUT_NO_DEPRECATED
#    define VTKIOVERAOUT_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOVeraOutModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOVERAOUT_EXPORT_H */
