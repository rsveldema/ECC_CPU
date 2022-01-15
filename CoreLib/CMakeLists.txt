# CMakeList.txt : CMake project for CoreLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
add_library (CoreLib "CoreLib.cpp" "CoreLib.h" "SimComponent.h" "SimComponent.cpp" "task.h" "ExampleSimComponent.h")

set_property(TARGET CoreLib PROPERTY CXX_STANDARD 20)

target_include_directories (CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# TODO: Add tests and install targets if needed.
