
#include "Vmain.h"
#include "verilated.h"

int main(int argc, char **argv, char **env)
{
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    auto *tb = new Vmain{contextp};
    while (!contextp->gotFinish())
    {
      tb->clk = 1;
      tb->eval();
      tb->clk = 0;
      tb->eval();
    }
    delete tb;
    delete contextp;
    return 0;
}

