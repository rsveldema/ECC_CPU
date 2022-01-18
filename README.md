# ECC_CPU

is a toolkit for experimenting with new processor designs.
It contains a cycle-accurate processor simulator and assembler.
The CPU architecture is:  
    - using a RISC instruction set;
    - pipelined;
    - 64 bit pointers with 32 bit instructions.
   
   TODO:
      - parallelism with kernel-like elements (OpenCL/Cuda/etc. compatible!)
      - add OpenCL implementation targetting the new architecture
        - SPIR-V?
        - LLVM?
      - run microcode on memory fetch latency
      - virtual memory support / memory mapping
      - shared memory between cores
     

The idea behind running code on fetch misses is to allow the CPU
to perform intelligent/application dependent prefetching
and task switching.


--------------

Linux installation:
 - sh config_linux.sh
 - cmake --build build
 
