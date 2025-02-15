
#ifndef VTKIOMOTIONFX_EXPORT_H
#define VTKIOMOTIONFX_EXPORT_H

#ifdef VTKIOMOTIONFX_STATIC_DEFINE
#  define VTKIOMOTIONFX_EXPORT
#  define VTKIOMOTIONFX_NO_EXPORT
#else
#  ifndef VTKIOMOTIONFX_EXPORT
#    ifdef IOMotionFX_EXPORTS
        /* We are building this library */
#      define VTKIOMOTIONFX_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOMOTIONFX_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOMOTIONFX_NO_EXPORT
#    define VTKIOMOTIONFX_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOMOTIONFX_DEPRECATED
#  define VTKIOMOTIONFX_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOMOTIONFX_DEPRECATED_EXPORT
#  define VTKIOMOTIONFX_DEPRECATED_EXPORT VTKIOMOTIONFX_EXPORT VTKIOMOTIONFX_DEPRECATED
#endif

#ifndef VTKIOMOTIONFX_DEPRECATED_NO_EXPORT
#  define VTKIOMOTIONFX_DEPRECATED_NO_EXPORT VTKIOMOTIONFX_NO_EXPORT VTKIOMOTIONFX_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOMOTIONFX_NO_DEPRECATED
#    define VTKIOMOTIONFX_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOMotionFXModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOMOTIONFX_EXPORT_H */
