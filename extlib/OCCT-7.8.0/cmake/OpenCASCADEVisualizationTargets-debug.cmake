#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TKService" for configuration "Debug"
set_property(TARGET TKService APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKService PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKService.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKService.dll"
  )

list(APPEND _cmake_import_check_targets TKService )
list(APPEND _cmake_import_check_files_for_TKService "${_IMPORT_PREFIX}/win64/vc14/libd/TKService.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKService.dll" )

# Import target "TKV3d" for configuration "Debug"
set_property(TARGET TKV3d APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKV3d PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKV3d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKV3d.dll"
  )

list(APPEND _cmake_import_check_targets TKV3d )
list(APPEND _cmake_import_check_files_for_TKV3d "${_IMPORT_PREFIX}/win64/vc14/libd/TKV3d.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKV3d.dll" )

# Import target "TKOpenGl" for configuration "Debug"
set_property(TARGET TKOpenGl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKOpenGl PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKOpenGl.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKOpenGl.dll"
  )

list(APPEND _cmake_import_check_targets TKOpenGl )
list(APPEND _cmake_import_check_files_for_TKOpenGl "${_IMPORT_PREFIX}/win64/vc14/libd/TKOpenGl.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKOpenGl.dll" )

# Import target "TKMeshVS" for configuration "Debug"
set_property(TARGET TKMeshVS APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKMeshVS PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKMeshVS.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKMeshVS.dll"
  )

list(APPEND _cmake_import_check_targets TKMeshVS )
list(APPEND _cmake_import_check_files_for_TKMeshVS "${_IMPORT_PREFIX}/win64/vc14/libd/TKMeshVS.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKMeshVS.dll" )

# Import target "TKIVtk" for configuration "Debug"
set_property(TARGET TKIVtk APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKIVtk PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKIVtk.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKIVtk.dll"
  )

list(APPEND _cmake_import_check_targets TKIVtk )
list(APPEND _cmake_import_check_files_for_TKIVtk "${_IMPORT_PREFIX}/win64/vc14/libd/TKIVtk.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKIVtk.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
