cmake_minimum_required(VERSION 4.1.1)

project(hts-wrapper
VERSION 0.0.2)


include(FindPkgConfig)


# make cache variables for install destinations
include(GNUInstallDirs)

include(GenerateExportHeader)

### find htslib from system ###
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Htslib)

add_library(hts-wrapper SHARED)

# TODO: delete once replacement vetted
# set_property(TARGET hts-wrapper PROPERTY CXX_STANDARD 20)
# trying modern CMake version. TODO: check this more, as this feature appears to have caveats.
target_compile_features(hts-wrapper PRIVATE cxx_std_20)


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source")

target_link_libraries (hts-wrapper PUBLIC  Hts::Hts)

if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()

generate_export_header(hts-wrapper)

install(TARGETS hts-wrapper
    EXPORT hts-wrapperTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR/hts-wrapper}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR/hts-wrapper}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR/hts-wrapper}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR/hts-wrapper}
    )



install(EXPORT hts-wrapperTargets
         FILE hts-wrapperTargets.cmake
         NAMESPACE HtsWrap::
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
   "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
   INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
)

#package version file
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfigVersion.cmake"
COMPATIBILITY AnyNewerVersion)

install(FILES
           "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
           "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfigVersion.cmake"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
 )

# this should be for including a prebuilt version of the library in a consuming build. Not going to be the recommended usage going forward
export(EXPORT hts-wrapperTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperTargets.cmake"
        NAMESPACE hts::
 )
