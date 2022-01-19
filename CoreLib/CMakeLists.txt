# CMakeList.txt : CMake project for CoreLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_library (CoreLib  "include/CoreLib.h" "Cache.cpp" "include/SimComponent.h" "SimComponentRegistry.cpp" "Task.cpp" "include/Task.h" "include/ExampleSimComponent.h" "include/MachineInfo.h" "FetchStage.cpp" "include/FetchStage.h" "include/MemoryBus.h" "include/FetchDecodeBus.h" "include/Cache.h" "include/RAM.h" "RAM.cpp" "MachineInfo.cpp" "include/DecodeStage.h" "DecodeStage.cpp" "include/ExecuteStage.h" "include/StoreStage.h" "ExecuteStage.cpp" "StoreStage.cpp" "include/RegisterFile.h" "include/DecodeToExecuteBus.h" "include/ExecuteToStoreBus.h" "include/StoreToFetchBus.h" "include/Multiplexer.h" "include/Core.h" "include/CoreCluster.h" "include/CoreClusterGrid.h" "include/SimComponentRegistry.h" "include/Logger.h" "include/Stats.h" "include/MachineConfig.h" "include/units.h" "include/RAM_Config.h" )

target_compile_features(CoreLib PUBLIC cxx_std_20)

#target_link_options(CoreLib PUBLIC -fcoroutines)

#set_property(TARGET CoreLib PROPERTY CXX_STANDARD 20)

target_include_directories (CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TODO: Add tests and install targets if needed.
