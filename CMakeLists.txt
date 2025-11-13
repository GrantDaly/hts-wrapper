cmake_minimum_required(VERSION 4.1.1)

project(HtsWrapper
VERSION 0.0.2)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # eventually plan to add "Clang" and possibly "AppleClang".
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.4.0)
    message(WARNING "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} older than oldest version tested (${CMAKE_CXX_COMPILER_ID} 13.3.0)")
  endif()
else()
  message(WARNING "${CMAKE_CXX_COMPILER_ID} compiler not tested")
endif()

include(FindPkgConfig)

### find htslib from system ###
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Htslib)

add_library(hts-wrapper SHARED)
add_library(HtsWrapper::hts-wrapper ALIAS hts-wrapper)

target_compile_features(hts-wrapper PRIVATE cxx_std_20)


# target_sources(hts-wrapper
# PUBLIC
# FILE_SET api
# TYPE HEADERS
# BASE_DIRS
# include
# ${CMAKE_CURRENT_BINARY_DIR}/include
# )
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/include/hts-wrapper")

include(GenerateExportHeader)
generate_export_header(hts-wrapper)

target_sources(hts-wrapper
PUBLIC
FILE_SET api
TYPE HEADERS
BASE_DIRS
include
"${CMAKE_CURRENT_SOURCE_DIR}/include"
FILES
include/hts-wrapper/bam-header.h
include/hts-wrapper/bam-record.h
include/hts-wrapper/cigar.h
include/hts-wrapper/faidx-cpp.h
include/hts-wrapper/sam-file.h
include/hts-wrapper/sam-iterator.h
include/hts-wrapper/hts-wrapper-util.h
include/hts-wrapper/aligned-insertion.hpp
include/hts-wrapper/snp.h
include/hts-wrapper/alignment-matrix.h

# problem that private includes not available to test executables, so I made everything public. TODO: look into making private includes available to them. Possibly define a private fileset and make that a source requirement for test executables
# looks like alias targets can be used for this. would export the private headers and then just have them as a requirement for the test executables https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#alias-targets
# PRIVATE
# FILE_SET privateincludes
# TYPE HEADERS
# BASE_DIRS
# source
# "${CMAKE_CURRENT_SOURCE_DIR}/source"
# FILES
# source/hts-wrapper-util.h
)

#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source")
target_sources(hts-wrapper PRIVATE
  source/faidx-cpp.cpp
  source/bam-header.cpp
  source/bam-record.cpp
  source/sam-file.cpp
  source/cigar.cpp)

target_link_libraries (hts-wrapper PUBLIC  Hts::Hts)

if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()

install(TARGETS hts-wrapper
  EXPORT HtsWrapper
  FILE_SET api)

install(EXPORT HtsWrapper
  DESTINATION cmake/HtsWrapper
  NAMESPACE HtsWrapper::
  )
