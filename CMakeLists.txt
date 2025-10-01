cmake_minimum_required(VERSION 4.1.1)

project(hts-wrapper
VERSION 0.0.2)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # eventually plan to add "Clang" and possibly "AppleClang".
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.4.0)
    message(WARNING "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} older than oldest version tested (${CMAKE_CXX_COMPILER_ID} 13.3.0)")
  endif()
else()
  message(WARNING "${CMAKE_CXX_COMPILER_ID} compiler not tested")
endif()


include(FindPkgConfig)





#include(GenerateExportHeader)

### find htslib from system ###
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Htslib)

add_library(hts-wrapper SHARED)

# trying modern CMake version. TODO: check this more, as this feature appears to have caveats.
target_compile_features(hts-wrapper PRIVATE cxx_std_20)


target_sources(hts-wrapper
PUBLIC
FILE_SET installed
TYPE HEADERS
BASE_DIRS
include
${CMAKE_CURRENT_BINARY_DIR}/include
)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/include/hts-wrapper")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source")

target_link_libraries (hts-wrapper PUBLIC  Hts::Hts)

if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()

# # generate_export_header(hts-wrapper)
# make cache variables for install destinations
include(GNUInstallDirs)
install(TARGETS hts-wrapper
    # EXPORT hts-wrapperTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )



# install(EXPORT hts-wrapperTargets
#          FILE hts-wrapperTargets.cmake
#          NAMESPACE HtsWrap::
#          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
# )

# include(CMakePackageConfigHelpers)
# configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
#    "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
#    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
# )

# #package version file
# write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfigVersion.cmake"
# COMPATIBILITY AnyNewerVersion)

# install(FILES
#            "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
#            "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfigVersion.cmake"
#          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
#  )

# # this should be for including a prebuilt version of the library in a consuming build. Not going to be the recommended usage going forward
# export(EXPORT hts-wrapperTargets
#         FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperTargets.cmake"
#         NAMESPACE hts::
#  )
