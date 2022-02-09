
#include "Vmain.h"
#include "verilated.h"

int main(int argc, char **argv, char **env)
{
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    auto *tb = new Vmain{contextp};
    while (!contextp->gotFinish())
    {
		tb->i_clk = 1;
		tb->eval();
		tb->i_clk = 0;
		tb->eval();
    }
    delete top;
    delete contextp;
    return 0;
}

