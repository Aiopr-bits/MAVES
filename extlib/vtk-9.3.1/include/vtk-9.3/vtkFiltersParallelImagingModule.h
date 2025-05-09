
#ifndef VTKFILTERSPARALLELIMAGING_EXPORT_H
#define VTKFILTERSPARALLELIMAGING_EXPORT_H

#ifdef VTKFILTERSPARALLELIMAGING_STATIC_DEFINE
#  define VTKFILTERSPARALLELIMAGING_EXPORT
#  define VTKFILTERSPARALLELIMAGING_NO_EXPORT
#else
#  ifndef VTKFILTERSPARALLELIMAGING_EXPORT
#    ifdef FiltersParallelImaging_EXPORTS
        /* We are building this library */
#      define VTKFILTERSPARALLELIMAGING_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKFILTERSPARALLELIMAGING_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKFILTERSPARALLELIMAGING_NO_EXPORT
#    define VTKFILTERSPARALLELIMAGING_NO_EXPORT 
#  endif
#endif

#ifndef VTKFILTERSPARALLELIMAGING_DEPRECATED
#  define VTKFILTERSPARALLELIMAGING_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKFILTERSPARALLELIMAGING_DEPRECATED_EXPORT
#  define VTKFILTERSPARALLELIMAGING_DEPRECATED_EXPORT VTKFILTERSPARALLELIMAGING_EXPORT VTKFILTERSPARALLELIMAGING_DEPRECATED
#endif

#ifndef VTKFILTERSPARALLELIMAGING_DEPRECATED_NO_EXPORT
#  define VTKFILTERSPARALLELIMAGING_DEPRECATED_NO_EXPORT VTKFILTERSPARALLELIMAGING_NO_EXPORT VTKFILTERSPARALLELIMAGING_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKFILTERSPARALLELIMAGING_NO_DEPRECATED
#    define VTKFILTERSPARALLELIMAGING_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkFiltersParallelImagingModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKFILTERSPARALLELIMAGING_EXPORT_H */
