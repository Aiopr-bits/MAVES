
#ifndef VTKFILTERSGENERIC_EXPORT_H
#define VTKFILTERSGENERIC_EXPORT_H

#ifdef VTKFILTERSGENERIC_STATIC_DEFINE
#  define VTKFILTERSGENERIC_EXPORT
#  define VTKFILTERSGENERIC_NO_EXPORT
#else
#  ifndef VTKFILTERSGENERIC_EXPORT
#    ifdef FiltersGeneric_EXPORTS
        /* We are building this library */
#      define VTKFILTERSGENERIC_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKFILTERSGENERIC_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKFILTERSGENERIC_NO_EXPORT
#    define VTKFILTERSGENERIC_NO_EXPORT 
#  endif
#endif

#ifndef VTKFILTERSGENERIC_DEPRECATED
#  define VTKFILTERSGENERIC_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKFILTERSGENERIC_DEPRECATED_EXPORT
#  define VTKFILTERSGENERIC_DEPRECATED_EXPORT VTKFILTERSGENERIC_EXPORT VTKFILTERSGENERIC_DEPRECATED
#endif

#ifndef VTKFILTERSGENERIC_DEPRECATED_NO_EXPORT
#  define VTKFILTERSGENERIC_DEPRECATED_NO_EXPORT VTKFILTERSGENERIC_NO_EXPORT VTKFILTERSGENERIC_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKFILTERSGENERIC_NO_DEPRECATED
#    define VTKFILTERSGENERIC_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkFiltersGenericModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKFILTERSGENERIC_EXPORT_H */
