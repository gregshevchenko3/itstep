# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("SpiritCalc")
message(CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR})
include_directories(E:/boost_1_73_0)
# Include sub-projects.
add_subdirectory ("SpiritCalc")
