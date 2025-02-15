
#ifndef VTKIOINFOVIS_EXPORT_H
#define VTKIOINFOVIS_EXPORT_H

#ifdef VTKIOINFOVIS_STATIC_DEFINE
#  define VTKIOINFOVIS_EXPORT
#  define VTKIOINFOVIS_NO_EXPORT
#else
#  ifndef VTKIOINFOVIS_EXPORT
#    ifdef IOInfovis_EXPORTS
        /* We are building this library */
#      define VTKIOINFOVIS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOINFOVIS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOINFOVIS_NO_EXPORT
#    define VTKIOINFOVIS_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOINFOVIS_DEPRECATED
#  define VTKIOINFOVIS_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOINFOVIS_DEPRECATED_EXPORT
#  define VTKIOINFOVIS_DEPRECATED_EXPORT VTKIOINFOVIS_EXPORT VTKIOINFOVIS_DEPRECATED
#endif

#ifndef VTKIOINFOVIS_DEPRECATED_NO_EXPORT
#  define VTKIOINFOVIS_DEPRECATED_NO_EXPORT VTKIOINFOVIS_NO_EXPORT VTKIOINFOVIS_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOINFOVIS_NO_DEPRECATED
#    define VTKIOINFOVIS_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOInfovisModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOINFOVIS_EXPORT_H */
