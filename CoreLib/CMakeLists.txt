# CMakeList.txt : CMake project for CoreLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_library (CoreLib  "include/CoreLib.h" 
            "L1InsnCache.cpp" "include/SimComponent.h" 
        "SimComponentRegistry.cpp" "Task.cpp" 
        "include/Task.h" "include/ExampleSimComponent.h" 
        "include/MachineInfo.h" "FetchStage.cpp" 
        "include/FetchStage.h" "include/VecMemoryBus.h" 
        "include/FetchToDecodeBus.h" "include/L3Cache.h" 
        "include/DRAM.h" "DRAM.cpp" "MachineInfo.cpp" 
        "include/DecodeStage.h" "DecodeStage.cpp" 
        "include/ExecuteStage.h" "include/StoreStage.h" 
        "ExecuteStage.cpp" "StoreStage.cpp" 
        "include/RegisterFile.h" "include/DecodeToExecuteBus.h"
        "include/ExecuteToStoreBus.h" "include/StoreToFetchBus.h"
         "include/Multiplexer.h" "include/Core.h" 
         "include/CoreCluster.h" "include/CoreClusterGrid.h" 
         "include/SimComponentRegistry.h" "include/Logger.h" 
         "include/Stats.h" "include/MachineConfig.h" 
         "include/units.h" 
          "include/VectorValue.h" 
          "include/DivergenceQueue.h" 
          "L2InsnCache.cpp" "include/MemoryBus.h" "include/L2DataCache.h" "include/L2InsnCache.h" "include/L1InsnCache.h" "include/VectorMemoryController.h" "L1DataCache.cpp" "L2DataCache.cpp" "L3Cache.cpp" "VectorMemoryController.cpp")

target_compile_features(CoreLib PUBLIC cxx_std_20)

#target_link_options(CoreLib PUBLIC -fcoroutines)

#set_property(TARGET CoreLib PROPERTY CXX_STANDARD 20)

target_include_directories (CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TODO: Add tests and install targets if needed.
