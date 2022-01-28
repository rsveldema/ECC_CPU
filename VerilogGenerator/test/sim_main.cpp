
#include "VFetchStage.h"
#include "verilated.h"

int main(int argc, char **argv, char **env)
{
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    auto *top = new VFetchStage{contextp};
    while (!contextp->gotFinish())
    {
        top->eval();
    }
    delete top;
    delete contextp;
    return 0;
}

