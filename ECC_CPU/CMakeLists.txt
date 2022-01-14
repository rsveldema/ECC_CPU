# CMakeList.txt : CMake project for ECC_CPU, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
add_executable (ECC_CPU "ECC_CPU.cpp" "ECC_CPU.h")

target_link_libraries(ECC_CPU LINK_PUBLIC CoreLib)


set_property(TARGET ECC_CPU PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.
