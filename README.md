# hts-wrapper
This is a c++ wrapper library for [htslib](https://github.com/samtools/htslib), a very widely used c library for high-throughput sequencing (HTS) data.

# Requirements
- Only intended to operate on Unix-like systems. So far, hts-wrapper has been tested on Ubuntu Linux only.
- Build system: CMake version 4.1.1 and g++ or clang (so far only g++ has been tested).
- System installed htslib, and its associated dependencies.

# Obtaining the software
It is recommended to obtain the software by cloning this GitHub repository.
```
git clone https://github.com/GrantDaly/hts-wrapper.git
```

# Building the software
Developer preference is assumed. 

Below is an abstracted command line invokation to configure and build the software. "<...>" denotes directories which can be substituted based on developer preference. Note that CMake will expect the top-level "CMakeLists.txt" file in <hts-wrapper-directory> and that it is recommended that <build-directory> be a separate directory than the source directory (a subdirectory of the top-level htd-wrapper directory is fine).

```
mkdir <build-directory>
cmake -S <hts-wrapper-directory> -B <build-directory>
cmake --build <build-directory>
```
A concrete example.
```
git clone https://github.com/GrantDaly/hts-wrapper.git
cd hts-wrapper
mkdir build
cd build
cmake -S ../ -B .
cmake --build .
```
# Installation
It is not currently recommended to install the software, and support for CMake installation files will be a low priority. 

The current goal is to configure this software to be consumed by other CMake projects. At present this hts-wrapper is intended to be included as a Git submodule within a consuming package's own source code. Next steps are to make hts-wrapper more independent, so it may be included with the CMake FetchContent mechanism. Instead of having to initialize the consuming package with a separate copy of hts-wrapper in a subdirectory, it is envisioned that hts-wrapper may be retreived by a specific Git tag with FetchContent. To develop hts-wrapper and its consumers simultaneously, overrides in the consuming package CMake Cache Variables may point to a local copy of hts-wrapper. 
