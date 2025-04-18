// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * When using 'fmt' in VTK, this header enables use to use either the fmt
 * version provided by VTK or an externally built version based on compile time
 * flags.
 *
 * When using 'fmt' include this header and then include any fmt header you need
 * as follows:
 *
 * ```c++
 *
 *  #include <vtk_fmt.>
 *
 *  // clang-format off
 *  #include VTK_FMT(fmt/core.h)
 *  // clang-format on
 *
 * ```
 *
 * Note the clang-format sentinels are need avoid incorrect formatting the
 * VTK_FMT macro call when using clang-format to format the code.
 */

#ifndef vtk_fmt_h
#define vtk_fmt_h

/* Use the fmt library configured for VTK.  */
#define VTK_MODULE_USE_EXTERNAL_VTK_fmt 0

#if VTK_MODULE_USE_EXTERNAL_VTK_fmt
# define VTK_FMT(header) <header>
#else
# define VTK_FMT(header) <vtkfmt/vtk##header>
#endif

#endif // #ifndef vtk_fmt_h
