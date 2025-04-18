
#ifndef VTKFILTERSHYBRID_EXPORT_H
#define VTKFILTERSHYBRID_EXPORT_H

#ifdef VTKFILTERSHYBRID_STATIC_DEFINE
#  define VTKFILTERSHYBRID_EXPORT
#  define VTKFILTERSHYBRID_NO_EXPORT
#else
#  ifndef VTKFILTERSHYBRID_EXPORT
#    ifdef FiltersHybrid_EXPORTS
        /* We are building this library */
#      define VTKFILTERSHYBRID_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKFILTERSHYBRID_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKFILTERSHYBRID_NO_EXPORT
#    define VTKFILTERSHYBRID_NO_EXPORT 
#  endif
#endif

#ifndef VTKFILTERSHYBRID_DEPRECATED
#  define VTKFILTERSHYBRID_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKFILTERSHYBRID_DEPRECATED_EXPORT
#  define VTKFILTERSHYBRID_DEPRECATED_EXPORT VTKFILTERSHYBRID_EXPORT VTKFILTERSHYBRID_DEPRECATED
#endif

#ifndef VTKFILTERSHYBRID_DEPRECATED_NO_EXPORT
#  define VTKFILTERSHYBRID_DEPRECATED_NO_EXPORT VTKFILTERSHYBRID_NO_EXPORT VTKFILTERSHYBRID_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKFILTERSHYBRID_NO_DEPRECATED
#    define VTKFILTERSHYBRID_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkFiltersHybridModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKFILTERSHYBRID_EXPORT_H */
