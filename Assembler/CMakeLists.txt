# CMakeList.txt : CMake project for Assembler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_executable (Assembler "Assembler.cpp" "Assembler.h" "parser.cpp" "Jmp.h" "Instruction.h" "SourcePosition.h" "MemoryAccess.h" "Halt.h" "AddRegRegConst.h" "AddRegRegReg.h" "MoveRegConst.h" "MoveRegReg.h" "RestorePC.h" "StoreRegister.h" "MoveRegSymbol.h" "Linker.cpp" "Linker.h" "ObjectWriter.h" "ObjectWriter.cpp")

target_link_libraries(Assembler LINK_PUBLIC CoreLib) 


target_compile_features(Assembler PRIVATE cxx_std_20)
#set_property(TARGET Assembler PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.
