find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  # can add back QUIET flag eventually to allow lower logging
  pkg_check_modules(PC_Htslib QUIET htslib)

  find_path(Htslib_INCLUDE_DIR
  NAMES hts.h
  HINTS ${PC_Htslib_INCLUDE_DIRS}
  PATH_SUFFIXES htslib
)

find_library(Htslib_LIBRARY
  NAMES hts
  HINTS ${PC_Htslib_LIBRARY_DIRS}
)

set(Htslib_VERSION ${PC_Htslib_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Htslib
  REQUIRED_VARS
    Htslib_LIBRARY
    Htslib_INCLUDE_DIR
  VERSION_VAR Htslib_VERSION
)

if(Htslib_FOUND AND NOT TARGET Hts::Hts)
  add_library(Hts::Hts UNKNOWN IMPORTED)
  set_target_properties(Hts::Hts PROPERTIES
    IMPORTED_LOCATION "${Htslib_LIBRARY}"
    INTERFACE_COMPILE_OPTIONS "${PC_Htslib_CFLAGS_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${Htslib_INCLUDE_DIR}" #"should only contain information about the target itself, and not any of its dependencies."
  )

  message("Include" ${Htslib_INCLUDE_DIR})
  message("Lib" ${Htslib_LIBRARY})
  message("I'face compile opt. " "${PC_Htslib_CFLAGS_OTHER}")
  message("Version" ${Htslib_VERSION})
endif()
endif()
