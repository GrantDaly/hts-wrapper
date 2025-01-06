cmake_minimum_required(VERSION 3.28)

project(hts-wrapper)


include(FindPkgConfig)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(version 0.0.1)



# make cache variables for install destinations
include(GNUInstallDirs)

include(GenerateExportHeader)

#find_library(htslib NAMES htslib hts libhts REQUIRED)
#pkg_search_module(htslib
#  REQUIRED htslib
#  )
#include_directories(include "${CONDA_PREFIX}/include")

#include_directories( ${PROJECT_SOURCE_DIR}/include)

#add_library(hts-wrapper SHARED IMPORTED)
add_library(hts-wrapper STATIC)



add_subdirectory(include)
add_subdirectory(source)
#"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
generate_export_header(hts-wrapper)

# doesn't seem to be correctly finding the system htslib and getting the correct linker flag
# i.e. pkg-config --libs htslib -> -L/usr/local/lib -lhts
# hacky solution is to change htslib to hts here so g++ should do -lhts instead of -lhtslib
# this seemed to work but should come up with long-term solution to correctly find system htslib
target_link_libraries (hts-wrapper PUBLIC hts)
# add compiler flags
target_compile_options(hts-wrapper PUBLIC -fmodules-ts)

add_subdirectory(tests)
# Install hts-wrapper
#install(FILES faidx.h DESTINATION include/hts-wrapper)
install(TARGETS hts-wrapper
   EXPORT hts-wrapperTargets
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   )

install(EXPORT hts-wrapperTargets
        FILE hts-wrapperTargets.cmake
        NAMESPACE hts::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
)

#haven't added a package version file yet
install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
          #"${CMAKE_CURRENT_BINARY_DIR}/hts-wrapperConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
)

export(EXPORT hts-wrapperTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperTargets.cmake"
       NAMESPACE hts::
)
