# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

project ("ECC_CPU" 
LANGUAGES C CXX)

enable_testing()

# Include sub-projects.
add_subdirectory ("CoreLib")
add_subdirectory ("Assembler")
add_subdirectory ("Simulator")


