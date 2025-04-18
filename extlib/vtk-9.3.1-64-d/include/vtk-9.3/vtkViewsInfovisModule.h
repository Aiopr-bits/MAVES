
#ifndef VTKVIEWSINFOVIS_EXPORT_H
#define VTKVIEWSINFOVIS_EXPORT_H

#ifdef VTKVIEWSINFOVIS_STATIC_DEFINE
#  define VTKVIEWSINFOVIS_EXPORT
#  define VTKVIEWSINFOVIS_NO_EXPORT
#else
#  ifndef VTKVIEWSINFOVIS_EXPORT
#    ifdef ViewsInfovis_EXPORTS
        /* We are building this library */
#      define VTKVIEWSINFOVIS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKVIEWSINFOVIS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKVIEWSINFOVIS_NO_EXPORT
#    define VTKVIEWSINFOVIS_NO_EXPORT 
#  endif
#endif

#ifndef VTKVIEWSINFOVIS_DEPRECATED
#  define VTKVIEWSINFOVIS_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKVIEWSINFOVIS_DEPRECATED_EXPORT
#  define VTKVIEWSINFOVIS_DEPRECATED_EXPORT VTKVIEWSINFOVIS_EXPORT VTKVIEWSINFOVIS_DEPRECATED
#endif

#ifndef VTKVIEWSINFOVIS_DEPRECATED_NO_EXPORT
#  define VTKVIEWSINFOVIS_DEPRECATED_NO_EXPORT VTKVIEWSINFOVIS_NO_EXPORT VTKVIEWSINFOVIS_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKVIEWSINFOVIS_NO_DEPRECATED
#    define VTKVIEWSINFOVIS_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkViewsInfovisModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKVIEWSINFOVIS_EXPORT_H */
