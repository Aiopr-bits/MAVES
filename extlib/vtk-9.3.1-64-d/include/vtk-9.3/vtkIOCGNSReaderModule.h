
#ifndef VTKIOCGNSREADER_EXPORT_H
#define VTKIOCGNSREADER_EXPORT_H

#ifdef VTKIOCGNSREADER_STATIC_DEFINE
#  define VTKIOCGNSREADER_EXPORT
#  define VTKIOCGNSREADER_NO_EXPORT
#else
#  ifndef VTKIOCGNSREADER_EXPORT
#    ifdef IOCGNSReader_EXPORTS
        /* We are building this library */
#      define VTKIOCGNSREADER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOCGNSREADER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOCGNSREADER_NO_EXPORT
#    define VTKIOCGNSREADER_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOCGNSREADER_DEPRECATED
#  define VTKIOCGNSREADER_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKIOCGNSREADER_DEPRECATED_EXPORT
#  define VTKIOCGNSREADER_DEPRECATED_EXPORT VTKIOCGNSREADER_EXPORT VTKIOCGNSREADER_DEPRECATED
#endif

#ifndef VTKIOCGNSREADER_DEPRECATED_NO_EXPORT
#  define VTKIOCGNSREADER_DEPRECATED_NO_EXPORT VTKIOCGNSREADER_NO_EXPORT VTKIOCGNSREADER_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKIOCGNSREADER_NO_DEPRECATED
#    define VTKIOCGNSREADER_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkIOCGNSReaderModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKIOCGNSREADER_EXPORT_H */
