
#ifndef VTKIOASYNCHRONOUS_EXPORT_H
#define VTKIOASYNCHRONOUS_EXPORT_H

#ifdef VTKIOASYNCHRONOUS_STATIC_DEFINE
#  define VTKIOASYNCHRONOUS_EXPORT
#  define VTKIOASYNCHRONOUS_NO_EXPORT
#else
#  ifndef VTKIOASYNCHRONOUS_EXPORT
#    ifdef IOAsynchronous_EXPORTS
        /* We are building this library */
#      define VTKIOASYNCHRONOUS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOASYNCHRONOUS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOASYNCHRONOUS_NO_EXPORT
#    define VTKIOASYNCHRONOUS_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOASYNCHRONOUS_DEPRECATED
#  define VTKIOASYNCHRONOUS_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOASYNCHRONOUS_DEPRECATED_EXPORT
#  define VTKIOASYNCHRONOUS_DEPRECATED_EXPORT VTKIOASYNCHRONOUS_EXPORT VTKIOASYNCHRONOUS_DEPRECATED
#endif

#ifndef VTKIOASYNCHRONOUS_DEPRECATED_NO_EXPORT
#  define VTKIOASYNCHRONOUS_DEPRECATED_NO_EXPORT VTKIOASYNCHRONOUS_NO_EXPORT VTKIOASYNCHRONOUS_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOASYNCHRONOUS_NO_DEPRECATED
#    define VTKIOASYNCHRONOUS_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOAsynchronousModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOASYNCHRONOUS_EXPORT_H */
