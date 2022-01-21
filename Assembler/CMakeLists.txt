# CMakeList.txt : CMake project for Assembler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_executable (Assembler "Assembler.cpp" "include/Assembler.h" "parser.cpp" "include/Jmp.h" "include/Instruction.h" "include/SourcePosition.h" "include/MemoryAccess.h" "include/Halt.h" "include/AddRegRegConst.h" "include/AddRegRegReg.h" "include/MoveRegConst.h" "include/MoveRegReg.h" "include/RestorePC.h" "include/StoreRegister.h" "include/MoveRegSymbol.h" "Linker.cpp" "include/Linker.h" "include/ObjectWriter.h" "ObjectWriter.cpp" "include/LoadRegister.h" "include/CmpRegReg.h" "include/Mnemonics.h" "include/ShlRegRegConst.h" "include/MoveRegBlockIndex.h" "include/DataObject.h" "include/LoadAddr.h")

target_link_libraries(Assembler LINK_PUBLIC CoreLib) 

target_include_directories (Assembler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(Assembler PRIVATE cxx_std_20)
#set_property(TARGET Assembler PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.
