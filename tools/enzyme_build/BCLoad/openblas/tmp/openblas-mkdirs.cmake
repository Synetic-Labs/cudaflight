# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/src/openblas")
  file(MAKE_DIRECTORY "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/src/openblas")
endif()
file(MAKE_DIRECTORY
  "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/src/openblas-build"
  "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/install"
  "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/tmp"
  "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/src/openblas-stamp"
  "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/src"
  "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/src/openblas-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/src/openblas-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/luke/Code/betaflight/tools/enzyme_build/BCLoad/openblas/src/openblas-stamp${cfgdir}") # cfgdir has leading slash
endif()
