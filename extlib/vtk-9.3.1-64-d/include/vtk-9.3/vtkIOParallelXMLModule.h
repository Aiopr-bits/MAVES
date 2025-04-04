
#ifndef VTKIOPARALLELXML_EXPORT_H
#define VTKIOPARALLELXML_EXPORT_H

#ifdef VTKIOPARALLELXML_STATIC_DEFINE
#  define VTKIOPARALLELXML_EXPORT
#  define VTKIOPARALLELXML_NO_EXPORT
#else
#  ifndef VTKIOPARALLELXML_EXPORT
#    ifdef IOParallelXML_EXPORTS
        /* We are building this library */
#      define VTKIOPARALLELXML_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOPARALLELXML_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOPARALLELXML_NO_EXPORT
#    define VTKIOPARALLELXML_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOPARALLELXML_DEPRECATED
#  define VTKIOPARALLELXML_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOPARALLELXML_DEPRECATED_EXPORT
#  define VTKIOPARALLELXML_DEPRECATED_EXPORT VTKIOPARALLELXML_EXPORT VTKIOPARALLELXML_DEPRECATED
#endif

#ifndef VTKIOPARALLELXML_DEPRECATED_NO_EXPORT
#  define VTKIOPARALLELXML_DEPRECATED_NO_EXPORT VTKIOPARALLELXML_NO_EXPORT VTKIOPARALLELXML_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOPARALLELXML_NO_DEPRECATED
#    define VTKIOPARALLELXML_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOParallelXMLModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOPARALLELXML_EXPORT_H */
