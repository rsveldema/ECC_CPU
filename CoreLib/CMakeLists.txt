# CMakeList.txt : CMake project for CoreLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_library (CoreLib "CoreLib.cpp" "CoreLib.h" "SimComponent.h" "SimComponent.cpp" "task.h" "ExampleSimComponent.h" "MachineInfo.h" "FetchStage.cpp" "FetchStage.h" "MemoryBus.h" "FetchDecodeBus.h" "Cache.h" "DRAM.h" "DRAM.cpp" "MachineInfo.cpp" "DecodeStage.h" "DecodeStage.cpp" "ExecuteStage.h" "StoreStage.h" "ExecuteStage.cpp" "StoreStage.cpp" "RegisterFile.h" "DecodeToExecuteBus.h" "ExecuteToStoreBus.h" "StoreToFetchBus.h" "Multiplexer.h" "Core.h" "CoreCluster.h" "CoreClusterGrid.h" "SimComponentRegistry.h")

target_compile_features(CoreLib PUBLIC cxx_std_20)

#set_property(TARGET CoreLib PROPERTY CXX_STANDARD 20)

target_include_directories (CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# TODO: Add tests and install targets if needed.
