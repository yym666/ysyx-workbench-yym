#include "verilated.h"
#include "verilated_vcd_c.h"
#include "obj_dir/VNJU6.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIM_TIME 200

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;

static VNJU6* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new VNJU6;
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
        int a = rand()%255;
        int opt = rand()%8;
        if (i == 1) opt = 1;
        if (i & 1) top->clock = 1;
        else top->clock = 0;
        top->io_control = opt;
        top->io_input = a;
        step_and_dump_wave();       
        printf("%d\n", top->io_output);
    }
    sim_exit();
}
