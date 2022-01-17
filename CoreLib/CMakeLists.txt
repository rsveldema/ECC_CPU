# CMakeList.txt : CMake project for CoreLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_library (CoreLib  "CoreLib.h" "SimComponent.h" "SimComponentRegistry.cpp" "Task.h" "ExampleSimComponent.h" "MachineInfo.h" "FetchStage.cpp" "FetchStage.h" "MemoryBus.h" "FetchDecodeBus.h" "Cache.h" "RAM.h" "RAM.cpp" "MachineInfo.cpp" "DecodeStage.h" "DecodeStage.cpp" "ExecuteStage.h" "StoreStage.h" "ExecuteStage.cpp" "StoreStage.cpp" "RegisterFile.h" "DecodeToExecuteBus.h" "ExecuteToStoreBus.h" "StoreToFetchBus.h" "Multiplexer.h" "Core.h" "CoreCluster.h" "CoreClusterGrid.h" "SimComponentRegistry.h" "Logger.h" "Stats.h" "MachineConfig.h" "units.h" "RAM_Config.h")

target_compile_features(CoreLib PUBLIC cxx_std_20)

#set_property(TARGET CoreLib PROPERTY CXX_STANDARD 20)

target_include_directories (CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# TODO: Add tests and install targets if needed.
