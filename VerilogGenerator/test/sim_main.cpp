
#include "Vmain.h"
#include "verilated.h"

int main(int argc, char **argv, char **env)
{
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    auto *top = new Vmain{contextp};
    while (!contextp->gotFinish())
    {
        top->eval();
    }
    delete top;
    delete contextp;
    return 0;
}

