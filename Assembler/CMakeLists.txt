# CMakeList.txt : CMake project for Assembler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
add_executable (Assembler "Assembler.cpp" "Assembler.h")

target_link_libraries(Assembler LINK_PUBLIC CoreLib) 

set_property(TARGET Assembler PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.
