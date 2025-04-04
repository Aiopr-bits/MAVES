
#ifndef VTKFILTERSIMAGING_EXPORT_H
#define VTKFILTERSIMAGING_EXPORT_H

#ifdef VTKFILTERSIMAGING_STATIC_DEFINE
#  define VTKFILTERSIMAGING_EXPORT
#  define VTKFILTERSIMAGING_NO_EXPORT
#else
#  ifndef VTKFILTERSIMAGING_EXPORT
#    ifdef FiltersImaging_EXPORTS
        /* We are building this library */
#      define VTKFILTERSIMAGING_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKFILTERSIMAGING_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKFILTERSIMAGING_NO_EXPORT
#    define VTKFILTERSIMAGING_NO_EXPORT 
#  endif
#endif

#ifndef VTKFILTERSIMAGING_DEPRECATED
#  define VTKFILTERSIMAGING_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKFILTERSIMAGING_DEPRECATED_EXPORT
#  define VTKFILTERSIMAGING_DEPRECATED_EXPORT VTKFILTERSIMAGING_EXPORT VTKFILTERSIMAGING_DEPRECATED
#endif

#ifndef VTKFILTERSIMAGING_DEPRECATED_NO_EXPORT
#  define VTKFILTERSIMAGING_DEPRECATED_NO_EXPORT VTKFILTERSIMAGING_NO_EXPORT VTKFILTERSIMAGING_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKFILTERSIMAGING_NO_DEPRECATED
#    define VTKFILTERSIMAGING_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkFiltersImagingModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKFILTERSIMAGING_EXPORT_H */
