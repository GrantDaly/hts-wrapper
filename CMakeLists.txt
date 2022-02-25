project(hts-wrapper)
cmake_minimum_required(VERSION 3.21)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(version 0.0.1)

# make cache variables for install destinations
include(GNUInstallDirs)

include(GenerateExportHeader)

#find_library(htslib NAMES htslib hts libhts REQUIRED)
#include_directories(include "${CONDA_PREFIX}/include")

#include_directories( ${PROJECT_SOURCE_DIR}/include)

add_library(hts-wrapper STATIC)

add_subdirectory(include)
add_subdirectory(source)
#"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
generate_export_header(hts-wrapper)

target_link_libraries (hts-wrapper PRIVATE htslib)

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
