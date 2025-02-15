
#ifndef VTKIOCITYGML_EXPORT_H
#define VTKIOCITYGML_EXPORT_H

#ifdef VTKIOCITYGML_STATIC_DEFINE
#  define VTKIOCITYGML_EXPORT
#  define VTKIOCITYGML_NO_EXPORT
#else
#  ifndef VTKIOCITYGML_EXPORT
#    ifdef IOCityGML_EXPORTS
        /* We are building this library */
#      define VTKIOCITYGML_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOCITYGML_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOCITYGML_NO_EXPORT
#    define VTKIOCITYGML_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOCITYGML_DEPRECATED
#  define VTKIOCITYGML_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOCITYGML_DEPRECATED_EXPORT
#  define VTKIOCITYGML_DEPRECATED_EXPORT VTKIOCITYGML_EXPORT VTKIOCITYGML_DEPRECATED
#endif

#ifndef VTKIOCITYGML_DEPRECATED_NO_EXPORT
#  define VTKIOCITYGML_DEPRECATED_NO_EXPORT VTKIOCITYGML_NO_EXPORT VTKIOCITYGML_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOCITYGML_NO_DEPRECATED
#    define VTKIOCITYGML_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOCityGMLModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOCITYGML_EXPORT_H */
