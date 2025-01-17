
#ifndef VTKIOTECPLOTTABLE_EXPORT_H
#define VTKIOTECPLOTTABLE_EXPORT_H

#ifdef VTKIOTECPLOTTABLE_STATIC_DEFINE
#  define VTKIOTECPLOTTABLE_EXPORT
#  define VTKIOTECPLOTTABLE_NO_EXPORT
#else
#  ifndef VTKIOTECPLOTTABLE_EXPORT
#    ifdef IOTecplotTable_EXPORTS
        /* We are building this library */
#      define VTKIOTECPLOTTABLE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOTECPLOTTABLE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOTECPLOTTABLE_NO_EXPORT
#    define VTKIOTECPLOTTABLE_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOTECPLOTTABLE_DEPRECATED
#  define VTKIOTECPLOTTABLE_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOTECPLOTTABLE_DEPRECATED_EXPORT
#  define VTKIOTECPLOTTABLE_DEPRECATED_EXPORT VTKIOTECPLOTTABLE_EXPORT VTKIOTECPLOTTABLE_DEPRECATED
#endif

#ifndef VTKIOTECPLOTTABLE_DEPRECATED_NO_EXPORT
#  define VTKIOTECPLOTTABLE_DEPRECATED_NO_EXPORT VTKIOTECPLOTTABLE_NO_EXPORT VTKIOTECPLOTTABLE_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOTECPLOTTABLE_NO_DEPRECATED
#    define VTKIOTECPLOTTABLE_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOTecplotTableModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOTECPLOTTABLE_EXPORT_H */
