#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TKExpress" for configuration "Debug"
set_property(TARGET TKExpress APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(TKExpress PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/win64/vc14/libd/TKExpress.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/win64/vc14/bind/TKExpress.dll"
  )

list(APPEND _cmake_import_check_targets TKExpress )
list(APPEND _cmake_import_check_files_for_TKExpress "${_IMPORT_PREFIX}/win64/vc14/libd/TKExpress.lib" "${_IMPORT_PREFIX}/win64/vc14/bind/TKExpress.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
