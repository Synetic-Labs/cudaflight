#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LLVMEnzyme-20" for configuration "Release"
set_property(TARGET LLVMEnzyme-20 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMEnzyme-20 PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELEASE ""
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMEnzyme-20.so"
  IMPORTED_NO_SONAME_RELEASE "TRUE"
  )

list(APPEND _cmake_import_check_targets LLVMEnzyme-20 )
list(APPEND _cmake_import_check_files_for_LLVMEnzyme-20 "${_IMPORT_PREFIX}/lib/LLVMEnzyme-20.so" )

# Import target "ClangEnzyme-20" for configuration "Release"
set_property(TARGET ClangEnzyme-20 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ClangEnzyme-20 PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELEASE ""
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/ClangEnzyme-20.so"
  IMPORTED_NO_SONAME_RELEASE "TRUE"
  )

list(APPEND _cmake_import_check_targets ClangEnzyme-20 )
list(APPEND _cmake_import_check_files_for_ClangEnzyme-20 "${_IMPORT_PREFIX}/lib/ClangEnzyme-20.so" )

# Import target "LLDEnzyme-20" for configuration "Release"
set_property(TARGET LLDEnzyme-20 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLDEnzyme-20 PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELEASE ""
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLDEnzyme-20.so"
  IMPORTED_NO_SONAME_RELEASE "TRUE"
  )

list(APPEND _cmake_import_check_targets LLDEnzyme-20 )
list(APPEND _cmake_import_check_files_for_LLDEnzyme-20 "${_IMPORT_PREFIX}/lib/LLDEnzyme-20.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
