
#ifndef VTKRENDERINGIMAGE_EXPORT_H
#define VTKRENDERINGIMAGE_EXPORT_H

#ifdef VTKRENDERINGIMAGE_STATIC_DEFINE
#  define VTKRENDERINGIMAGE_EXPORT
#  define VTKRENDERINGIMAGE_NO_EXPORT
#else
#  ifndef VTKRENDERINGIMAGE_EXPORT
#    ifdef RenderingImage_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGIMAGE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKRENDERINGIMAGE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKRENDERINGIMAGE_NO_EXPORT
#    define VTKRENDERINGIMAGE_NO_EXPORT 
#  endif
#endif

#ifndef VTKRENDERINGIMAGE_DEPRECATED
#  define VTKRENDERINGIMAGE_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKRENDERINGIMAGE_DEPRECATED_EXPORT
#  define VTKRENDERINGIMAGE_DEPRECATED_EXPORT VTKRENDERINGIMAGE_EXPORT VTKRENDERINGIMAGE_DEPRECATED
#endif

#ifndef VTKRENDERINGIMAGE_DEPRECATED_NO_EXPORT
#  define VTKRENDERINGIMAGE_DEPRECATED_NO_EXPORT VTKRENDERINGIMAGE_NO_EXPORT VTKRENDERINGIMAGE_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKRENDERINGIMAGE_NO_DEPRECATED
#    define VTKRENDERINGIMAGE_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkRenderingImageModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKRENDERINGIMAGE_EXPORT_H */
