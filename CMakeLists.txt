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

### find htslib from system ###
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Htslib)

#include_directories(include "${CONDA_PREFIX}/include")

#include_directories( ${PROJECT_SOURCE_DIR}/include)

#add_library(hts-wrapper SHARED IMPORTED)
add_library(hts-wrapper STATIC)
set_property(TARGET hts-wrapper PROPERTY CXX_STANDARD 20)


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source")

# "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
# generate_export_header(hts-wrapper)

# doesn't seem to be correctly finding the system htslib and getting the correct linker flag
# i.e. pkg-config --libs htslib -> -L/usr/local/lib -lhts
# hacky solution is to change htslib to hts here so g++ should do -lhts instead of -lhtslib
# this seemed to work but should come up with long-term solution to correctly find system htslib



target_link_libraries (hts-wrapper PUBLIC  hts::htslib)
#target_include_directories(hts-wrapper PUBLIC "${CMAKE_BINARY_DIR}/hts-wrapper/include") # /opt/include)


# add compiler flags
target_compile_options(hts-wrapper PRIVATE -fmodules-ts)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
# # Install hts-wrapper
# install(FILES faidx-cpp.h DESTINATION include/hts-wrapper)

### copying includes over so I can use the build directory from a higher-level project
### do not intend this to be a long-term solution
file(COPY include/ DESTINATION "${CMAKE_BINARY_DIR}/include/hts-wrapper/"
FILES_MATCHING REGEX "[a-zA-Z-_]+\.h(pp)?")

#file(COPY include/*.hpp DESTINATION "${CMAKE_BINARY_DIR}/include/hts-wrapper/")
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
         NAMESPACE hts::
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
   "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
   INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
)

# haven't added a package version file yet
install(FILES
           "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
           #"${CMAKE_CURRENT_BINARY_DIR}/hts-wrapperConfigVersion.cmake"
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
 )

# # install(FILES
# #           "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperConfig.cmake"
# #         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hts-wrapper
# # )

export(EXPORT hts-wrapperTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/hts-wrapperTargets.cmake"
        NAMESPACE hts::
 )
