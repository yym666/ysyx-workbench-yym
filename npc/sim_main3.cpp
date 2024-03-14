#include "verilated.h"
#include "verilated_vcd_c.h"
#include "obj_dir/VNJU3.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIM_TIME 200

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;

static VNJU3* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new VNJU3;
  contextp->traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open("dump.vcd");
}

void sim_exit(){
  step_and_dump_wave();
  tfp->close();
}

int main() {
    sim_init();

    for (int i = 1; i < SIM_TIME; ++i){
        int a = rand()%16;
        int b = rand()%16;
        int opt = rand()%8;
        top->io_opt = opt;
        top->io_a = a;
        top->io_b = b;
        step_and_dump_wave();       
        printf("%d\n", top->io_c);
    }
    sim_exit();
}
