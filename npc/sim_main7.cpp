#include "verilated.h"
#include "verilated_vcd_c.h"
#include "obj_dir/VNJU7.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIM_TIME 200

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;

static VNJU7* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new VNJU7;
  contextp->traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open("dump.vcd");

  top->reset = 1;
  top->clock = 1;
  step_and_dump_wave();
}

void sim_exit(){
  step_and_dump_wave();
  tfp->close();
}

int main() {
    sim_init();

    for (int i = 1; i < SIM_TIME; ++i){
        top->clock = top->clock ^ 1;
        step_and_dump_wave();       
        if (i > 10) top->reset = 0;
        printf("%d\n", top->io_output);
    }
    sim_exit();
}
