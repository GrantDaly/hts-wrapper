find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(Htslib REQUIRED htslib)
endif()

 find_path(Htslib_INCLUDE_DIR
   NAMES hts.h
   HINTS ${Htslib_INCLUDE_DIRS}
   PATH_SUFFIXES htslib
 )
 find_library(Htslib_LIBRARY
   NAMES htslib hts
   HINTS ${Htslib_LIBRARY_DIRS}
 )

#set(Htslib_VERSION ${Htslib_VERSION})

include(CMakePrintHelpers)
cmake_print_variables(Htslib_LIBRARY)
cmake_print_variables(Htslib_LIBRARY_DIRS)
cmake_print_variables(Htslib_INCLUDE_DIR)
cmake_print_variables(Htslib_INCLUDE_DIRS)

if(Htslib_FOUND AND NOT TARGET hts::htslib)
  message("Adding hts::htslib library target")
  add_library(hts::htslib UNKNOWN IMPORTED)
  set_target_properties(hts::htslib PROPERTIES
    IMPORTED_LOCATION "${Htslib_LIBRARY}"
    INTERFACE_COMPILE_OPTIONS "${Htslib_CFLAGS_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${Htslib_INCLUDE_DIR}"
  )
endif()