#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TKG2d" for configuration "Debug"
set_property(TARGET TKG2d APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKG2d PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKG2d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKG2d.dll"
  )

list(APPEND _cmake_import_check_targets TKG2d )
list(APPEND _cmake_import_check_files_for_TKG2d "${_IMPORT_PREFIX}/win64/vc14/libd/TKG2d.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKG2d.dll" )

# Import target "TKG3d" for configuration "Debug"
set_property(TARGET TKG3d APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKG3d PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKG3d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKG3d.dll"
  )

list(APPEND _cmake_import_check_targets TKG3d )
list(APPEND _cmake_import_check_files_for_TKG3d "${_IMPORT_PREFIX}/win64/vc14/libd/TKG3d.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKG3d.dll" )

# Import target "TKGeomBase" for configuration "Debug"
set_property(TARGET TKGeomBase APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKGeomBase PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKGeomBase.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKGeomBase.dll"
  )

list(APPEND _cmake_import_check_targets TKGeomBase )
list(APPEND _cmake_import_check_files_for_TKGeomBase "${_IMPORT_PREFIX}/win64/vc14/libd/TKGeomBase.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKGeomBase.dll" )

# Import target "TKBRep" for configuration "Debug"
set_property(TARGET TKBRep APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKBRep PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKBRep.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKBRep.dll"
  )

list(APPEND _cmake_import_check_targets TKBRep )
list(APPEND _cmake_import_check_files_for_TKBRep "${_IMPORT_PREFIX}/win64/vc14/libd/TKBRep.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKBRep.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
