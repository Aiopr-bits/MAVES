
#ifndef VTKIOOGGTHEORA_EXPORT_H
#define VTKIOOGGTHEORA_EXPORT_H

#ifdef VTKIOOGGTHEORA_STATIC_DEFINE
#  define VTKIOOGGTHEORA_EXPORT
#  define VTKIOOGGTHEORA_NO_EXPORT
#else
#  ifndef VTKIOOGGTHEORA_EXPORT
#    ifdef IOOggTheora_EXPORTS
        /* We are building this library */
#      define VTKIOOGGTHEORA_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOOGGTHEORA_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOOGGTHEORA_NO_EXPORT
#    define VTKIOOGGTHEORA_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOOGGTHEORA_DEPRECATED
#  define VTKIOOGGTHEORA_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOOGGTHEORA_DEPRECATED_EXPORT
#  define VTKIOOGGTHEORA_DEPRECATED_EXPORT VTKIOOGGTHEORA_EXPORT VTKIOOGGTHEORA_DEPRECATED
#endif

#ifndef VTKIOOGGTHEORA_DEPRECATED_NO_EXPORT
#  define VTKIOOGGTHEORA_DEPRECATED_NO_EXPORT VTKIOOGGTHEORA_NO_EXPORT VTKIOOGGTHEORA_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOOGGTHEORA_NO_DEPRECATED
#    define VTKIOOGGTHEORA_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOOggTheoraModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOOGGTHEORA_EXPORT_H */
