
#ifndef VTKRENDERINGQT_EXPORT_H
#define VTKRENDERINGQT_EXPORT_H

#ifdef VTKRENDERINGQT_STATIC_DEFINE
#  define VTKRENDERINGQT_EXPORT
#  define VTKRENDERINGQT_NO_EXPORT
#else
#  ifndef VTKRENDERINGQT_EXPORT
#    ifdef RenderingQt_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGQT_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKRENDERINGQT_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKRENDERINGQT_NO_EXPORT
#    define VTKRENDERINGQT_NO_EXPORT 
#  endif
#endif

#ifndef VTKRENDERINGQT_DEPRECATED
#  define VTKRENDERINGQT_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKRENDERINGQT_DEPRECATED_EXPORT
#  define VTKRENDERINGQT_DEPRECATED_EXPORT VTKRENDERINGQT_EXPORT VTKRENDERINGQT_DEPRECATED
#endif

#ifndef VTKRENDERINGQT_DEPRECATED_NO_EXPORT
#  define VTKRENDERINGQT_DEPRECATED_NO_EXPORT VTKRENDERINGQT_NO_EXPORT VTKRENDERINGQT_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKRENDERINGQT_NO_DEPRECATED
#    define VTKRENDERINGQT_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkRenderingQtModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKRENDERINGQT_EXPORT_H */
