

all: run

compile:
	verilator -Wall --cc --exe --build sim_main.cpp main.sv -top-module main -Wno-unused 
	
run: compile
	obj_dir/Vmain